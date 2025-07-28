#include "FreeTypeUtils.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <codecvt>

constexpr int kFontName = 4;
constexpr int kUnicodeSystem = 0;
constexpr int kWindowsSystem = 3;
constexpr int kEnglishLanguage = 1033;
constexpr int kChineseLanguage = 2052;
constexpr int kDefaultLanguageInUncode = 0;


std::vector<std::string> allSystemFontFiles()
{
    std::filesystem::path font_root = std::getenv("SystemRoot") + std::string("/Fonts/");
    std::vector<std::string> results;
    for (auto item : std::filesystem::recursive_directory_iterator(font_root))
    {
        if (item.is_directory())
        {
            continue;
        }
        std::string ext = item.path().extension().string();
        if (".ttf" == ext || ".ttc" == ext || ".TTF" == ext || ".TTC" == ext)
        {
            results.emplace_back(item.path().string());
        }
    }
    return results;
}

std::vector<FontInfo> getAllSystemFontInfos()
{
    FT_Library library;
    FT_Error error = FT_Init_FreeType(&library);
    if (error)
    {
        return {};
    }

    std::vector<FontInfo> results;
    std::vector<std::string> files = allSystemFontFiles();
    for (const std::string& font_file : files)
    {
        FT_Face face;
        error = FT_New_Face(library, font_file.c_str(), 0, &face);
        if (error)
        {
            continue;
        }
        FontInfo info;
        info.font_file = font_file;
        std::tie(info.font_name, info.chinese_language) = findBestFontName(face);
        results.emplace_back(info);
        // free face will destroy the FT_sfntName;
        FT_Done_Face(face);
    }
    FT_Done_FreeType(library);
    return results;
}

std::tuple<std::wstring, bool> findBestFontName(FT_Face face)
{
    std::vector<std::tuple<std::wstring, int>> availables; // font name, language_id
    FT_UInt count = FT_Get_Sfnt_Name_Count(face);
    for (FT_UInt idx = 0; idx < count; idx++)
    {
        FT_SfntName sfn;
        FT_Get_Sfnt_Name(face, idx, &sfn);
        if (sfn.name_id != kFontName)
        {
            continue;
        }
        if (sfn.language_id != kDefaultLanguageInUncode && sfn.language_id != kChineseLanguage && sfn.language_id != kEnglishLanguage)
        {
            continue;
        }
        unsigned char* ch = sfn.string;
        int leng = sfn.string_len;

        std::wstring wstr;
        // "Strings for the Unicode platform must be encoded in UTF-16BE." "All string data for platform 3 must be encoded in UTF-16BE." 
        // https://learn.microsoft.com/en-us/typography/opentype/spec/name
        if ((sfn.platform_id == 3) || (sfn.platform_id == 0))
        {
            wstr.reserve(sfn.string_len / 2);
            for (unsigned int i = 0; i < sfn.string_len / 2; i++)
            {
                wchar_t wch = ch[i * 2] * 256 + ch[i * 2 + 1]; // big endian: bigger address store the lastest data.
                wstr.push_back(wch);
            }
        }
        else
        {
            wstr.reserve(sfn.string_len);
            for (unsigned int i = 0; i < sfn.string_len; i++)
            {
                wchar_t wch = ch[i];
                wstr.push_back(wch);
            }
        }
        availables.emplace_back(std::make_tuple(wstr, sfn.language_id));
    }

    for (auto tp : availables)
    {
        if (std::get<int>(tp) == kChineseLanguage)
        {
            return std::make_tuple(std::get<std::wstring>(tp), true);
        }
    }
    for (auto tp : availables)
    {
        if (std::get<int>(tp) == kEnglishLanguage)
        {
            return std::make_tuple(std::get<std::wstring>(tp), false);
        }
    }
    if (!availables.empty())
    {
        return std::make_tuple(std::get<std::wstring>(availables.front()), false);
    }
    return std::make_tuple(std::wstring(), false);
}


std::string WstringToString(const std::wstring& wstr) noexcept
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;
    return converterX.to_bytes(wstr);
}

void outputFontInfos()
{
    std::string err_msg;
    std::ofstream ofs("font_file_font_name.txt", std::ios::binary);
    if (ofs.fail())
    {
        return;
    }
    std::vector<FontInfo> all_infos = getAllSystemFontInfos();
    for (const FontInfo& info : all_infos)
    {
        std::filesystem::path path = info.font_file;
        std::string stem = path.stem().string() + "        ";
        ofs.write(stem.c_str(), stem.size());
        std::string name = WstringToString(info.font_name);
        ofs.write(name.c_str(), name.size());
        ofs << std::endl;
    }
}


#include "Util/ArgumentUtil.h"


std::unordered_map<std::string, int> ArgumentUtil::m_values;
std::unordered_map<std::string, std::string> ArgumentUtil::m_strings;

void ArgumentUtil::initArguments(int argc, char* argv[])
{
    m_values.clear();
    m_strings.clear();
    for (int idx = 1; idx < argc; idx++)
    {
        const char* argument = argv[idx];
        std::string str(argument);
        std::size_t pos = str.find('=');
        if (pos == std::string::npos)
        {
            continue;
        }
        std::string key = str.substr(0, pos);
        std::string value = str.substr(pos + 1);
        try
        {
            int num = std::stoi(value);
            m_values[key] = num;
        }
        catch (const std::exception& e)
        {
            m_strings[key] = value;
        }
    }
}

bool ArgumentUtil::getValueByKey(const std::string& key, int& value)
{
    if (m_values.contains(key))
    {
        value = m_values.at(key);
        return true;
    }
    return false;
}

bool ArgumentUtil::getStringByKey(const std::string& key, std::string& value)
{
    if (m_strings.contains(key))
    {
        value = m_strings.at(key);
        return true;
    }
    return false;
}

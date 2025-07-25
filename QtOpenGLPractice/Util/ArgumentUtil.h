#pragma once

#include <string>
#include <unordered_map>

class ArgumentUtil
{
public:
    static void initArguments(int argc, char* argv[]);

    static bool getValueByKey(const std::string & key, int& value);

    static bool getStringByKey(const std::string& key, std::string& value);

private:
    static std::unordered_map<std::string, int> m_values;
    static std::unordered_map<std::string, std::string> m_strings;
};


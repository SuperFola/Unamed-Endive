#ifndef DEF_CFG
#define DEF_CFG

#include "../json/json.h"
#include <string>
#include <fstream>

class Config
{
private:
    Json::Value root;
    std::ifstream file;
    static Config self;
    Config();

public:
    static void load();
    static void close();
    static void save();
    static Json::Value get(const std::string& name);
};

#endif // DEF_CFG

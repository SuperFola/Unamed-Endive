#ifndef DEF_CFG
#define DEF_CFG

#include "../json/json.h"
#include <string>
#include <fstream>

class Config
{
private:
    Config& operator=(const Config&) {return self;}
    Config(const Config&) {}
    Json::Value root;
    std::ifstream file;
    static class Config self;
    Config();

public:
    static void load();
    static void close();
    static void save();
    static Json::Value get(const std::string& name);
};

#endif // DEF_CFG

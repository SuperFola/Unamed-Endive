#ifndef DEF_CFG
#define DEF_CFG

#include "../json/json.h"
#include <string>
#include <fstream>
#include "functions.hpp"

class Config
{
private:
    Config& operator=(const Config&) {return self;}
    Config(const Config&) {}
    Json::Value root;
    std::string name;
    static class Config self;
    Config();
    bool loaded;

public:
    static void load();
    static void save();
    static Json::Value get(const std::string& name);
};

#endif // DEF_CFG

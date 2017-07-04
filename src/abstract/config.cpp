#include <iostream>
#include "../../debug.hpp"

#include "config.hpp"

Config Config::self = Config();

Config::Config() :
    name("config.json")
{

}

void Config::load()
{
    self.root.clear();
    self.loaded = true;
    if (is_file_existing(self.name))
    {
        DebugLog(SH_INFO, "Loading config");
        std::ifstream file (self.name);
        file >> self.root;
    }
    else
    {
        DebugLog(SH_WARN, "Creating default config");
        self.loaded = false;

        self.root["music"] = true;
        self.root["aa"] = 1;
        self.root["v-sync"] = true;
        self.root["shader"] = "none";
        self.root["fps"] = 120;
        self.root["menu"] = "escape";
        self.root["up"] = "z";
        self.root["down"] = "s";
        self.root["right"] = "d";
        self.root["left"] = "q";
        self.root["sigma"] = 0.5f;
    }
}

void Config::save()
{
    DebugLog(SH_INFO, "Saving config");
    std::ofstream output("config.json");
    output << self.root;
}

Json::Value Config::get(const std::string& name)
{
    return self.root[name];
}

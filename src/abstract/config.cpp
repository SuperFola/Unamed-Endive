#include <iostream>
#include "../../debug.hpp"

#include "config.hpp"

Config Config::self = Config();

Config::Config() :
    file("config.json")
{

}

void Config::load()
{
    self.root.clear();
    self.file >> self.root;
}

void Config::close()
{
    self.file.close();
}

void Config::save()
{
    self.close();

    std::ofstream output("config.json");
    output << self.root;

    self.load();
}

Json::Value Config::get(const std::string& name)
{
    return self.root[name];
}

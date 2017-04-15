#include <iostream>
#include "../../debug.hpp"

#include "network.hpp"
#include "../scripting/scripting.hpp"

namespace Network
{
Connection Connection::self = Connection();

Connection::Connection() :
    connected(false)
{

}

void Connection::init(std::string h, int p, Protoc pr)
{
    self.HOST = h;
    self.PORT = p;
    self.PROTOC = pr;
    self.type = (self.PROTOC == Protoc::TCP) ? "TCP" : "UDP";
}

void Connection::start()
{
    PyScripting::run_code(("netconnect('" + self.HOST + "', " + to_string<int>(self.PORT) + ", '" + self.type + "')").c_str());
}

bool Connection::isSecured()
{
    return PyScripting::exec_net_req_getstr("secured?", self.type.c_str()) == "True";
}

int Connection::connect()
{
    int o = PyScripting::exec_net_req_getint("connect", self.type.c_str());
    while (o != self.PORT)
    {
        self.PORT = o;
        o = PyScripting::exec_net_req_getint("connect", self.type.c_str());
        if (o == 0)
            break;
    }
    if (o != 0)
    {
        self.connected = true;
        DebugLog(SH_SPE, "NET> Connected on " << self.HOST << ":" << self.PORT);
    }
    else
    {
        DebugLog(SH_ERR, "NET> Could not connect to " << self.HOST << ":" << self.PORT<< ", all servers can be buzy");
    }

    return o;
}

int Connection::auth(std::string name, std::string pass)
{
    if (self.connected)
    {
        pass = PyScripting::sha256crypt(pass.c_str());
        return PyScripting::exec_net_req_getint(("auth " + name + " " + pass).c_str(), self.type.c_str());
    }
    return -1;
}

void Connection::init_game(std::string skinname)
{
    if (self.connected)
    {
        PyScripting::exec_net_req_getstr(("qzd " + skinname).c_str(), self.type.c_str());
    }
}

Json::Value Connection::getMap(int mid)
{
    Json::Value map_root;
    if (self.connected)
    {
        map_root = PyScripting::exec_net_req_getjson(("map " + to_string<int>(mid)).c_str(), self.type.c_str());
    }
    return map_root;
}

Json::Value Connection::getPlayersOnMap(int mid)
{
    Json::Value root;
    if (self.connected)
    {
        root = PyScripting::exec_net_req_getjson(("playerson " + to_string<int>(mid)).c_str(), self.type.c_str());
    }
    return root;
}

void Connection::sendMsg(std::string msg)
{
    if (self.connected)
    {
        PyScripting::exec_net_req_getint(("sendmsg " + msg).c_str(), self.type.c_str());
    }
}

Json::Value Connection::getMsg(int quantity)
{
    Json::Value root;
    if (self.connected)
    {
        root = PyScripting::exec_net_req_getjson(("getmsg " + to_string<int>(quantity)).c_str(), self.type.c_str());
    }
    return root;
}

Json::Value Connection::getPNJCode()
{
    Json::Value root;
    if (self.connected)
    {
        root = PyScripting::exec_net_req_getjson("getpnjscode", self.type.c_str());
    }
    return root;
}

Json::Value Connection::getSkin(std::string name)
{
    Json::Value root;
    if (self.connected)
    {
        root = PyScripting::exec_net_req_getjson(("getskin " + name).c_str(), self.type.c_str());
    }
    return root;
}

bool Connection::setSkin(std::string name)
{
    if (self.connected)
    {
        return PyScripting::exec_net_req_getint(("setskin " + name).c_str(), self.type.c_str()) == 1;
    }
    return false;
}

std::string Connection::getTileset()
{
    if (self.connected)
    {
        return PyScripting::exec_net_req_getstr("gettileset", self.type.c_str());
    }
    return "";
}

Json::Value Connection::getInfo()
{
    Json::Value root;
    if (self.connected)
    {
        root = PyScripting::exec_net_req_getjson("about", self.type.c_str());
    }
    return root;
}

int Connection::quit()
{
    if (self.connected)
    {
        self.connected = false;
        return PyScripting::exec_net_req_getint("disconnect", self.type.c_str());
    }
    return -1;
}
}

#include <iostream>
#include "../../debug.hpp"

#include "network.hpp"
#include "../scripting/scripting.hpp"

Connection Connection::self = Connection();

Connection::Connection() :
    , connected(false)
{

}

void Connection::init(std::string h, int p, Protoc pr)
{
    self.HOST = h;
    self.PORT = p;
    self.PROTOC = p;
    self.type = (self.PROTOC == Protoc::TCP) ? "TCP" : "UDP";
}

void Connection::start()
{
    PyScripting::run_code(("netconnect('" + self.HOST + "', " + to_string<int>(self.PORT) + ", '" + self.type + "')").c_str());
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

    return o;
}

int Connection::auth(std::string name, std::string pass)
{
    if (self.connected)
    {
        pass = PyScripting::sha256crypt(pass);
        return PyScripting::exec_net_req_getint(("auth " + name + " " + pass).c_str(), self.type.c_str());
    }
    return -1;
}

Json::Value Connection::getMap(int mid)
{
    Json::Value map_root;
    if (self.connected)
    {
        return PyScripting::exec_net_req_getjson(("map " + to_string<int>(mid)).c_str(), self.type.c_str());
    }
    return map_root;
}

int Connection::quit()
{
    if (self.connected)
    {
        self.connected = false;
        return PyScripting::exec_net_req_getint("disconnect".c_str(), self.type.c_str());
    }
    return -1;
}

#include <iostream>
#include "../../debug.hpp"

#include "network.hpp"
#include "../scripting/scripting.hpp"

Connection::Connection(std::string h, int p, Protoc p) :
    HOST(h)
    , PORT(p)
    , PROTOC(p)
    , connected(false)
{
    this->type = (this->PROTOC == Protoc::TCP) ? "TCP" : "UDP";
}

void Connection::start()
{
    PyScripting::run_code(("netconnect('" + this->HOST + "', " + to_string<int>(this->PORT) + ", '" + this->type + "')").c_str());
}

int Connection::connect()
{
    int o = PyScripting::exec_net_req_getint("connect", this->type.c_str());
    while (o != this->PORT)
    {
        this->PORT = o;
        o = PyScripting::exec_net_req_getint("connect", this->type.c_str());
        if (o == 0)
            break;
    }
    if (o != 0)
    {
        this->connected = true;
        DebugLog(SH_SPE, "NET> Connected on " << this->HOST << ":" << this->PORT);
    }

    return o;
}

int Connection::auth(std::string name, std::string pass)
{
    if (this->connected)
    {
        pass = PyScripting::sha256crypt(pass);
        return PyScripting::exec_net_req_getint(("auth " + name + " " + pass).c_str(), this->type.c_str());
    }
    return -1;
}

int Connection::quit()
{
    if (this->connected)
    {
        this->connected = false;
        return PyScripting::exec_net_req_getint("disconnect".c_str(), this->type.c_str());
    }
    return -1;
}

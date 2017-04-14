#ifndef DEF_NETWORK
#define DEF_NETWORK

#include <string>
#include <vector>

#include "type.hpp"
#include "../abstract/functions.hpp"
#include "../json/json.h"

class Connection
{
private:
    std::string HOST;
    int PORT;
    Protoc PROTOC;
    bool connected;
    std::string type;
    static class Connection self;
    Connection();

public:
    static void init(std::string, int, Protoc);
    static void start();
    static int connect();
    static int auth(std::string, std::string);
    static Json::Value getMap(int);
    static int quit();
};

#endif // DEF_NETWORK

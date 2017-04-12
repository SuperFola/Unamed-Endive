#ifndef DEF_NETWORK
#define DEF_NETWORK

#include <string>
#include <vector>

#include "type.hpp"
#include "../abstract/functions.hpp"

class Connection
{
private:
    std::string HOST;
    int PORT;
    Protoc PROTOC;
    bool connected;
    std::string type;

public:
    Connection(std::string, int, Protoc);
    void start();
    int connect();
    int auth(std::string, std::string);
    int quit();
};

#endif // DEF_NETWORK

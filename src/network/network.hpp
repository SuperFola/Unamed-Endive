#ifndef DEF_NETWORK
#define DEF_NETWORK

#include <string>
#include <vector>

#include "type.hpp"
#include "../abstract/functions.hpp"
#include "../json/json.h"

namespace Network
{
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
    static bool isSecured();
    static int connect();
    static int auth(std::string, std::string);
    static void init_game(std::string);
    static Json::Value getMap(int);
    static Json::Value getPlayersOnMap(int);
    static void sendMsg(std::string);
    static Json::Value getMsg(int);
    static Json::Value getPNJCode();
    static Json::Value getSkin(std::string);
    static bool setSkin(std::string);
    static std::string getTileset();
    static Json::Value getInfo();
    static int quit();
};
}

#endif // DEF_NETWORK

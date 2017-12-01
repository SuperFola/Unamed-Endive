#include <iostream>
#include "../../debug.hpp"

#include <string>
#include <algorithm>

#include "scripting.hpp"
#include "../constants.hpp"
#include "../abstract/defines.hpp"
#include "../game.hpp"

/** Definitions of the functions related to the game **/
void PyScripting::setEvent(sf::Event& ev)
{
    instance.event = ev;
}

sf::Event PyScripting::getEvent()
{
    return instance.event;
}

int PyScripting::loadImage(const char* name, const char* id)
{
    std::string tname = std::string(name);
    std::string tid = std::string(id);

    sf::Texture tex;
    if (!tex.loadFromFile(tname))
       return -1;
    instance.textures.add(tid, tex);
    instance.sprites[tid] = sf::Sprite(instance.textures.get(tid));

    return 0;
}

int PyScripting::displayImage(const char* id, int x, int y)
{
    std::string tid = std::string(id);

    if (instance.sprites.find(tid) != instance.sprites.end())
    {
        instance.sprites[tid].setPosition(x, y);
        instance.window->draw(instance.sprites[tid]);
    }

    return 0;
}

int PyScripting::displayImageWithView(const char* id, int x, int y)
{
    std::string tid = std::string(id);

    if (instance.sprites.find(tid) != instance.sprites.end())
    {
        sf::Vector2f p2 = instance.window->mapPixelToCoords(sf::Vector2i(x, y));
        instance.sprites[tid].setPosition(p2);
        instance.window->draw(instance.sprites[tid]);
    }

    return 0;
}

const char* PyScripting::getCurrentMusicName()
{
    return instance.music_player->getCurrentName().data();
}

int PyScripting::getCurrentView()
{
    return instance.sm->getId();
}

int PyScripting::hasActiveHud(int vid)
{
    return instance.sm->hasActiveHud(instance.sm->getId());
}

void PyScripting::stopMusic()
{
    return instance.music_player->stop();
}

int PyScripting::playMusic(const char* name)
{
    return int(instance.music_player->play(std::string(name)));
}

void PyScripting::createPNJ(int mid, const char* name, const char* text, int pnjkind_i, const char* dname, int x, int y)
{
    PNJkind kind;
    switch (pnjkind_i)
    {
    case 0:
        kind = PNJkind::normal;
        break;

    case 1:
        kind = PNJkind::special;
        break;

    case 2:
        kind = PNJkind::system;
        break;

    default:
        kind = PNJkind::normal;
        break;
    }

    instance.pnjm->add_pnj_on_map(mid, std::string(name), std::string(text), kind, std::string(dname), x, y);
}

int PyScripting::map_is_tp(int x, int y)
{
    return (instance.level->is_tp(x, y)) ? 1 : 0;
}

int PyScripting::map_getMapFromTp(int x, int y)
{
    return instance.level->getMapFromTp(x + y * instance.level->getWidth());
}

int PyScripting::getMapWidth()
{
    return instance.level->getWidth();
}

int PyScripting::getMapHeight()
{
    return instance.level->getHeight();
}

int PyScripting::getMapId()
{
    return instance.level->getId();
}

void PyScripting::changeBlockAttrib(int rid, const char* identifier, int value)
{
    instance.level->setBlockAttrib(rid, std::string(identifier), bool(value));
}

const char* PyScripting::getPlayerName()
{
    return instance.player->getName().c_str();
}

const char* PyScripting::getPlayerFolder()
{
    return instance.player->getFolder().c_str();
}

void PyScripting::map_tpPlayerOn(int rid)
{
    instance.player->setPos(
                            rid % instance.level->getWidth()
                            , rid / instance.level->getWidth()
                            );
}

const char* PyScripting::screenshot()
{
    time_t t = time(0);
    struct tm* now = localtime(&t);
    sf::Vector2u windowSize = instance.window->getSize();
    sf::Texture texture;

    texture.create(windowSize.x, windowSize.y);
    texture.update(*(instance.window));

    sf::Image screenshot = texture.copyToImage();
    std::string name = std::string("screenshots/screenshot-")
                          + to_string<int>(now->tm_year + 1900)
                          + std::string("-")
                          + to_string<int>(now->tm_mon + 1)
                          + std::string("-")
                          + to_string<int>(now->tm_mday)
                          + std::string(" - ")
                          + to_string<int>(now->tm_hour)
                          + std::string("-")
                          + to_string<int>(now->tm_min)
                          + std::string("-")
                          + to_string<int>(now->tm_sec)
                          + std::string(".jpg");
    screenshot.saveToFile(name);

    return name.c_str();
}

std::string PyScripting::exec_net_req_getstr(const char* req, const char* t)
{
    std::string r = "netsend('" + std::string(req)+ "', '" + std::string(t) + "');" + "upr(netrecv())";
    std::string n = std::string(PyScripting::run_code_and_get_out(r.c_str()));
    return n.substr(1, n.size() - 2);
}

int PyScripting::exec_net_req_getint(const char* req, const char* t)
{
    std::string preq = PyScripting::exec_net_req_getstr(req, t);

    if (preq == "OK")
    {
        DebugLog(SH_WARN, "NET> " << preq);
        return 1;
    }
    else if (preq.size() >=5 && preq.substr(0, 5) == "WRONG")
    {
        DebugLog(SH_WARN, "NET> " << preq);
        return 0;
    }
    int ipreq = atoi(preq.c_str());
    DebugLog(SH_INFO, preq << " " << ipreq);
    return ipreq;
}

Json::Value PyScripting::exec_net_req_getjson(const char* req, const char* t)
{
    std::string preq = PyScripting::exec_net_req_getstr(req, t);
    Json::Value root;
    Json::Reader reader;

    bool parsed = reader.parse(preq.c_str(), root);
    if (!parsed)
    {
        DebugLog(SH_ERR, "Could not parse given json with network req " << req);
    }

    return root;
}

std::vector<int> PyScripting::exec_net_req_getvectorint(const char* req, const char* t)
{
    std::vector<std::string> strings = PyScripting::exec_net_req_getvectorstr(req, t);
    std::vector<int> temp;

    for (int i=0; i < strings.size(); i++)
    {
        int integer = atoi(strings[i].c_str());
        temp.push_back(integer);
    }

    return temp;
}

std::vector<std::string> PyScripting::exec_net_req_getvectorstr(const char* req, const char* t)
{
    std::string preq = PyScripting::exec_net_req_getstr(req, t);
    std::vector<std::string> temp;
    std::string test = "";

    for (int i=0; i < preq.size(); i++)
    {
        if (preq.at(i) != ' ')
        {
            test += preq.at(i);
        }
        else
        {
            temp.push_back(test);
            test = "";
        }
    }

    return temp;
}

std::string PyScripting::sha256crypt(const char* word)
{
    std::string n = std::string(PyScripting::run_code_and_get_out(("upr(sha256('" + std::string(word)+ "'))").c_str()));
    return n.substr(1, n.size() - 2);
}

void PyScripting::setCurrentView(int vid)
{
    instance.sm->change_view(vid);
}

int PyScripting::countCreaturesInEquip()
{
    return instance.player->getEquip()->getSize();
}

int PyScripting::countCreaturesInPC()
{
    return instance.player->getPC()->getSize();
}

void PyScripting::giveObject(int id, int qu, int pocket)
{
    instance.player->getBag()->getPocket(pocket)->add_object(id, qu);
}

void PyScripting::turnPlayer(int dir)
{
    instance.player->setDir(static_cast<DIRECTION>(dir));
}

void PyScripting::createText(int x, int y, int words, const char* text, int fs, int color, const char* id)
{
    int r = 0, g = 0, b = 0;
    sf::Text ntext;

    r = (color & 0xff0000) >> 16;
    g = (color & 0x00ff00) >> 8;
    b = (color & 0x0000ff);

    setupFont(ntext, instance.font, sf::Color(r, g, b), fs)
    ntext.setString(wrapText(sf::String(text), words, instance.font, fs));
    ntext.setPosition(x, y);

    instance.texts.add(std::string(id), ntext);
}

void PyScripting::writeText(const char* id)
{
    if (instance.texts.contains(std::string(id)))
    {
        if (instance.sm->getId() == DEFAULT_VIEW_ID)
        {
            sf::Vector2f n;
            n.x = instance.texts.get(std::string(id)).getPosition().x;
            n.y = instance.texts.get(std::string(id)).getPosition().y;

            sf::Vector2f p2 = instance.window->mapPixelToCoords(sf::Vector2i(instance.texts.get(std::string(id)).getPosition()));
            instance.texts.get(std::string(id)).setPosition(p2);

            instance.window->draw(instance.texts.get(std::string(id)));
            instance.texts.get(std::string(id)).setPosition(n);
        }
        else
            instance.window->draw(instance.texts.get(std::string(id)));
    }
}

void PyScripting::setFightEnv(int id)
{
    instance.sm->getFight()->set_env(static_cast<FightEnv>(id % static_cast<int>(FightEnv::FECount)));
}

void PyScripting::setFightEscape(bool e)
{
    instance.sm->getFight()->set_escape(e);
}

void PyScripting::triggerBalloonPrompt(const char* prompt, int& e, int len)
{
    if (!instance.game->get_triggered_inner_balloon_prompt())
    {
        instance.game->trigger_inner_balloon_prompt(true);
        instance.game->set_balloon_prompt(prompt, len);
        e = 0;
    }
    else
        e = 1;
}

const char* PyScripting::balloonPromptGetOuput()
{
    if (instance.game->get_triggered_inner_balloon_prompt())
    {
        return instance.game->get_inner_balloon_text();
    }
    else
        return 0;
}

void PyScripting::set_fight_opponents(fight_opponent fo_1)
{
    std::vector<fight_opponent> v;
    v.push_back(fo_1);
    instance.sm->getFight()->set_opponents(v);
}

void PyScripting::set_fight_opponents(fight_opponent fo_1, fight_opponent fo_2)
{
    std::vector<fight_opponent> v;
    v.push_back(fo_1);
    v.push_back(fo_2);
    instance.sm->getFight()->set_opponents(v);
}

void PyScripting::set_fight_opponents(fight_opponent fo_1, fight_opponent fo_2, fight_opponent fo_3)
{
    std::vector<fight_opponent> v;
    v.push_back(fo_1);
    v.push_back(fo_2);
    v.push_back(fo_3);
    instance.sm->getFight()->set_opponents(v);
}

int PyScripting::set_creature_name(int in, int id, const char* name)
{
    if (in == 0) // equip
    {
        if (0 <= id && id < instance.sm->getDefault()->getCharacter()->getEquip()->getSize())
        {
            instance.sm->getDefault()->getCharacter()->getEquip()->getCrea(id)->setName(std::string(name));
            return 0;
        }
        else
            return -1;
    }
    else
    {
        // pc
        if (0 <= id && id < instance.sm->getDefault()->getCharacter()->getPC()->getSize())
        {
            instance.sm->getDefault()->getCharacter()->getPC()->getCrea(id)->setName(std::string(name));
            return 0;
        }
        else
            return -1;
    }
}

void PyScripting::remove_object_from_pocket(int pocket_id, int id, int qu)
{
    instance.player->getBag()->getPocket(pocket_id)->drop_object_from_id(id, qu);
}

void PyScripting::add_object_to_pocket(int pocket_id, int id, int qu)
{
    instance.player->getBag()->getPocket(pocket_id)->add_object(id, qu);
}

void PyScripting::setPlayerName(const char* n)
{
    instance.player->setName(std::string(n));
}

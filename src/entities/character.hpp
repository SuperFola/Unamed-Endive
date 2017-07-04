#ifndef DEF_CHARACTER
#define DEF_CHARACTER

#include <string>
#include <vector>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

#include "../constants.hpp"
#include "../abstract/point.hpp"
#include "../map/map.hpp"
#include "../bag/bag.hpp"
#include "../abstract/functions.hpp"
#include "animatedentity.hpp"
#include "../abstract/equip.hpp"
#include "../objects/dex.hpp"
#include "pnjmanager.hpp"

enum class Sex {
    Female,
    Male
};

class Character : public AnimatedEntity {
private:
    // variables
    std::string folder;
    std::string name;
    Sex sex;
    Bag bag;
    Equip equip;
    Equip pc;
    Dex dex;
    PNJManager* pnjm;

public:
    // methods
    Character(Sex sex_=Sex::Male);
    ~Character();
    int chara_move(Map&, std::vector<float>) override;
    void chara_send_player_touch(Map&) override;
    bool pass_pnj(Map&, std::vector<float>) override;
    int save();
    void setFolder(const std::string&);
    std::string getName();
    std::string getFolder();
    void _load() override;
    Bag* getBag();
    Equip* getEquip();
    Equip* getPC();
    Dex* getDex();
    void setPNJm(PNJManager*);
};

#endif // DEF_CHARACTER

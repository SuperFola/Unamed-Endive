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
    std::string name;
    Sex sex;
    Bag bag;
    Equip equip;
    Equip pc;
    Dex dex;

public:
    // methods
    Character(Sex sex_=Sex::Male);
    int chara_move(Map&, std::vector<float>) override;
    int save();
    void setName(const std::string);
    std::string getName();
    void _load() override;
    void speak(int, PNJManager*);
    Bag* getBag();
    Equip* getEquip();
    Equip* getPC();
    Dex* getDex();
};

#endif // DEF_CHARACTER

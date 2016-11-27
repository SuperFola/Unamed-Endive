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
#include "../abstract/texturesmanager.hpp"
#include "../bag/bag.hpp"
#include "../abstract/functions.hpp"
#include "animatedentity.hpp"

enum class Sex {
    Female,
    Male
};

class Character : public AnimatedEntity {
private:
    // variables
    std::string name;
    Point pos;
    Sex sex;
    Bag bag;

public:
    // methods
    Character(Sex sex_=Sex::Male);
    int save();
    void setName(const std::string);
    void _load() override;
    Bag* getBag();
};

#endif // DEF_CHARACTER

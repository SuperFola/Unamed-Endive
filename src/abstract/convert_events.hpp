#ifndef DEF_CONVERTEVENTS
#define DEF_CONVERTEVENTS

#include <string>
#include <vector>
#include <SFML/Window/Event.hpp>

#include "functions.hpp"

#define TO_PYBOOL(name, boolean) name = (boolean) ? "True" : "False";

std::string eventToString(sf::Event);

#endif // DEF_CONVERTEVENTS

#include <iostream>

#include "../../debug.hpp"

#include "convert_events.hpp"

std::string eventToString(sf::Event ev)
{
    std::string sev;

    std::string alt, control, shift, system, vertical;

    switch (ev.type)
    {
        case sf::Event::Closed:
            sev = "{'type': 'closed'}";
            break;

        case sf::Event::Resized:
            sev = "{'type': 'resized', 'width': " + to_string<int>(ev.size.width) + ", 'height': " + to_string<int>(ev.size.height) + "}";
            break;

        case sf::Event::LostFocus:
            sev = "{'type': 'lostfocus'}";
            break;

        case sf::Event::GainedFocus:
            sev = "{'type': 'gainedfocus'}";
            break;

        case sf::Event::TextEntered:
            sev = "{'type': 'textentered', 'unicode': '" + to_string<unsigned int>(ev.text.unicode) + "'}";
            break;

        case sf::Event::KeyPressed:
            TO_PYBOOL(alt, ev.key.alt)
            TO_PYBOOL(control, ev.key.control)
            TO_PYBOOL(shift, ev.key.shift)
            TO_PYBOOL(system, ev.key.system)
            sev = "{'type': 'keypressed', 'key': " + to_string<int>(ev.key.code) + ", 'alt': " + alt + ", 'control': " + control + ", 'shift': " + shift + ", 'system': " + system + "}";
            break;

        case sf::Event::KeyReleased:
            TO_PYBOOL(alt, ev.key.alt)
            TO_PYBOOL(control, ev.key.control)
            TO_PYBOOL(shift, ev.key.shift)
            TO_PYBOOL(system, ev.key.system)
            sev = "{'type': 'keyreleased', 'key': " + to_string<int>(ev.key.code) + ", 'alt': " + alt + ", 'control': " + control + ", 'shift': " + shift + ", 'system': " + system + "}";
            break;

        case sf::Event::MouseWheelScrolled:
            TO_PYBOOL(vertical, ev.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
            sev = "{'type': 'mousewheelscrolled', 'vertical': " + vertical + ", 'delta': " + to_string<int>(ev.mouseWheelScroll.delta) + ", 'x': " + to_string<int>(ev.mouseWheelScroll.x) + ", 'y': " + to_string<int>(ev.mouseWheelScroll.y) + "}";
            break;

        case sf::Event::MouseButtonPressed:
            sev = "{'type': 'mousebuttonpressed', 'button': " + to_string<int>(ev.mouseButton.button) + ", 'x': " + to_string<int>(ev.mouseButton.x) + ", 'y': " + to_string<int>(ev.mouseButton.y) + "}";
            break;

        case sf::Event::MouseButtonReleased:
            sev = "{'type': 'mousebuttonreleased', 'button': " + to_string<int>(ev.mouseButton.button) + ", 'x': " + to_string<int>(ev.mouseButton.x) + ", 'y': " + to_string<int>(ev.mouseButton.y) + "}";
            break;

        case sf::Event::MouseMoved:
            sev = "{'type': 'mousemoved', 'x': " + to_string<int>(ev.mouseMove.x) + ", 'y': " + to_string<int>(ev.mouseMove.y) + "}";
            break;

        case sf::Event::MouseEntered:
            sev = "{'type': 'mouseentered'}";
            break;

        case sf::Event::MouseLeft:
            sev = "{'type': 'mouseleft'}";
            break;

        default:
            sev = "{'type': 'unrecognized'}";
    }

    return sev;
}

#include <iostream>

#include "statemachine.hpp"

// public
StateMachine::StateMachine() :
    loaded(0)
    , current_view(-1)
{
    this->history.reserve(MAX_HISTORY);
}

bool StateMachine::load(sf::String playername)
{
    int id = -1;
    switch(this->loaded)
    {
    case 0:
        id = this->defaultv.getId();
        this->defaultv.load(playername);
        break;

    case 1:
        id = this->creav.getId();
        this->creav.load();
        break;

    case 2:
        id = this->dexv.getId();
        this->dexv.load();
        break;

    case 3:
        id = this->savev.getId();
        this->savev.load();
        break;

    case 4:
        id = this->inventoryv.getId();
        this->inventoryv.load();
        break;

    case 5:
        id = this->mapv.getId();
        this->mapv.load();
        break;

    case 6:
        id = this->fightv.getId();
        this->fightv.load();
        break;
    }
    std::cout << "Loading view " << id << std::endl;
    this->loaded++;

    return this->loaded == 7;
}

int StateMachine::getId()
{
    return this->current_view;
}

bool StateMachine::hasActiveHud(int vid)
{
    bool ret_val = false;

    switch(vid)
    {
    case DEFAULT_VIEW_ID:
        ret_val = this->defaultv.hasActiveHud();
        break;

    case MYCREATURES_VIEW_ID:
        ret_val = this->creav.hasActiveHud();
        break;

    case DEX_VIEW_ID:
        ret_val = this->dexv.hasActiveHud();
        break;

    case SAVING_VIEW_ID:
        ret_val = this->savev.hasActiveHud();
        break;

    case INVENTORY_VIEW_ID:
        ret_val = this->inventoryv.hasActiveHud();
        break;

    case MAP_VIEW_ID:
        ret_val = this->mapv.hasActiveHud();
        break;

    case FIGHT_VIEW_ID:
        ret_val = this->fightv.hasActiveHud();
        break;

    default:
        ret_val = -1;
        break;
    }

    return ret_val;
}

int StateMachine::change_view(int new_view)
{
    if (new_view == -1)
        return 0;

    int ret_val = -1;

    switch(new_view)
    {
    case LAST_VIEW_ID:
        ret_val = (!this->go_back_to_last_view()) ? 1 : -1;
        break;

    case DEFAULT_VIEW_ID:
    case MYCREATURES_VIEW_ID:
    case DEX_VIEW_ID:
    case SAVING_VIEW_ID:
    case INVENTORY_VIEW_ID:
    case MAP_VIEW_ID:
        this->history.push_back(this->current_view);
        this->current_view = new_view;
        std::cout << "Changing view from id " << this->history[this->history.size() - 1] << " to id " << new_view << std::endl;
        ret_val = 1;
        break;

    case FIGHT_VIEW_ID:
        // note that here, we don't log that we were in a fight to avoid going back with the use of LAST_VIEW_ID
        this->current_view = new_view;
        std::cout << "Changing view (fight)  to id " << new_view << std::endl;
        ret_val = 1;
        break;

    default:
        ret_val = -1;
        break;
    }

    return ret_val;
}

int StateMachine::go_back_to_last_view()
{
    if (this->history.size() != 0)
    {
        int latest = pop<int>(&this->history, -1);
        if (latest != -1)
        {
            this->current_view = latest;
            std::cout << "Going back to view id " << latest << std::endl;
            return 0;
        }
        else
        {
            return -1;  // can not pop
        }
    }
    return -1; // history is empty
}

int StateMachine::process_event_current(sf::Event& event, sf::Time elapsed)
{
    int ret_val = -1;

    switch(this->current_view)
    {
    case DEFAULT_VIEW_ID:
        ret_val = this->defaultv.process_event(event, elapsed);
        break;

    case MYCREATURES_VIEW_ID:
        ret_val = this->creav.process_event(event, elapsed);
        break;

    case DEX_VIEW_ID:
        ret_val = this->dexv.process_event(event, elapsed);
        break;

    case SAVING_VIEW_ID:
        ret_val = this->savev.process_event(event, elapsed);
        break;

    case INVENTORY_VIEW_ID:
        ret_val = this->inventoryv.process_event(event, elapsed);
        break;

    case MAP_VIEW_ID:
        ret_val = this->mapv.process_event(event, elapsed);
        break;

    case FIGHT_VIEW_ID:
        ret_val = this->fightv.process_event(event, elapsed);
        break;

    default:
        ret_val = -1;
        break;
    }

    return ret_val;
}

void StateMachine::render_current(sf::RenderWindow& window)
{
    switch(this->current_view)
    {
    case DEFAULT_VIEW_ID:
        this->defaultv.render(window);
        break;

    case MYCREATURES_VIEW_ID:
        this->creav.render(window);
        break;

    case DEX_VIEW_ID:
        this->dexv.render(window);
        break;

    case SAVING_VIEW_ID:
        this->savev.render(window);
        break;

    case INVENTORY_VIEW_ID:
        this->inventoryv.render(window);
        break;

    case MAP_VIEW_ID:
        this->mapv.render(window);
        break;

    case FIGHT_VIEW_ID:
        this->fightv.render(window);
        break;

    default:
        break;
    }
}

void StateMachine::update_current(sf::RenderWindow& window, sf::Time elapsed)
{
    // set sf::view as default
    window.setView(window.getDefaultView());

    switch(this->current_view)
    {
    case DEFAULT_VIEW_ID:
        this->defaultv.update(window, elapsed);
        break;

    case MYCREATURES_VIEW_ID:
        this->creav.update(window, elapsed);
        break;

    case DEX_VIEW_ID:
        this->dexv.update(window, elapsed);
        break;

    case SAVING_VIEW_ID:
        this->savev.update(window, elapsed);
        break;

    case INVENTORY_VIEW_ID:
        this->inventoryv.update(window, elapsed);
        break;

    case MAP_VIEW_ID:
        this->mapv.update(window, elapsed);
        break;

    case FIGHT_VIEW_ID:
        this->fightv.update(window, elapsed);
        break;

    default:
        break;
    }
}

DefaultView* StateMachine::getDefault()
{
    return &this->defaultv;
}

CreaView* StateMachine::getCrea()
{
    return &this->creav;
}

DexView* StateMachine::getDex()
{
    return &this->dexv;
}

SaveView* StateMachine::getSave()
{
    return &this->savev;
}

InventView* StateMachine::getInventory()
{
    return &this->inventoryv;
}

MapView* StateMachine::getMap()
{
    return &this->mapv;
}

FightView* StateMachine::getFight()
{
    return &this->fightv;
}


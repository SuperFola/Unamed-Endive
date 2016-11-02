#ifndef DEF_SAVE_VIEW
#define DEF_SAVE_VIEW

#include "view.hpp"

class SaveView : public View
{
private:
    // variables

public:
    // methods
    SaveView();
    bool load() override;
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;
};

#endif // DEF_SAVE_VIEW



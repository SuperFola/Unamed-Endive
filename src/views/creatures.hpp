#ifndef DEF_CREA_VIEW
#define DEF_CREA_VIEW

#include "view.hpp"

class CreaView : public View
{
private:
    // variables

public:
    // methods
    CreaView();
    bool load() override;
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;
};

#endif // DEF_CREA_VIEW




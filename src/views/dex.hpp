#ifndef DEF_DEX_VIEW
#define DEF_DEX_VIEW

#include "view.hpp"

class DexView : public View
{
private:
    // variables

public:
    // methods
    DexView();
    bool load() override;
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;
};

#endif // DEF_DEX_VIEW


#ifndef DEF_DEFAULT_VIEW
#define DEF_DEFAULT_VIEW

#include "view.hpp"
#include "../particles/particles.hpp"

class DefaultView : public View
{
private:
    // variables
    sf::VertexArray triangle;
    ParticleSystem particles;

public:
    // methods
    DefaultView();
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&) override;
    void update(sf::RenderWindow&, sf::Time) override;
};

#endif // DEF_DEFAULT_VIEW

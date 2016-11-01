#ifndef DEF_PARTICLE_SYS
#define DEF_PARTICLE_SYS

#include <SFML/Graphics.hpp>
#include <math.h>

class ParticleSystem : public sf::Drawable, public sf::Transformable
{
private:
    // methods
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply transform
        states.transform *= getTransform();

        // no texture for the particles
        states.texture = NULL;

        // draw the vertex array
        target.draw(vertices, states);
    }

    struct Particle
    {
        sf::Vector2f velocity;
        sf::Time lifetime;
    };

    void resetParticle(std::size_t);

    // variables
    std::vector<Particle> particles;
    sf::VertexArray vertices;
    sf::Time lifetime;
    sf::Vector2f emitter;

public:
    // methods
    ParticleSystem(unsigned int);
    void setEmitter(sf::Vector2f);
    void update(sf::Time);
};

#endif // DEF_PARTICLE_SYS

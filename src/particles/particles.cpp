#include <random>

#include "particles.hpp"

// private
void ParticleSystem::resetParticle(std::size_t index)
{
    // give random speed and random lifetime to a particle
    float angle = (std::rand() % 360) * 3.14f / 180.f;
    float speed = (std::rand() % 72) + 100.f;
    this->particles[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
    this->particles[index].lifetime = sf::milliseconds((std::rand() % 2000) + 1000);

    // reset particle'vertex'position
    this->vertices[index].position = this->emitter;
}

// public
ParticleSystem::ParticleSystem(unsigned int count) :
    particles(count)
    , vertices(sf::Points, count)
    , lifetime(sf::seconds(3.0f))
    , emitter(0, 0)
{
}

void ParticleSystem::setEmitter(sf::Vector2f position)
{
    this->emitter = position;
}

void ParticleSystem::setColor(sf::Color cl)
{
    this->fill_cl = cl;
}

void ParticleSystem::update(sf::Time elapsed)
{
    for (std::size_t i = 0; i < this->particles.size(); ++i)
    {
        // update particle's lifetime
        Particle& p = particles[i];
        p.lifetime -= elapsed;

        // if the particle is dead, reboot it !
        if (p.lifetime <= sf::Time::Zero)
            resetParticle(i);

        // update position of the vertex
        this->vertices[i].position += p.velocity * elapsed.asSeconds();

        // update alpha of the particle
        float ratio = p.lifetime.asSeconds() / this->lifetime.asSeconds();
        this->vertices[i].color.r = this->fill_cl.r;
        this->vertices[i].color.g = this->fill_cl.g;
        this->vertices[i].color.b = this->fill_cl.b;
        this->vertices[i].color.a = static_cast<sf::Uint8>(ratio * 255);
    }
}

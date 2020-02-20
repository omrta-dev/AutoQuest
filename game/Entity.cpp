//
// Created by omar on 2/20/20.
//

#include "Entity.hpp"
#include <SFML/Graphics/Texture.hpp>

aik::Entity::Entity()
{
    this->setPosition(50, 50);
    sf::Texture texture;
    texture.create(32, 32);
    this->setTexture(texture);
    this->setColor(sf::Color(0, 255, 0));
}

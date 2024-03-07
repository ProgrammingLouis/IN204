#pragma once

#include <SFML/Graphics.hpp>

class MyDrawable
{
    public:
        virtual void destroyBody(b2World& world) = 0;
        virtual void draw(sf::RenderWindow& target, float pixPerMeter) = 0;

        //TODO check if needed
        virtual ~MyDrawable() {} // Virtual destructor
};
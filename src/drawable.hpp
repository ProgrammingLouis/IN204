#pragma once

#include <SFML/Graphics.hpp>

class MyDrawable
{
    public:
        virtual void draw(sf::RenderWindow& target, float pixPerMeter) = 0;
};
#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include <iostream>

#include "boxes.hpp"
#include "drawable.hpp"

class MyFinish : MyDrawable
{
    private:
        MyWindowStaticBox leftBox;
        MyWindowStaticBox bottomBox;
        MyWindowStaticBox rightBox;
    
    public:
        // Screen space variables
        sf::Vector2f posOnAttachedWin; // position in the window space of the box center

        MyFinish(const sf::Vector2f& winPosition, const sf::Vector2f& screenHalfSize, b2World& world, float pixPerMeter, MyWindow& window) :
            leftBox(sf::Vector2f(winPosition.x-screenHalfSize.x+10, winPosition.y), sf::Vector2f(10, screenHalfSize.y), world, pixPerMeter, window),
            bottomBox(sf::Vector2f(winPosition.x, winPosition.y+screenHalfSize.y/2), sf::Vector2f(screenHalfSize.x-20, screenHalfSize.y/2), world, pixPerMeter, window),
            rightBox(sf::Vector2f(winPosition.x+screenHalfSize.x-10, winPosition.y), sf::Vector2f(10, screenHalfSize.y), world, pixPerMeter, window)
        {
            leftBox.shape.setFillColor(sf::Color::Red);
            bottomBox.shape.setFillColor(sf::Color::Blue);
            rightBox.shape.setFillColor(sf::Color::Red);
        }

        void updatePosition(float pixPerMeter)
        {
            leftBox.updatePosition(pixPerMeter);
            bottomBox.updatePosition(pixPerMeter);
            rightBox.updatePosition(pixPerMeter);
        }


        //TODO Change the draw to draw a semi arc
        void draw(sf::RenderWindow& window, float pixPerMeter)
        {
            leftBox.draw(window, pixPerMeter);
            bottomBox.draw(window, pixPerMeter);
            rightBox.draw(window, pixPerMeter);
        }
};

#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include <iostream>

#include "boxes.hpp"
#include "virtual_method_classes.hpp"
#include "constants.h"

class MyFinish : MyDrawable, MyWindowStaticObject
{
    private:
        MyWindowStaticBox leftBox;
        MyWindowStaticBox bottomBox;
        MyWindowStaticBox rightBox;
    
    public:
        // Screen space variables
        sf::Vector2f posOnAttachedWin; // position in the window space of the box center

        MyFinish(const sf::Vector2f& winPosition, const sf::Vector2f& screenHalfSize, b2World& world, float pixPerMeter, MyWindow& window) :
            leftBox(sf::Vector2f(winPosition.x-screenHalfSize.x+10, winPosition.y), sf::Vector2f(10, screenHalfSize.y), 0, world, pixPerMeter, window),
            bottomBox(sf::Vector2f(winPosition.x, winPosition.y+screenHalfSize.y/2), sf::Vector2f(screenHalfSize.x-20, screenHalfSize.y/2), 0, world, pixPerMeter, window),
            rightBox(sf::Vector2f(winPosition.x+screenHalfSize.x-10, winPosition.y), sf::Vector2f(10, screenHalfSize.y), 0, world, pixPerMeter, window)
        {
            leftBox.shape.setFillColor(sf::Color::Red);
            bottomBox.shape.setFillColor(sf::Color::Blue);
            rightBox.shape.setFillColor(sf::Color::Red);
        }

        void destroyBody(b2World& world) {
            leftBox.destroyBody(world);
            bottomBox.destroyBody(world);
            rightBox.destroyBody(world);
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

        bool isInside(b2Vec2 simulationPos)
        {
            const b2Vec2 bottomBoxBodyPos = bottomBox.body->GetPosition();
            return simulationPos.x < bottomBoxBodyPos.x + FINISH_RANGE && simulationPos.x > bottomBoxBodyPos.x - FINISH_RANGE
                && simulationPos.y < bottomBoxBodyPos.y -MAIN_CIRCLE_RADIUS + FINISH_RANGE && simulationPos.y > bottomBoxBodyPos.y -MAIN_CIRCLE_RADIUS- FINISH_RANGE;
        }
};

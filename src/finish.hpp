#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include <iostream>

#include "boxes.hpp"
#include "virtual_method_classes.hpp"
#include "constants.h"


// Create an empty template MyFinish class to be specialized later
template <class T>
class MyFinish;


template <>
class MyFinish<MyWindow> : MyDrawable, MyWindowStaticObject
{
    private:
        MyWindowStaticBox<MyWindow> leftBox;
        MyWindowStaticBox<MyWindow> bottomBox;
        MyWindowStaticBox<MyWindow> rightBox;
        const sf::Vector2f screenHalfSize;
    
    public:
        // Screen space variables
        sf::Vector2f posOnAttachedWin; // position in the window space of the box center

        MyFinish(const sf::Vector2f& winPosition, const sf::Vector2f& screenHalfSize, b2World& world, float pixPerMeter, MyWindow& window) : screenHalfSize(screenHalfSize),
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

        void updatePositionIfDrag(float pixPerMeter)
        {
            leftBox.updatePositionIfDrag(pixPerMeter);
            bottomBox.updatePositionIfDrag(pixPerMeter);
            rightBox.updatePositionIfDrag(pixPerMeter);
        }

        void setBodyPosition(b2Vec2 position, float pixPerMeter)
        {
            leftBox.posOnAttachedWin = sf::Vector2f(position.x-screenHalfSize.x+10, position.y);
            bottomBox.posOnAttachedWin = sf::Vector2f(position.x, position.y+screenHalfSize.y/2);
            rightBox.posOnAttachedWin = sf::Vector2f(position.x+screenHalfSize.x-10, position.y);

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


template <>
class MyFinish<MyStaticWindow> : MyDrawable, MyWindowStaticObject
{
    private:
        MyWindowStaticBox<MyStaticWindow> leftBox;
        MyWindowStaticBox<MyStaticWindow> bottomBox;
        MyWindowStaticBox<MyStaticWindow> rightBox;
        const sf::Vector2f screenHalfSize;
    
    public:
        // Screen space variables
        sf::Vector2f posOnAttachedWin; // position in the window space of the box center

        MyFinish(const sf::Vector2f& winPosition, const sf::Vector2f& screenHalfSize, b2World& world, float pixPerMeter, MyStaticWindow& window) : screenHalfSize(screenHalfSize),
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

        void setBodyPosition(b2Vec2 position, float pixPerMeter)
        {
            leftBox.posOnAttachedWin = sf::Vector2f(position.x-screenHalfSize.x+10, position.y);
            bottomBox.posOnAttachedWin = sf::Vector2f(position.x, position.y+screenHalfSize.y/2);
            rightBox.posOnAttachedWin = sf::Vector2f(position.x+screenHalfSize.x-10, position.y);

            leftBox.setBodyPosition(b2Vec2(position.x-screenHalfSize.x+10, position.y), pixPerMeter);
            bottomBox.setBodyPosition(b2Vec2(position.x, position.y+screenHalfSize.y/2), pixPerMeter);
            rightBox.setBodyPosition(b2Vec2(position.x+screenHalfSize.x-10, position.y), pixPerMeter);
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

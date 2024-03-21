#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

class MyDrawable
{
    public:
        virtual void destroyBody(b2World& world) = 0;
        virtual void draw(sf::RenderWindow& target, float pixPerMeter) = 0;

        virtual ~MyDrawable() {} // Virtual destructor needed for polymorphism when deleting objects
};

class MyWindowStaticObject
{
    public:
        virtual void updatePositionIfDrag(float pixPerMeter) = 0;
};

class MyDynamicObject
{
    public:
        b2Body* body;
        b2Vec2 startDagPos;
        int windowID; // The id of the window in which the object is
};
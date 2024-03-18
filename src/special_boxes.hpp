#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include <iostream>

#include "virtual_method_classes.hpp"
#include "window.hpp"
#include "boxes.hpp"

class MyWindowStaticAccelerator : MyWindowStaticBox<MyWindow>
{
    public:
        MyWindowStaticAccelerator(const sf::Vector2f& position, const sf::Vector2f& size, b2World& world, float pixPerMeter, int windowID) : MyWindowStaticBox<MyWindow>(position, size, world, pixPerMeter, windowID)
        {
            body->GetFixtureList()->SetSensor(true);
        }

        void updatePositionIfDrag(float pixPerMeter) override
        {
            if (body->GetType() == b2_staticBody)
            {
                body->SetTransform(b2Vec2(position.x / pixPerMeter, position.y / pixPerMeter), 0);
            }
        }
};
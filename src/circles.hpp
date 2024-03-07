#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include <iostream>

#include "virtual_method_classes.hpp"

class MyDynamicCircle : MyDrawable
{
    public:
        // SFML shape
        sf::CircleShape shape;

        // Screen space variables
        sf::Vector2f sreenPosition; // position in the screen space of the circle center
        sf::Vector2f posOnWin; // position in the window space of the circle center
        float screenRadius; // radius of the circle shape

        // Box2D objects
        b2BodyDef bodyDef;
        b2Body* body;
        b2CircleShape CircleShape;
        b2FixtureDef fixtureDef;

        MyDynamicCircle(const sf::Vector2f& screenPosition, const float screenRadius, b2World& world, float pixPerMeter) : sreenPosition(screenPosition), posOnWin(0, 0), shape(screenRadius), screenRadius(screenRadius)
        {
            shape = sf::CircleShape(screenRadius);
            shape.setFillColor(sf::Color::Blue);
            shape.setOrigin(screenRadius, screenRadius);

            //? Where is locate the position of the Box2D body on it shape ? (center, top-left corner, bottom-right corner, etc.)
            //? The position of the Box2D body is located at the center of the shape

            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(screenPosition.x/pixPerMeter, screenPosition.y/pixPerMeter);
            body = world.CreateBody(&bodyDef);

            CircleShape.m_radius = screenRadius/pixPerMeter;

            fixtureDef.shape = &CircleShape;
            fixtureDef.density = 1.0f;
            fixtureDef.friction = 10.3f;

            body->CreateFixture(&fixtureDef);
        }

        void destroyBody(b2World& world) {
            world.DestroyBody(body);
        }

        void draw(sf::RenderWindow& window, float pixPerMeter)
        {
            sreenPosition = sf::Vector2f(body->GetPosition().x*pixPerMeter, body->GetPosition().y*pixPerMeter);

            sf::Vector2i winPos = window.getPosition();
            posOnWin = sreenPosition - (sf::Vector2f)winPos;

            sf::Vector2u winSize = window.getSize();

            // Only draw the object if it is inside the window
            
            if (posOnWin.x+screenRadius > 0 && posOnWin.x-screenRadius < winSize.x && posOnWin.y+screenRadius > 0 && posOnWin.y-screenRadius < winSize.y)
            {
                shape.setPosition(posOnWin);
                shape.setRotation(body->GetAngle()*180/b2_pi);
                window.draw(shape);
            }
        };
};
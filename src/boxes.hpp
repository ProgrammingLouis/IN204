#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <Box2D/Box2D.h>

#include <iostream>


class MyDynamicBox
{
    public:
        // SFML shape
        sf::RectangleShape shape;

        // Screen space variables
        sf::Vector2f sreenPosition; // position in the screen space of the box center
        sf::Vector2f posOnWin; // position in the window space of the box center
        sf::Vector2f screenHalfSize; // half size of the rectangle shape

        // Box2D objects
        b2BodyDef bodyDef;
        b2Body* body;
        b2PolygonShape dynamicBox;
        b2FixtureDef fixtureDef;

        MyDynamicBox(const sf::Vector2f& screenPosition, const sf::Vector2f& screenHalfSize, b2World& world, float pixPerMeter) : sreenPosition(screenPosition), posOnWin(0, 0), shape(screenHalfSize), screenHalfSize(screenHalfSize)
        {
            shape = sf::RectangleShape(sf::Vector2f(screenHalfSize.x*2, screenHalfSize.y*2));
            shape.setFillColor(sf::Color::Blue);
            shape.setOrigin(screenHalfSize.x, screenHalfSize.y);

            //? Where is locate the position of the Box2D body on it shape ? (center, top-left corner, bottom-right corner, etc.)
            //? The position of the Box2D body is located at the center of the shape

            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(screenPosition.x/pixPerMeter, screenPosition.y/pixPerMeter);
            body = world.CreateBody(&bodyDef);

            dynamicBox.SetAsBox((screenHalfSize.x)/pixPerMeter, (screenHalfSize.y)/pixPerMeter);

            fixtureDef.shape = &dynamicBox;
            fixtureDef.density = 10.0f;
            fixtureDef.friction = 0.3f;

            body->CreateFixture(&fixtureDef);
        }

        void draw(sf::RenderWindow& window, float pixPerMeter)
        {
            sreenPosition = sf::Vector2f(body->GetPosition().x*pixPerMeter, body->GetPosition().y*pixPerMeter);

            sf::Vector2i winPos = window.getPosition();
            posOnWin = sreenPosition - (sf::Vector2f)winPos;

            sf::Vector2u winSize = window.getSize();

            // Only draw the object if it is inside the window
            
            if (posOnWin.x+screenHalfSize.x > 0 && posOnWin.x-screenHalfSize.x < winSize.x && posOnWin.y+screenHalfSize.y > 0 && posOnWin.y-screenHalfSize.y < winSize.y)
            {
                shape.setPosition(posOnWin);
                shape.setRotation(body->GetAngle()*180/b2_pi);
                window.draw(shape);
            }
        };
};



class MyStaticBox
{
    public:
        // SFML shape
        sf::RectangleShape shape;

        // Screen space variables
        sf::Vector2f sreenPosition; // position in the screen space of the box center
        sf::Vector2f posOnWin; // position in the window space of the box center
        sf::Vector2f screenHalfSize; // half size of the rectangle shape

        // Box2D objects
        b2BodyDef bodyDef;
        b2Body* body;
        b2PolygonShape staticBox;

        ////////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// \param screenHalfSize !! HALF Size of the rectangle shape
        ///
        ////////////////////////////////////////////////////////////
        MyStaticBox(const sf::Vector2f& screenPosition, const sf::Vector2f& screenHalfSize, b2World& world, float pixPerMeter) : sreenPosition(screenPosition), posOnWin(0, 0), shape(screenHalfSize), screenHalfSize(screenHalfSize)
        {
            shape = sf::RectangleShape(sf::Vector2f(screenHalfSize.x*2, screenHalfSize.y*2));
            shape.setFillColor(sf::Color::Cyan);
            shape.setOrigin(screenHalfSize.x, screenHalfSize.y);

            //? Where is locate the position of the Box2D body on it shape ? (center, top-left corner, bottom-right corner, etc.)
            //? The position of the Box2D body is located at the center of the shape

            bodyDef.position.Set(screenPosition.x/pixPerMeter, screenPosition.y/pixPerMeter);
            body = world.CreateBody(&bodyDef);

            staticBox.SetAsBox((screenHalfSize.x)/pixPerMeter, (screenHalfSize.y)/pixPerMeter);

            body->CreateFixture(&staticBox, 0.0f);
        }

        void draw(sf::RenderWindow& window, float pixPerMeter)
        {
            // std::cout << "Static box position: " << sreenPosition.x << ", " << sreenPosition.y << std::endl;

            sf::Vector2i winPos = window.getPosition();
            posOnWin = sreenPosition - (sf::Vector2f)winPos;

            sf::Vector2u winSize = window.getSize();

            // Only draw the object if it is inside the window
            if (posOnWin.x+screenHalfSize.x > 0 && posOnWin.x-screenHalfSize.x < winSize.x && posOnWin.y+screenHalfSize.y > 0 && posOnWin.y-screenHalfSize.y < winSize.y)
            {
                shape.setPosition(posOnWin);
                window.draw(shape);
            }
        };
};

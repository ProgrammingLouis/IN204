#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include <iostream>

#include "virtual_method_classes.hpp"
#include "window.hpp"


class MyDynamicBox : MyDrawable
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
        b2PolygonShape PolygonShape;
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

            PolygonShape.SetAsBox((screenHalfSize.x)/pixPerMeter, (screenHalfSize.y)/pixPerMeter);

            fixtureDef.shape = &PolygonShape;
            fixtureDef.density = 0.5f;
            fixtureDef.friction = 0.3f;

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
            
            if (posOnWin.x+screenHalfSize.x > 0 && posOnWin.x-screenHalfSize.x < winSize.x && posOnWin.y+screenHalfSize.y > 0 && posOnWin.y-screenHalfSize.y < winSize.y)
            {
                shape.setPosition(posOnWin);
                shape.setRotation(body->GetAngle()*180/b2_pi);
                window.draw(shape);
            }
        };
};



class MyStaticBox : MyDrawable
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
        b2PolygonShape PolygonShape;

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

            bodyDef.position.Set(screenPosition.x/pixPerMeter, screenPosition.y/pixPerMeter);
            body = world.CreateBody(&bodyDef);

            PolygonShape.SetAsBox((screenHalfSize.x)/pixPerMeter, (screenHalfSize.y)/pixPerMeter);

            body->CreateFixture(&PolygonShape, 0.0f);
        }

        void destroyBody(b2World& world) {
            world.DestroyBody(body);
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

////////////////////////////////////////////////////////////
/// \brief Class that represents a static box attached to the window and that moves with it
///
////////////////////////////////////////////////////////////
class MyWindowStaticBox : MyDrawable, MyWindowStaticObject
{
    public:
        // SFML shape
        sf::RectangleShape shape;

        // Screen space variables
        sf::Vector2f posOnAttachedWin; // position in the window space of the box center
        sf::Vector2f screenHalfSize; // half size of the rectangle shape

        MyWindow& window; // reference to the window where the box is attached

        // Box2D objects
        b2BodyDef bodyDef;
        b2Body* body;
        b2PolygonShape PolygonShape;

        ////////////////////////////////////////////////////////////
        /// \brief Default constructor
        /// 
        /// \param winPosition !! Position of the rectangle shape in the window space
        /// \param screenHalfSize !! HALF Size of the rectangle shape
        ///
        ////////////////////////////////////////////////////////////
        MyWindowStaticBox(const sf::Vector2f& winPosition, const sf::Vector2f& screenHalfSize, b2World& world, float pixPerMeter, MyWindow& window) : posOnAttachedWin(winPosition), screenHalfSize(screenHalfSize), window(window)
        {
            shape = sf::RectangleShape(sf::Vector2f(screenHalfSize.x*2, screenHalfSize.y*2));
            shape.setFillColor(sf::Color::Cyan);
            shape.setOrigin(screenHalfSize.x, screenHalfSize.y);

            bodyDef.type = b2_kinematicBody;
            bodyDef.position.Set((window.getPosition().x+winPosition.x)/pixPerMeter, (window.getPosition().y+winPosition.y)/pixPerMeter);
            body = world.CreateBody(&bodyDef);

            PolygonShape.SetAsBox((screenHalfSize.x)/pixPerMeter, (screenHalfSize.y)/pixPerMeter);

            body->CreateFixture(&PolygonShape, 0.0f);
        }

        void destroyBody(b2World& world) {
            world.DestroyBody(body);
        }

        // TODO Also update dynamic object position like the main circle and the dynamic boxes
        void updatePosition(float pixPerMeter)
        {
            //TODO This will call getIsDragging() for each box, maybe it is better to call it only once
            if (!window.getIsDragging()) return;
            body->SetAwake(true);
            body->SetTransform(b2Vec2((window.getPosition().x+posOnAttachedWin.x)/pixPerMeter, (window.getPosition().y+posOnAttachedWin.y)/pixPerMeter), 0);
        }

        void draw(sf::RenderWindow& window, float pixPerMeter)
        {
            //TODO check if this is optimized or not
            // if the window is the same as the one where the box is attached we just need to draw the shape
            if (&window == (MyWindow*)(&this->window)) {
                shape.setPosition(posOnAttachedWin);
                window.draw(shape);
                return;
            }
            // Otherwise we need to calculate the position of the box in the window space
            
            sf::Vector2i winPos = window.getPosition();
            sf::Vector2u winSize = window.getSize();

            sf::Vector2f posOnWin = sf::Vector2f(body->GetPosition().x*pixPerMeter, body->GetPosition().y*pixPerMeter) - (sf::Vector2f)winPos;

            // Only draw the object if it is inside the window
            // TODO optimize visibility by getting only the overlapping windows to draw the object
            if (posOnWin.x+screenHalfSize.x > 0 && posOnWin.x-screenHalfSize.x < winSize.x && posOnWin.y+screenHalfSize.y > 0 && posOnWin.y-screenHalfSize.y < winSize.y)
            {
                shape.setPosition(posOnWin);
                window.draw(shape);
            }
        };
};

/*
This file defines a screen object class that will be used to encapsulate
all objects that have their position defined in the screen space.
These objects will typically be able to move throw different windows on screen
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <iostream>


class ScreenObject
{
    public:
        sf::Vector2f sreenPosition;
        sf::Vector2f posOnWin;
        //! The circle shape has to have its origin as default for visibility box to work
        sf::CircleShape circle;
        sf::Vector2f visibilityBox;

    
        ScreenObject(const sf::Vector2f& position, sf::CircleShape circle) : sreenPosition(position), posOnWin(0, 0), circle(circle)
        {
            // circle.setOrigin(circle.getRadius(), circle.getRadius());
            visibilityBox = sf::Vector2f(circle.getRadius() * 2, circle.getRadius() * 2);
        }
        virtual ~ScreenObject() = default;

        void draw(sf::RenderWindow& window)
        {
            sf::Vector2i winPos = window.getPosition();
            posOnWin = sreenPosition - (sf::Vector2f)winPos;

            sf::Vector2u winSize = window.getSize();

            // Only draw the object if it is inside the window
            if (posOnWin.x + visibilityBox.x > 0 && posOnWin.x < winSize.x && posOnWin.y + visibilityBox.y > 0 && posOnWin.y < winSize.y)
            {
                // std::cout << "Drawing object at: " << posOnWin.x << ", " << posOnWin.y << std::endl;
                circle.setPosition(posOnWin);
                window.draw(circle);
            }
            // } else std::cout << "Object not visible" << std::endl;
        };

};

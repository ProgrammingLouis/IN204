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

template <typename ShapeType>
class ScreenObject
{
    public:
        sf::Vector2f sreenPosition;
        sf::Vector2f posOnWin;
        //! The circle shape has to have its origin as default for visibility box to work
        ShapeType shape;
        sf::Vector2f visibilityBox;

    
        ScreenObject(const sf::Vector2f& position, ShapeType shape) : sreenPosition(position), posOnWin(0, 0), shape(shape)
        {
            // circle.setOrigin(circle.getRadius(), circle.getRadius());
            // To create the visibility box, use getRadius() if the shape is a circle and getSize() if it is a rectangle
            if constexpr (std::is_same<ShapeType, sf::CircleShape>::value)
            {
                visibilityBox = sf::Vector2f(shape.getRadius(), shape.getRadius());
            }
            else if constexpr (std::is_same<ShapeType, sf::RectangleShape>::value)
            {
                visibilityBox = shape.getSize();
            }
            else
            {
                std::cerr << "Shape type not supported" << std::endl;
            }
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
                shape.setPosition(posOnWin);
                window.draw(shape);
            }
            // } else std::cout << "Object not visible" << std::endl;
        };

};

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>

class MyWindow : public sf::RenderWindow
{
    private:
        sf::Vector2i mouseStartDragPos; // position of the mouse in desktop coordonates when the drag starts
        bool isDragging = false; // is the window being dragged
        sf::Vector2i startDragWindowPos; // position of the window when the drag starts

        // bool posUpdated = false;
        // sf::Vector2i windowPos;

    public:
        MyWindow(const sf::VideoMode& videoMode, const std::string& title, sf::Uint32 style, const sf::ContextSettings& settings) : sf::RenderWindow(videoMode, title, style, settings)
        {
            setVerticalSyncEnabled(true);
        }

        void pollEvents() {
            for (auto event = sf::Event{}; sf::RenderWindow::pollEvent(event);)
            {
                if (event.type == sf::Event::Closed)
                    close();
                else if (event.type == sf::Event::Resized)
                    setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                else if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (event.mouseButton.button != sf::Mouse::Button::Left) continue;
                    startDragWindowPos = getPosition();
                    mouseStartDragPos = sf::Mouse::getPosition();
                    isDragging = true;
                    // std::cout << "Mouse button pressed " << event.mouseButton.button << " at " << event.mouseButton.x << ", " << event.mouseButton.y << std::endl;
                } else if (event.type == sf::Event::MouseButtonReleased) {
                    if (event.mouseButton.button != sf::Mouse::Button::Left) continue;
                    isDragging = false;
                }
            }
        }

        void updatePosition() {
            if (isDragging)
            {

                sf::Vector2i mousePos = sf::Mouse::getPosition();
                sf::Vector2i mouseDelta = mousePos - mouseStartDragPos;
                setPosition(startDragWindowPos+mouseDelta/2);
                // posUpdated = true;

                // windowPos = startDragWindowPos+sf::Vector2i(mouseDelta.x/2.1f, mouseDelta.y/2.1f);
            }
            // else posUpdated = false;
        }

        // sf::Vector2i getPosition() const {
        //     if (posUpdated) {
        //         return windowPos;
        //     }
        //     return sf::RenderWindow::getPosition();
        // }

        // void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default){
        //     clear();
            
        //     // draw
        //     display();
        // }

};

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>

class MyWindow : public sf::RenderWindow
{
    private:
        sf::Vector2i mouseStartDragPos; // position of the mouse in desktop coordonates when the drag starts
        bool isDragging = false; // is the window being dragged
        bool isDraggingFocus = false; // is the window started dragging when it gained focus (it is used to check for the mouse release which will not be sent to the window)
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
                //TODO try to replace the if-else with a switch
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
                    isDraggingFocus = false;
                } else if (event.type == sf::Event::MouseButtonReleased) {
                    if (event.mouseButton.button != sf::Mouse::Button::Left) continue;
                    isDragging = false;
                    isDraggingFocus = false;
                } else if (event.type == sf::Event::GainedFocus) {
                    // if we gained focus and the left mouse button is pressed it means we started dragging
                    if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) continue;
                    startDragWindowPos = getPosition();
                    mouseStartDragPos = sf::Mouse::getPosition();
                    isDragging = true;
                    isDraggingFocus = true;
                } else if (event.type == sf::Event::LostFocus) {
                    isDragging = false;
                    isDraggingFocus = false;
                }
            }
            if (isDraggingFocus && !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                isDragging = false;
                isDraggingFocus = false;
            }
        }

        void updatePosition(std::vector<MyWindow*> windows) {
            if (isDragging)
            {

                sf::Vector2i mousePos = sf::Mouse::getPosition();
                sf::Vector2i mouseDelta = mousePos - mouseStartDragPos;
                sf::Vector2i newPos = startDragWindowPos+mouseDelta/2;

                // if the new position overlaps with another window, put the window at the border of the other window
                for (MyWindow* window : windows)
                {
                    if (window == this) continue;
                    sf::Vector2i otherPos = window->getPosition();
                    sf::Vector2u otherSize = window->getSize();
                    if (newPos.x < otherPos.x+otherSize.x && newPos.x+getSize().x > otherPos.x && newPos.y < otherPos.y+otherSize.y && newPos.y+getSize().y > otherPos.y)
                    {
                        // if (newPos.x < otherPos.x+otherSize.x || newPos.y < otherPos.y+otherSize.y) {
                            int distBorderRight = otherPos.x+otherSize.x - newPos.x;
                            int distBorderBottom = (otherPos.y+otherSize.y) - newPos.y;
                            int distBorderLeft = newPos.x+getSize().x - otherPos.x;
                            int distBorderTop = newPos.y+getSize().y - otherPos.y;

                            if (distBorderRight < distBorderBottom && distBorderRight < distBorderLeft && distBorderRight < distBorderTop) {
                                newPos.x = otherPos.x+otherSize.x;
                            } else if (distBorderBottom < distBorderLeft && distBorderBottom < distBorderTop) {
                                newPos.y = otherPos.y+otherSize.y;
                            } else if (distBorderLeft < distBorderTop) {
                                newPos.x = otherPos.x-getSize().x;
                            } else {
                                newPos.y = otherPos.y-getSize().y;
                            }

                            // if (distBorderRight < distBorderBottom) {
                            //     newPos.x = otherPos.x+otherSize.x;
                            // } else {
                            //     newPos.y = otherPos.y+otherSize.y;
                            // }
                        // }
                    }
                }

                setPosition(newPos);


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

        void draw(std::vector<MyDrawable*> drawables, float pixPerMeter) {
            clear();
            for (MyDrawable* drawable : drawables)
            {
                drawable->draw(*this, pixPerMeter);
            }
            display();
        }

        bool getIsDragging() const {
            return isDragging;
        }

};

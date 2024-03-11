#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <Box2D/Box2D.h>

int windowsCount = 0; //FOR DEBUGGING

class MyWindow : public sf::RenderWindow
{
    private:
        sf::Vector2i mouseStartDragPos; // position of the mouse in desktop coordonates when the drag starts
        bool isDragging = false; // is the window being dragged
        bool isDraggingFocus = false; // is the window started dragging when it gained focus (it is used to check for the mouse release which will not be sent to the window)
        int windowID; //FOR DEBUGGING


    public:
        sf::Vector2i startDragWindowPos; // position of the window when the drag starts
        sf::Vector2i mouseDelta; // The distance between the mouse position when the drag starts and the current mouse position

        sf::Vector2i newPos; // The new position of the window for next frame when dragging (it is stored here to be used to update window position after the updatePositionIfDrag function)


        MyWindow(const sf::VideoMode& videoMode, const std::string& title, sf::Uint32 style, const sf::ContextSettings& settings) : sf::RenderWindow(videoMode, title, style, settings)
        {
            setVerticalSyncEnabled(true);
            windowID = windowsCount++; //FOR DEBUGGING
        }

        // return true if started dragging
        bool pollEvents() {
            bool startedDragging = false;
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
                    startedDragging = true;
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
                    startedDragging = true;
                } else if (event.type == sf::Event::LostFocus) {
                    isDragging = false;
                    isDraggingFocus = false;
                }
            }
            if (isDraggingFocus && !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                isDragging = false;
                isDraggingFocus = false;
            }

            sf::Vector2i mousePos = sf::Mouse::getPosition();
            mouseDelta = (mousePos - mouseStartDragPos)/2;

            return startedDragging;
        }

        void updatePositionIfDrag(std::vector<MyWindow*> windows) {
            if (isDragging)
            {
                newPos = startDragWindowPos+mouseDelta;

                // if the new position overlaps with another window, put the window at the border of the other window
                for (MyWindow* window : windows)
                {
                    if (window == this) continue;
                    sf::Vector2i otherPos = window->getPosition();
                    sf::Vector2u otherSize = window->getSize();
                    if (newPos.x < otherPos.x+otherSize.x && newPos.x+getSize().x > otherPos.x && newPos.y < otherPos.y+otherSize.y && newPos.y+getSize().y > otherPos.y)
                    {
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

                    }
                }
            }
        }

        void draw(std::vector<MyDrawable*> drawables, float pixPerMeter) {
            for (MyDrawable* drawable : drawables)
            {
                drawable->draw(*this, pixPerMeter);
            }
        }

        bool getIsDragging() const {
            return isDragging;
        }

};

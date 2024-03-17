#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <Box2D/Box2D.h>

int windowsCount = 0; //FOR DEBUGGING


class MyWindowBase : public sf::RenderWindow
{
    protected:
        std::map<sf::Event::EventType, std::function<void(sf::Event)>> eventHandlers;
    public:
        MyWindowBase(const sf::VideoMode& videoMode, const std::string& title, sf::Uint32 style, const sf::ContextSettings& settings) : sf::RenderWindow(videoMode, title, style, settings)
        {
            setVerticalSyncEnabled(true);

            eventHandlers[sf::Event::Closed] = [this](sf::Event event) {
                close();
            };
        }

        void draw(std::vector<MyDrawable*> drawables, float pixPerMeter) {
            for (MyDrawable* drawable : drawables)
            {
                drawable->draw(*this, pixPerMeter);
            }
        }
        //TODO add close event

        void pollEvents() {
            for (auto event = sf::Event{}; sf::RenderWindow::pollEvent(event);)
            {
                if (eventHandlers.find(event.type) != eventHandlers.end()) eventHandlers[event.type](event);
            }
        }
};

class MyStaticWindow : public MyWindowBase
{
    public:
        sf::Vector2i winPos;

        MyStaticWindow(const sf::VideoMode& videoMode, const std::string& title, sf::Uint32 style, const sf::ContextSettings& settings, const sf::Vector2i winPos) : MyWindowBase(videoMode, title, style, settings), winPos(winPos)
        {
            setPosition(winPos);
        }

        ////////////////////////////////////////////////////////////
        /// \brief Reset the position of the window if it has been moved
        ///
        ////////////////////////////////////////////////////////////
        void resetPosition() {
            if (getPosition() != winPos) setPosition(winPos);
        }
};

class MyDraggableWindow : public MyWindowBase
{
    protected:
        sf::Vector2i mouseStartDragPos; // position of the mouse in desktop coordonates when the drag starts
        bool isDragging = false; // is the window being dragged
        bool isDraggingFocus = false; // if the window started dragging when it gained focus (it is used to check for the mouse release which will not be sent to the window)
        int windowID; //FOR DEBUGGING
        bool startedDragging = false;


    public:
        sf::Vector2i startDragWindowPos; // position of the window when the drag starts
        sf::Vector2i mouseDelta; // The distance between the mouse position when the drag starts and the current mouse position


        MyDraggableWindow(const sf::VideoMode& videoMode, const std::string& title, sf::Uint32 style, const sf::ContextSettings& settings) : MyWindowBase(videoMode, title, style, settings)
        {
            windowID = windowsCount++; //FOR DEBUGGING
            eventHandlers[sf::Event::MouseButtonPressed] = [this](sf::Event event) {
                if (event.mouseButton.button != sf::Mouse::Button::Left) return;
                startDragWindowPos = getPosition();
                mouseStartDragPos = sf::Mouse::getPosition();
                isDragging = true;
                isDraggingFocus = false;
                startedDragging = true;
            };
            eventHandlers[sf::Event::MouseButtonReleased] = [this](sf::Event event) {
                if (event.mouseButton.button != sf::Mouse::Button::Left) return;
                isDragging = false;
                isDraggingFocus = false;
            };
            eventHandlers[sf::Event::GainedFocus] = [this](sf::Event event) {
                // if we gained focus and the left mouse button is pressed it means we started dragging
                if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) return;
                startDragWindowPos = getPosition();
                mouseStartDragPos = sf::Mouse::getPosition();
                isDragging = true;
                isDraggingFocus = true;
                startedDragging = true;
            };
            eventHandlers[sf::Event::LostFocus] = [this](sf::Event event) {
                isDragging = false;
                isDraggingFocus = false;
            };
        }

        // return true if started dragging
        bool pollEvents() {
            startedDragging = false;
            for (auto event = sf::Event{}; sf::RenderWindow::pollEvent(event);)
            {
                if (eventHandlers.find(event.type) != eventHandlers.end()) eventHandlers[event.type](event);
            }

            if (isDraggingFocus && !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                isDragging = false;
                isDraggingFocus = false;
            }

            sf::Vector2i mousePos = sf::Mouse::getPosition();
            mouseDelta = (mousePos - mouseStartDragPos)/2;

            return startedDragging;
        }

        bool getIsDragging() const {
            return isDragging;
        }
};

class MyWindow : public MyDraggableWindow
{
    public:
        sf::Vector2i newPos; // The new position of the window for next frame when dragging (it is stored here to be used to update window position after the updatePositionIfDrag function)
        
        MyWindow(const sf::VideoMode& videoMode, const std::string& title, sf::Uint32 style, const sf::ContextSettings& settings) : MyDraggableWindow(videoMode, title, style, settings)
        {
        }

        void calculateNewPosDrag(std::vector<MyWindow*> windows, std::vector<MyStaticWindow*> staticWindows) {
            if (isDragging)
            {
                newPos = startDragWindowPos+mouseDelta;

                // if the new position overlaps with another window, put the window at the border of the other window
                for (int i=0; i<windows.size()+staticWindows.size(); i++)
                {
                    MyWindowBase* window;
                    if (i>=windows.size()) {
                        int j = i-windows.size();
                        window = (MyWindowBase*)staticWindows[j];
                    } else window = (MyWindowBase*)windows[i];

                    if (window == this) continue;

                    //! A window can be in the vector but be closed (it is not removed from the vector)
                    if (!window->isOpen()) continue;

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
};

class MyForceFieldWindow : public MyDraggableWindow
{
    private:
       sf::IntRect forceFieldRect; 
    public:
        float angle;

        MyForceFieldWindow(const sf::VideoMode& videoMode, float angle, const std::string& title, sf::Uint32 style, const sf::ContextSettings& settings) : MyDraggableWindow(videoMode, title, style, settings), angle(angle)
        {
            forceFieldRect = sf::IntRect(0, 0, std::max(videoMode.width, videoMode.height)*3,std::max(videoMode.width, videoMode.height)*3);

            eventHandlers[sf::Event::Resized] = [this](sf::Event event) {
                setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                forceFieldRect = sf::IntRect(0, 0, std::max(event.size.width, event.size.height)*3, std::max(event.size.width, event.size.height)*3);
            };
        }

        void draw(std::vector<MyDrawable*> drawables, float pixPerMeter, sf::Sprite forceFieldSprite) {
            //TODO test if having just one a force field sprite is a good idea
            forceFieldSprite.setTextureRect(forceFieldRect); // maybe dont do that each frame
            forceFieldSprite.setOrigin(forceFieldRect.width/2, forceFieldRect.height/2);
            forceFieldSprite.setPosition(getSize().x/2, getSize().y/2);
            forceFieldSprite.setRotation(angle);
            MyDraggableWindow::draw(drawables, pixPerMeter);
            sf::RenderWindow::draw(forceFieldSprite);
        }
};
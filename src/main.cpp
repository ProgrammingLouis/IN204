#include <SFML/Graphics.hpp>
#include <iostream>

// include Box2D
#include <Box2D/Box2D.h>
// #include "/usr/local/include/box2d/box2d.h"

#include <chrono>

// Local includes
#include "boxes.hpp"
#include "circles.hpp"
#include "window.hpp"
#include "finish.hpp"
#include "levels_data.hpp"
#include "levels_manager.hpp"
#include "virtual_method_classes.hpp"


#include <SFML/Window/Mouse.hpp>  // For desktop mouse position
#include <SFML/Window/Keyboard.hpp>  // For keyboard input

std::map<sf::Keyboard::Key, bool> keysPressed;

int main()
{
    //TODO Add cheat alert rules

    // Print SFML and Box2D version
    std::cout << "SFML version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << "." << SFML_VERSION_PATCH << std::endl;
    std::cout << "Box2D version: " << b2_version.major << "." << b2_version.minor << "." << b2_version.revision << std::endl;

    // Print desktop video modes
    auto desktopVideoMode = sf::VideoMode::getDesktopMode();
    //!! For some reason the desktop video mode is not the same as the screen resolution and it must be divided by 2
    desktopVideoMode.width /= 2;
    desktopVideoMode.height /= 2;
    std::cout << "Desktop video mode divided by 2 : " << desktopVideoMode.width << "x" << desktopVideoMode.height << "x" << desktopVideoMode.bitsPerPixel << std::endl;

    
    auto windowsSettings = sf::ContextSettings();
    windowsSettings.antialiasingLevel = 8;

    int currentLevel = -1;


    std::vector<MyWindow*> windows;
    
    //TODO fix the window drag from the title bar 

    /* #region Loading windows for level 1 */
    auto window = MyWindow(levelsData[0].videoModes[0], "MainWindow", sf::Style::Titlebar, windowsSettings);
    // mainWindow.setFramerateLimit(0);
    window.setVerticalSyncEnabled(true);
    window.setPosition(levelsData[0].windowPositions[0]);
    windows.push_back(&window);

    auto window1 = MyWindow(levelsData[0].videoModes[1], "Second window", sf::Style::Titlebar, windowsSettings);
    // secondWindow.setFramerateLimit(0);
    window1.setVerticalSyncEnabled(true);
    window1.setPosition(levelsData[0].windowPositions[1]);
    windows.push_back(&window1);
    /* #endregion */

    // std::vector<MyWindow*> windows = {&mainWindow, &secondWindow};

    // Load texture
    sf::Texture texture;
    if (!texture.loadFromFile("imgs/SFML.png"))
    {
        std::cerr << "Error loading texture 'imgs/SFML.png'" << std::endl;
        return 1;
    }
    texture.setSmooth(true);

    // Create a sprite
    auto sprite = sf::Sprite(texture);
    sprite.setTexture(texture);


    /* #region Box2D Init */
    b2Vec2 gravity(0.0f, 100.0f);
    b2World world(gravity);
    // world.SetAllowSleeping(true);
    // world.SetContinuousPhysics(true);
    //!! World size should be less than 2km (2000m) in any direction
    //!! Bodies size should be between 0.1m and 10m

    float pixPerMeter = desktopVideoMode.width / 500.0f;

    // Simulation parameters
    float timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 8;
    int32 positionIterations = 8;

    /* #endregion */
    
    std::vector<MyDrawable*> levelDrawables;
    std::vector<MyWindowStaticObject*> levelWindowStaticObjects;
    std::vector<MyDrawable*> allLevelDrawables;
    std::vector<MyDynamicObject*> levelDynamicObject; //TODO use this or delete it, and clear it in the levels_manager

    MyDynamicCircle* mainCircle = new MyDynamicCircle(sf::Vector2f(540, 300), 20, world, pixPerMeter);
    allLevelDrawables.push_back((MyDrawable*)mainCircle);
    levelDynamicObject.push_back((MyDynamicObject*)mainCircle);

    MyFinish* finish = new MyFinish(sf::Vector2f(0, 0), sf::Vector2f(40, 20), world, pixPerMeter, *(windows[0]));
    allLevelDrawables.push_back((MyDrawable*)finish);

    // Load level 1
    nextLevel(currentLevel, levelsData.size()-1, &levelDrawables, mainCircle, finish, &levelWindowStaticObjects, windows, windowsSettings, world, pixPerMeter);


    std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
    float deltaTime = 0.0f;

    std::cout << "Starting main loop" << std::endl;
    bool allWindowsClosed = false;
    while (!allWindowsClosed)
    {
        //!! Don't delete this
        // auto now = std::chrono::steady_clock::now();
        // deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - lastTime).count()*100.0f;
        // lastTime = now;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            mainCircle->body->ApplyLinearImpulseToCenter(b2Vec2(-200.0f, 0.0f), true);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            mainCircle->body->ApplyLinearImpulseToCenter(b2Vec2(200.0f, 0.0f), true);
        }

        //TODO maybe delete this cause it will close even if Escape is pressed on another app
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            for (auto& window : windows)
            {
                window->close();
            }
        }
        //TODO delete this for final game
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            if (!keysPressed[sf::Keyboard::Space])
            {
                nextLevel(currentLevel, levelsData.size()-1, &levelDrawables, mainCircle, finish, &levelWindowStaticObjects, windows, windowsSettings, world, pixPerMeter);
                keysPressed[sf::Keyboard::Space] = true;
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            if (!keysPressed[sf::Keyboard::R])
            {
                // Restart level
                loadLevel(currentLevel, &levelDrawables, mainCircle, finish, &levelWindowStaticObjects, windows, windowsSettings, world, pixPerMeter);
                keysPressed[sf::Keyboard::R] = true;
            }
        }

        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            keysPressed[sf::Keyboard::Space] = false;
        }
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            keysPressed[sf::Keyboard::R] = false;
        }

        // If circle is in the finish zone, go to the next level
        //TODO check if the finish box is right
        if (mainCircle->body->GetLinearVelocity().Length() < 0.1f && finish->isInside(mainCircle->body->GetPosition()))
        {
            //TODO uncomment this for final game
            // nextLevel(currentLevel, levelsData.size()-1, &levelDrawables, mainCircle, &levelWindowStaticObjects, windows, windowsSettings, world, pixPerMeter);
        }


        for (auto& winStaticObject : levelWindowStaticObjects)
        {
            winStaticObject->updatePositionIfDrag(pixPerMeter);
        }
        finish->updatePositionIfDrag(pixPerMeter);



        // // apply a force to mainCricle to offset gravity
        // mainCircle->body->ApplyForceToCenter(b2Vec2(0.0f, -100.0f*mainCircle->body->GetMass()), true);
        
        world.Step(timeStep, velocityIterations, positionIterations);
        
        /* #region Windows update */
        allWindowsClosed = true;
        int winID =0;
        for (auto& window : windows)
        {
            bool winStartedDragging = window->pollEvents();
            if (winStartedDragging) std::cout << "Window " << winID << " started dragging" << std::endl;
            
            window->updatePositionIfDrag(windows);

            //!!
            //TODO determine object inside and outside the dragging window

            //TODO mabe should put the body in sleep mode here
            if (winStartedDragging) {
                for (auto& dynamicObject : levelDynamicObject)
                {
                    //TODO MAYBE DO THIS EVERY FRAME
                    // if object is in the window
                    // if (dynamicObject->body->GetPosition().x*pixPerMeter > window->getPosition().x && dynamicObject->body->GetPosition().x*pixPerMeter < window->getPosition().x+window->getSize().x && dynamicObject->body->GetPosition().y*pixPerMeter > window->getPosition().y && dynamicObject->body->GetPosition().y*pixPerMeter < window->getPosition().y+window->getSize().y)
                    // {
                        dynamicObject->startDagPos = dynamicObject->body->GetPosition();
                    //     dynamicObject->windowID = winID;
                    // }
                }
            }

            if (window->getIsDragging()) {
                for (auto& dynamicObject : levelDynamicObject)
                {
                    // if (dynamicObject->windowID != winID) continue;
                    //!! if window moves too fast the object can be left behind because it's not in the window anymore
                    if (dynamicObject->body->GetPosition().x*pixPerMeter > window->getPosition().x && dynamicObject->body->GetPosition().x*pixPerMeter < window->getPosition().x+window->getSize().x && dynamicObject->body->GetPosition().y*pixPerMeter > window->getPosition().y && dynamicObject->body->GetPosition().y*pixPerMeter < window->getPosition().y+window->getSize().y)
                    {
                        b2Vec2 bodyPosWithoutDelta = dynamicObject->body->GetPosition();
                        sf::Vector2i winDelta = window->newPos - window->getPosition();
                        dynamicObject->body->SetTransform(b2Vec2(bodyPosWithoutDelta.x+winDelta.x/pixPerMeter, bodyPosWithoutDelta.y+winDelta.y/pixPerMeter), dynamicObject->body->GetAngle());
                    }
                }
            }

            if (window->getIsDragging()) {
                window->setPosition(window->newPos);
            }

            if (window->isOpen()) {
                window->clear();
                window->draw(levelDrawables, pixPerMeter);
                window->draw(allLevelDrawables, pixPerMeter);
                window->display();
                allWindowsClosed = false;
            }
            winID++;
        }

        /* #endregion */

    }

    // Free memory
    for (auto& window : windows)
    {
        delete window;
    }
    std::cout << "Cleared windows" << std::endl;

    for (auto& drawable : levelDrawables)
    {
        delete drawable;
    }
    std::cout << "Cleared levelDrawables" << std::endl;
    
    //TODO THis doesn't work is next levels, why ?
    for (auto& drawable : allLevelDrawables)
    {
        delete drawable;
    }
    std::cout << "Cleared allLevelDrawables" << std::endl;

    return 0;
}
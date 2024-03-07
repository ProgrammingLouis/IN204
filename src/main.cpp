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


#include <SFML/Window/Mouse.hpp>  // For desktop mouse position
#include <SFML/Window/Keyboard.hpp>  // For keyboard input

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

    // auto mainWindowVideoMode = desktopVideoMode;
    // mainWindowVideoMode.width /= 2;
    // mainWindowVideoMode.height /= 2;
    // std::cout << "Main window video mode: " << mainWindowVideoMode.width << "x" << mainWindowVideoMode.height << "x" << mainWindowVideoMode.bitsPerPixel << std::endl;
    
    auto windowsSettings = sf::ContextSettings();
    windowsSettings.antialiasingLevel = 8;

    int currentLevel = 0;

    // std::vector<MyWindow&> windows(level1::numberOfWindows);

    std::vector<MyWindow*> windows;

    // TODO allow for unlimited number of windows
    for (int windowID = 0; windowID < levelsData[0].numberOfWindows; windowID++)
    {
        //TODO fix the window drag from the title bar 
        auto thisWindow = new MyWindow(levelsData[0].videoModes[windowID], "Window", sf::Style::Titlebar, windowsSettings);
        thisWindow->setVerticalSyncEnabled(true);
        thisWindow->setPosition(levelsData[0].windowPositions[windowID]);
        windows.push_back(thisWindow);
        std::cout << "Window " << windowID << " created" << std::endl;
        // windows[windowID].create(level1::videoModes[windowID], "Window "+std::to_string(windowID), sf::Style::None, windowsSettings);
        // windows[windowID].setVerticalSyncEnabled(true);
        // windows[windowID].setPosition(level1::windowPositions[windowID]);
    }
    
    // std::vector<MyWindow*> windows;

    // std::cout << "Number of windows: " << windows.size() << std::endl;
    // for (auto& window : windows)
    // {
    //     window->create(mainWindowVideoMode, "Window", sf::Style::None, windowsSettings);
    // }
    // std::cout << "All windows polled" << std::endl;

    /* #region Loading windows for level 1 */
    // auto window = MyWindow(levelsData[0].videoModes[0], "MainWindow", sf::Style::Titlebar, windowsSettings);
    // // mainWindow.setFramerateLimit(0);
    // window.setVerticalSyncEnabled(true);
    // window.setPosition(levelsData[0].windowPositions[0]);
    // windows.push_back(&window);

    // auto window1 = MyWindow(levelsData[0].videoModes[1], "Second window", sf::Style::Titlebar, windowsSettings);
    // // secondWindow.setFramerateLimit(0);
    // window1.setVerticalSyncEnabled(true);
    // window1.setPosition(levelsData[0].windowPositions[1]);
    // windows.push_back(&window1);
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
    //!! World size should be less than 2km (2000m) in any direction
    //!! Bodies size should be between 0.1m and 10m

    float pixPerMeter = desktopVideoMode.width / 500.0f;

    // Simulation parameters
    float timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 8;
    int32 positionIterations = 8;

    /* #endregion */
    
    std::vector<MyDrawable*> drawables;

    MyDynamicBox* dynamicBox1 = new MyDynamicBox(sf::Vector2f(540, 400), sf::Vector2f(25, 25), world, pixPerMeter);
    drawables.push_back((MyDrawable*)dynamicBox1);

    MyStaticBox* staticBox1 = new MyStaticBox(sf::Vector2f(500, 500), sf::Vector2f(50, 20), world, pixPerMeter);
    drawables.push_back((MyDrawable*)staticBox1);

    MyDynamicCircle* mainCircle = new MyDynamicCircle(sf::Vector2f(540, 300), 20, world, pixPerMeter);
    drawables.push_back((MyDrawable*)mainCircle);


    MyWindowStaticBox* windowStaticBox1 = new MyWindowStaticBox(sf::Vector2f(100, 100), sf::Vector2f(50, 20), world, pixPerMeter, *(windows[1]));
    drawables.push_back((MyDrawable*)windowStaticBox1);

    MyFinish* finish = new MyFinish(sf::Vector2f(80, 100), sf::Vector2f(40, 20), world, pixPerMeter, *(windows[0]));
    drawables.push_back((MyDrawable*)finish);


    std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
    float deltaTime = 0.0f;

    // sf::Vector2i mousePos = sf::Mouse::getPosition();
    // sf::Vector2i mouseStartDragPos;
    // sf::Vector2i secondWindowPos = secondWindow.getPosition();


    while (windows[0]->isOpen() || windows[1]->isOpen())
    {
        //!! Don't delete this
        // auto now = std::chrono::steady_clock::now();
        // deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - lastTime).count()*100.0f;
        // lastTime = now;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            mainCircle->body->ApplyLinearImpulseToCenter(b2Vec2(-200.0f, 0.0f), true);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            mainCircle->body->ApplyLinearImpulseToCenter(b2Vec2(200.0f, 0.0f), true);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            windows[0]->close();
            windows[1]->close();
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            nextLevel(currentLevel, 1, &drawables, windows, windowsSettings, world, pixPerMeter);
        }

        //TODO Do these updates automatically
        if (currentLevel == 0) {
            windowStaticBox1->updatePosition(pixPerMeter);
            finish->updatePosition(pixPerMeter);
        }

        world.Step(timeStep, velocityIterations, positionIterations);

        /* #region Windows update */
        for (auto& window : windows)
        {
            window->pollEvents();
            window->updatePosition(windows);
            if (window->isOpen()) window->draw(drawables, pixPerMeter);
        }
        /* #endregion */

    }

    // Free memory
    for (auto& window : windows)
    {
        delete window;
    }

    for (auto& drawable : drawables)
    {
        delete drawable;
    }

    return 0;
}
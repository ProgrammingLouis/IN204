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

// For desktop mouse position
#include <SFML/Window/Mouse.hpp>


int main()
{
    // Print SFML version
    std::cout << "SFML version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << "." << SFML_VERSION_PATCH << std::endl;
    
    // Print desktop video modes
    auto desktopVideoMode = sf::VideoMode::getDesktopMode();
    //!! For some reason the desktop video mode is not the same as the screen resolution and it must be divided by 2
    desktopVideoMode.width /= 2;
    desktopVideoMode.height /= 2;
    std::cout << "Desktop video mode divicded by 2 : " << desktopVideoMode.width << "x" << desktopVideoMode.height << "x" << desktopVideoMode.bitsPerPixel << std::endl;

    auto mainWindowVideoMode = desktopVideoMode;
    mainWindowVideoMode.width /= 2;
    mainWindowVideoMode.height /= 2;
    std::cout << "Main window video mode: " << mainWindowVideoMode.width << "x" << mainWindowVideoMode.height << "x" << mainWindowVideoMode.bitsPerPixel << std::endl;
    
    auto windowsSettings = sf::ContextSettings();
    windowsSettings.antialiasingLevel = 8;

    int currentLevel = 1;

    // std::vector<MyWindow&> windows(level1::numberOfWindows);

    std::vector<MyWindow*> windows;


    //!! For some really strange I get a segmentation fault when I try to create the windows in a loop
    // Why ?

    // for (int windowID = 0; windowID < level1::numberOfWindows; windowID++)
    // {

    //     auto thisWindow = MyWindow(mainWindowVideoMode, "Window", sf::Style::None, windowsSettings);
    //     thisWindow.setVerticalSyncEnabled(true);
    //     if (windowID == 0) thisWindow.setPosition(sf::Vector2i(10, 0));
    //     else thisWindow.setPosition(sf::Vector2i(400, 300));
    //     // thisWindow.setPosition(sf::Vector2i(10, 0));
    //     windows.push_back(&thisWindow);
    //     std::cout << "Window " << windowID << " created" << std::endl;
    //     // windows[windowID].create(level1::videoModes[windowID], "Window "+std::to_string(windowID), sf::Style::None, windowsSettings);
    //     // windows[windowID].setVerticalSyncEnabled(true);
    //     // windows[windowID].setPosition(level1::windowPositions[windowID]);
    // }
    
    // std::vector<MyWindow*> windows;

    // std::cout << "Number of windows: " << windows.size() << std::endl;
    // for (auto& window : windows)
    // {
    //     window->create(mainWindowVideoMode, "Window", sf::Style::None, windowsSettings);
    // }
    // std::cout << "All windows polled" << std::endl;

    auto window = MyWindow(level1::videoModes[0], "MainWindow", sf::Style::None, windowsSettings);
    // mainWindow.setFramerateLimit(0);
    window.setVerticalSyncEnabled(true);
    window.setPosition(level1::windowPositions[0]);
    windows.push_back(&window);



    auto window1 = MyWindow(level1::videoModes[1], "Second window", sf::Style::None, windowsSettings);
    // secondWindow.setFramerateLimit(0);
    window1.setVerticalSyncEnabled(true);
    window1.setPosition(level1::windowPositions[1]);
    windows.push_back(&window1);

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

    MyDynamicBox dynamicBox1(sf::Vector2f(540, 400), sf::Vector2f(25, 25), world, pixPerMeter);
    drawables.push_back((MyDrawable*)(&dynamicBox1));

    MyStaticBox staticBox1(sf::Vector2f(500, 500), sf::Vector2f(50, 20), world, pixPerMeter);
    drawables.push_back((MyDrawable*)(&staticBox1));

    MyDynamicCircle circle2(sf::Vector2f(540, 300), 20, world, pixPerMeter);
    drawables.push_back((MyDrawable*)(&circle2));


    MyWindowStaticBox windowStaticBox1(sf::Vector2f(100, 100), sf::Vector2f(50, 20), world, pixPerMeter, *(windows[1]));
    drawables.push_back((MyDrawable*)(&windowStaticBox1));

    MyFinish finish(sf::Vector2f(80, 100), sf::Vector2f(40, 20), world, pixPerMeter, *(windows[0]));
    drawables.push_back((MyDrawable*)(&finish));


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
        

        //TODO Do these updates automatically
        windowStaticBox1.updatePosition(pixPerMeter);
        finish.updatePosition(pixPerMeter);

        world.Step(timeStep, velocityIterations, positionIterations);


        // mainWindow.pollEvents();
        // mainWindow.updatePosition();
        // mainWindow.draw(drawables, pixPerMeter);

        // secondWindow.pollEvents();
        // secondWindow.updatePosition();
        // secondWindow.draw(drawables, pixPerMeter);
        /* #region Windows update */
        for (auto& window : windows)
        {
            window->pollEvents();
            window->updatePosition(windows);
            if (window->isOpen()) window->draw(drawables, pixPerMeter);
        }
        /* #endregion */

    }

    delete &world;

    return 0;
}
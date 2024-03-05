#include <SFML/Graphics.hpp>
#include <iostream>

// include Box2D
#include <Box2D/Box2D.h>
// #include "/usr/local/include/box2d/box2d.h"

#include <chrono>
#include "screenObject.hpp"
#include "boxes.hpp"
#include "circles.hpp"
#include "window.hpp"

#include <SFML/Window/Mouse.hpp>


int main()
{
    // Print SFML version
    std::cout << "SFML version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << "." << SFML_VERSION_PATCH << std::endl;
    // Print desktop video modes
    auto desktopVideoMode = sf::VideoMode::getDesktopMode();
    std::cout << "Desktop video mode: " << desktopVideoMode.width << "x" << desktopVideoMode.height << "x" << desktopVideoMode.bitsPerPixel << std::endl;
    std::cout << "Dividing desktop video mode by 2" << std::endl;
    desktopVideoMode.width /= 2;
    desktopVideoMode.height /= 2;

    auto mainWindowVideoMode = desktopVideoMode;
    mainWindowVideoMode.width /= 2;
    mainWindowVideoMode.height /= 2;
    std::cout << "Main window video mode: " << mainWindowVideoMode.width << "x" << mainWindowVideoMode.height << "x" << mainWindowVideoMode.bitsPerPixel << std::endl;
    
    auto windowsSettings = sf::ContextSettings();
    windowsSettings.antialiasingLevel = 8;
    
    auto mainWindow = MyWindow(mainWindowVideoMode, "SFML works!", sf::Style::Default, windowsSettings);
    // mainWindow.setFramerateLimit(0);
    mainWindow.setVerticalSyncEnabled(true);
    mainWindow.setPosition(sf::Vector2i(10, 0));


    auto secondWindow = MyWindow(mainWindowVideoMode, "Second window", sf::Style::Default, windowsSettings);
    // secondWindow.setFramerateLimit(0);
    secondWindow.setVerticalSyncEnabled(true);
    secondWindow.setPosition(sf::Vector2i(400, 300));

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
    int32 positionIterations = 3;

    /* #endregion */
    
    // new box
    MyDynamicBox dynamicBox1(sf::Vector2f(540, 400), sf::Vector2f(25, 25), world, pixPerMeter);

    MyStaticBox staticBox1(sf::Vector2f(500, 500), sf::Vector2f(50, 20), world, pixPerMeter);

    MyDynamicCircle circle2(sf::Vector2f(540, 300), 20, world, pixPerMeter);

    MyWindowKinematicBox windowStaticBox1(sf::Vector2f(100, 100), sf::Vector2f(50, 20), world, pixPerMeter, secondWindow);



    std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
    float deltaTime = 0.0f;

    sf::Vector2i mousePos = sf::Mouse::getPosition();
    sf::Vector2i mouseStartDragPos;
    sf::Vector2i secondWindowPos = secondWindow.getPosition();


    while (mainWindow.isOpen() || secondWindow.isOpen())
    {
        auto now = std::chrono::steady_clock::now();
        deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - lastTime).count()*100.0f;
        lastTime = now;


        // SetTransform version
        windowStaticBox1.body->SetAwake(true);
        windowStaticBox1.body->SetTransform(b2Vec2((secondWindow.getPosition().x+windowStaticBox1.posOnAttachedWin.x)/pixPerMeter, (secondWindow.getPosition().y+windowStaticBox1.posOnAttachedWin.y)/pixPerMeter), 0);
        
        // // Applying forces towards new position version
        // windowStaticBox1.body->SetAwake(true);
        // windowStaticBox1.body->ApplyForceToCenter(b2Vec2(((secondWindow.getPosition().x+windowStaticBox1.winPosition.x)/pixPerMeter - windowStaticBox1.body->GetPosition().x)*1000.0f, ((secondWindow.getPosition().y+windowStaticBox1.winPosition.y)/pixPerMeter - windowStaticBox1.body->GetPosition().y)*1000.0f), true);

        
        
        // std::cout << "Setting simulation position: " << (secondWindow.getPosition().x+windowStaticBox1.winPosition.x)/pixPerMeter << ", " << (secondWindow.getPosition().y+windowStaticBox1.winPosition.y)/pixPerMeter << std::endl;

        world.Step(timeStep, velocityIterations, positionIterations);

        // std::cout << "Delta time: " << deltaTime << std::endl;

        // boxScreenObject.sreenPosition = sf::Vector2f(body->GetPosition().x*pixPerMeter-boxSize/2, body->GetPosition().y * pixPerMeter-boxSize/2);
        // boxScreenObject.shape.setRotation(body->GetAngle() * 180 / b2_pi);
        // std::cout << "Simulation position: " << body->GetPosition().x << ", " << body->GetPosition().y << std::endl;
        // std::cout << "Screen position: " << boxScreenObject.sreenPosition.x << ", " << boxScreenObject.sreenPosition.y << std::endl;

        // screenObject2.sreenPosition.x += 1 * deltaTime;

        // for (auto event = sf::Event{}; mainWindow.pollEvent(event);)
        // {
        //     if (event.type == sf::Event::Closed)
        //     {
        //         mainWindow.close();
        //     } else if (event.type == sf::Event::Resized) {
        //         mainWindow.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
        //     }
        // }

        // for (auto event = sf::Event{}; secondWindow.pollEvent(event);)
        // {
        //     if (event.type == sf::Event::Closed)
        //     {
        //         secondWindow.close();
        //     } else if (event.type == sf::Event::Resized) {
        //         secondWindow.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
        //     } else if (event.type == sf::Event::MouseButtonPressed) {
        //         if (event.mouseButton.button != sf::Mouse::Button::Left) continue;
        //         mouseStartDragPos = sf::Mouse::getPosition();
        //         // std::cout << "Mouse button pressed " << event.mouseButton.button << " at " << event.mouseButton.x << ", " << event.mouseButton.y << std::endl;
        //     } 
        // }

        /* #region Windows update */
        mainWindow.pollEvents();
        secondWindow.pollEvents();

        mainWindow.updatePosition();
        secondWindow.updatePosition();
        /* #endregion */

        if (mainWindow.isOpen())
        {
            mainWindow.clear();

            // boxScreenObject.draw(mainWindow);
            dynamicBox1.draw(mainWindow, pixPerMeter);
            staticBox1.draw(mainWindow, mainWindow.getPosition(), pixPerMeter);
            circle2.draw(mainWindow, pixPerMeter);

            windowStaticBox1.draw(mainWindow, pixPerMeter);

            mainWindow.display();
        }
        if (secondWindow.isOpen())
        {
            secondWindow.clear();

            // boxScreenObject.draw(secondWindow);
            dynamicBox1.draw(secondWindow, pixPerMeter);
            staticBox1.draw(secondWindow, secondWindow.getPosition(), pixPerMeter);
            circle2.draw(secondWindow, pixPerMeter);

            windowStaticBox1.draw(secondWindow, pixPerMeter);

            secondWindow.display();
        }

    }

    delete &world;

    return 0;
}
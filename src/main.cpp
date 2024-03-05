#include <SFML/Graphics.hpp>
#include <iostream>

// include Box2D
#include <Box2D/Box2D.h>
// #include "/usr/local/include/box2d/box2d.h"

#include <chrono>
#include "screenObject.hpp"
#include "boxes.hpp"

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
    
    
    auto mainWindow = sf::RenderWindow(mainWindowVideoMode, "SFML works!", sf::Style::Default, windowsSettings);
    // mainWindow.setFramerateLimit(144);
    mainWindow.setVerticalSyncEnabled(true);

    auto secondWindow = sf::RenderWindow(mainWindowVideoMode, "Second window", sf::Style::Default, windowsSettings);
    // secondWindow.setFramerateLimit(144);
    secondWindow.setVerticalSyncEnabled(true);

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


    // Create a green circle
    auto circle1 = sf::CircleShape(10);
    circle1.setFillColor(sf::Color::Green);
    auto screenObject1 = ScreenObject(sf::Vector2f(500, 500), circle1); 

    // Create a screen object
    auto circle = sf::CircleShape(50);
    circle.setFillColor(sf::Color::Red);
    auto screenObject2 = ScreenObject(sf::Vector2f(500, 200), circle);

    // Create a box screen object
    int boxSize = 50;
    auto box = sf::RectangleShape(sf::Vector2f(boxSize, boxSize));
    box.setFillColor(sf::Color::Blue);
    auto boxScreenObject = ScreenObject(sf::Vector2f(500, 200), box);

    // box.setOrigin(2*boxSize, 2*boxSize);


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

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(250.0f, 20.0f);

    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(100.0f, 10.0f);

    groundBody->CreateFixture(&groundBox, 0.0f);
    



    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(360.0f, desktopVideoMode.height/2/pixPerMeter);
    // bodyDef.angularVelocity = 4.0f;
    b2Body* body = world.CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    // dynamicBox.SetAsBox(boxSize/pixPerMeter, boxSize/pixPerMeter);
    dynamicBox.SetAsBox(5, 5);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 10.0f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);

    
    // new box
    MyDynamicBox box1(sf::Vector2f(550, 400), sf::Vector2f(25, 25), world, pixPerMeter);

    MyStaticBox staticBox1(sf::Vector2f(500, 500), sf::Vector2f(50, 10), world, pixPerMeter);



    std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
    float deltaTime = 0.0f;

    while (mainWindow.isOpen() || secondWindow.isOpen())
    {
        auto now = std::chrono::steady_clock::now();
        deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - lastTime).count()*100.0f;
        lastTime = now;

        world.Step(timeStep, velocityIterations, positionIterations);

        // std::cout << "Delta time: " << deltaTime << std::endl;

        boxScreenObject.sreenPosition = sf::Vector2f(body->GetPosition().x*pixPerMeter-boxSize/2, body->GetPosition().y * pixPerMeter-boxSize/2);
        boxScreenObject.shape.setRotation(body->GetAngle() * 180 / b2_pi);
        // std::cout << "Simulation position: " << body->GetPosition().x << ", " << body->GetPosition().y << std::endl;
        // std::cout << "Screen position: " << boxScreenObject.sreenPosition.x << ", " << boxScreenObject.sreenPosition.y << std::endl;

        screenObject2.sreenPosition.x += 1 * deltaTime;

        for (auto event = sf::Event{}; mainWindow.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                mainWindow.close();
            } else if (event.type == sf::Event::Resized) {
                mainWindow.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
            }
        }

        for (auto event = sf::Event{}; secondWindow.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                secondWindow.close();
            } else if (event.type == sf::Event::Resized) {
                secondWindow.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
            }
        }

        if (mainWindow.isOpen())
        {
            mainWindow.clear();
            

            screenObject1.draw(mainWindow);
            screenObject2.draw(mainWindow);
            // boxScreenObject.draw(mainWindow);
            box1.draw(mainWindow, pixPerMeter);
            staticBox1.draw(mainWindow, pixPerMeter);

            mainWindow.display();
        }
        if (secondWindow.isOpen())
        {
            secondWindow.clear();

            screenObject1.draw(secondWindow);
            screenObject2.draw(secondWindow);
            // boxScreenObject.draw(secondWindow);
            box1.draw(secondWindow, pixPerMeter);
            staticBox1.draw(secondWindow, pixPerMeter);


            secondWindow.display();
        }

    }

    delete &world;

    return 0;
}
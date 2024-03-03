#include <SFML/Graphics.hpp>
#include <iostream>

#include <chrono>
#include "screenObject.hpp"

int main()
{
    // Print SFML version
    std::cout << "SFML version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << "." << SFML_VERSION_PATCH << std::endl;
    // Print desktop video modes
    auto desktopVideoMode = sf::VideoMode::getDesktopMode();
    std::cout << "Desktop video mode: " << desktopVideoMode.width << "x" << desktopVideoMode.height << "x" << desktopVideoMode.bitsPerPixel << std::endl;
    auto mainWindowVideoMode = desktopVideoMode;
    mainWindowVideoMode.width /= 4;
    mainWindowVideoMode.height /= 4;
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
    auto circle = sf::CircleShape(100);
    circle.setFillColor(sf::Color::Green);
    auto screenObject1 = ScreenObject(sf::Vector2f(500, 500), circle); 

    // Create a screen object
    auto screenObject2 = ScreenObject(sf::Vector2f(500, 200), circle);



    // while (mainWindow.isOpen())
    // {
    //     for (auto event = sf::Event{}; mainWindow.pollEvent(event);)
    //     {
    //         if (event.type == sf::Event::Closed)
    //         {
    //             mainWindow.close();
    //         }
    //     }

    //     mainWindow.clear();
    //     mainWindow.display();
    // }

    std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
    float deltaTime = 0.0f;

    while (mainWindow.isOpen() || secondWindow.isOpen())
    {
        auto now = std::chrono::steady_clock::now();
        deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - lastTime).count()*100.0f;
        lastTime = now;

        // std::cout << "Delta time: " << deltaTime << std::endl;

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

            mainWindow.display();
        }
        if (secondWindow.isOpen())
        {
            secondWindow.clear();

            screenObject1.draw(secondWindow);
            screenObject2.draw(secondWindow);


            secondWindow.display();
        }

    }


    return 0;
}
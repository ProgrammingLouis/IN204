#include <SFML/Graphics.hpp>
#include <iostream>

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

    // Create a green circle
    auto circle = sf::CircleShape(100);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    circle.setFillColor(sf::Color::Green);
    int circleX = 500;
    int circleY = 500;



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

    while (mainWindow.isOpen() || secondWindow.isOpen())
    {
        for (auto event = sf::Event{}; mainWindow.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                mainWindow.close();
            }
        }

        for (auto event = sf::Event{}; secondWindow.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                secondWindow.close();
            }
        }

        if (mainWindow.isOpen())
        {
            mainWindow.clear();
            
            sf::Vector2i winPos = mainWindow.getPosition();
            circle.setPosition(circleX-winPos.x, circleY-winPos.y);

            mainWindow.draw(circle);

            mainWindow.display();
        }
        if (secondWindow.isOpen())
        {
            secondWindow.clear();

            sf::Vector2i winPos = secondWindow.getPosition();
            circle.setPosition(circleX-winPos.x, circleY-winPos.y);

            secondWindow.draw(circle);

            secondWindow.display();
        }
    }


    return 0;
}
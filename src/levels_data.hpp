#pragma once

#include <SFML/Graphics.hpp>

#include "virtual_method_classes.hpp"
#include "boxes.hpp"

// namespace level1 {
//     int numberOfWindows = 2;
//     std::vector<sf::VideoMode> videoModes = {sf::VideoMode(400, 300), sf::VideoMode(400, 300)};
//     std::vector<sf::Vector2i> windowPositions = {sf::Vector2i(100, 100), sf::Vector2i(800, 200)};
// }

// namespace level2 {
//     int numberOfWindows = 2;
//     std::vector<sf::VideoMode> videoModes = {sf::VideoMode(400, 800), sf::VideoMode(400, 300)};
//     std::vector<sf::Vector2i> windowPositions = {sf::Vector2i(100, 100), sf::Vector2i(800, 200)};
// }



enum MyDrawableType {
    WINDOW_STATIC_BOX,
    STATIC_BOX,
    DYNAMIC_BOX,
    DYNAMIC_CIRCLE,
    FINISH
};

struct MyDrawableData {
    MyDrawableType type;
    sf::Vector2f position;
    sf::Vector2f size;
    float density;
    float friction;
    float restitution;
    int windowID;
};

struct LevelData {
    int numberOfWindows;
    std::vector<sf::VideoMode> videoModes;
    std::vector<sf::Vector2i> windowPositions;
    std::vector<MyDrawableData> drawablesData;
    sf::Vector2i mainCirclePosition;
};

std::vector<LevelData> levelsData = {
    {
        2,
        {sf::VideoMode(400, 300), sf::VideoMode(400, 300)},
        {sf::Vector2i(100, 100), sf::Vector2i(800, 200)},
        {},
        {200, 150}
    },
    {
        3,
        {sf::VideoMode(400, 300), sf::VideoMode(400, 300), sf::VideoMode(400, 300)},
        {sf::Vector2i(100, 400), sf::Vector2i(800, 200), sf::Vector2i(800, 400)},
        {
            {WINDOW_STATIC_BOX, sf::Vector2f(50, 50), sf::Vector2f(50, 20), 0, 0, 0, 1},
            {FINISH, sf::Vector2f(350, 250), sf::Vector2f(50, 20), 0, 0, 0, 0}
        },
        {200, 150}
     }
};



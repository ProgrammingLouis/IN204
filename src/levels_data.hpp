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
    DYNAMIC_CIRCLE
    // FINISH
};

struct MyDrawableData {
    MyDrawableType type;
    sf::Vector2f position;
    sf::Vector2f size;
    float angle; // in degrees
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
    sf::Vector2i mainCirclePosition; // NOT USED if mainCirclePlacedOn != -1
    int mainCirclePlacedOn; // The id in the drawablesData vector of the object on which the main circle will be placed OR -1 if we should use the mainCirclePosition
};

std::vector<LevelData> levelsData = {
    // Level 0
    {
        2, // numberOfWindows

        // videoModes
        {sf::VideoMode(400, 300), sf::VideoMode(400, 300)},

        // windowPositions
        {sf::Vector2i(100, 100), sf::Vector2i(800, 200)},

        // drawablesData
        {},

        {200, 150}, // mainCirclePosition
        -1 // mainCirclePlacedOn
    },

    // Level 1
    {
        3, // numberOfWindows

        // videoModes
        {sf::VideoMode(400, 300), sf::VideoMode(400, 300), sf::VideoMode(400, 300)},

        // windowPositions
        {sf::Vector2i(100, 400), sf::Vector2i(800, 200), sf::Vector2i(800, 400)},

        // drawablesData
        {
            {WINDOW_STATIC_BOX, sf::Vector2f(200, 150), sf::Vector2f(50, 20), 0, 0, 0, 0, 1},
            {WINDOW_STATIC_BOX, sf::Vector2f(250, 150), sf::Vector2f(50, 20), 20, 0, 0, 0, 1},
            // {FINISH, sf::Vector2f(200, 150), sf::Vector2f(50, 20), 0, 0, 0, 0}
        },
        
        {0, 0}, // mainCirclePosition
        0 // mainCirclePlacedOn
     }
};



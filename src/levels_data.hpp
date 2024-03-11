#pragma once

#include <SFML/Graphics.hpp>

#include "virtual_method_classes.hpp"
#include "boxes.hpp"

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
    b2Vec2 finishWinPosition; // finish position in the window space
    int finishWinID; // finish window id
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
        {
            // window static box at the middle of window 0
            {WINDOW_STATIC_BOX, sf::Vector2f(200, 200), sf::Vector2f(40, 10), 0, 0, 0, 0, 0},
            {WINDOW_STATIC_BOX, sf::Vector2f(290, 210), sf::Vector2f(60, 10), 10, 0, 0, 0, 0},
            {WINDOW_STATIC_BOX, sf::Vector2f(170, 150), sf::Vector2f(40, 10), 0, 0, 0, 0, 1},
            {WINDOW_STATIC_BOX, sf::Vector2f(260, 160), sf::Vector2f(60, 10), 10, 0, 0, 0, 1}
        },

        {0, 0}, // mainCirclePosition
        0, // mainCirclePlacedOn
        {80, 100}, // finishWinPosition
        0 // finishWinID
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
            {WINDOW_STATIC_BOX, sf::Vector2f(200, 140), sf::Vector2f(60, 10), 0, 0, 0, 0, 1},
            {WINDOW_STATIC_BOX, sf::Vector2f(295, 115), sf::Vector2f(50, 8), -35, 0, 0, 0, 1},
            {WINDOW_STATIC_BOX, sf::Vector2f(105, 115), sf::Vector2f(50, 8), 35, 0, 0, 0, 1},
            // {FINISH, sf::Vector2f(200, 150), sf::Vector2f(50, 20), 0, 0, 0, 0}
        },
        
        {0, 0}, // mainCirclePosition
        0, // mainCirclePlacedOn
        {200, 150}, // finishWinPosition
        0 // finishWinID
     }
};



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

    int numberOfStaticWindows;
    std::vector<sf::VideoMode> staticVideoModes;
    std::vector<sf::Vector2i> staticWindowPositions;

    int numberOfForceFieldWindows;
    std::vector<sf::VideoMode> forceFieldVideoModes;
    std::vector<sf::Vector2i> forceFieldWindowPositions;

    std::vector<MyDrawableData> drawablesData;
    sf::Vector2i mainCirclePosition; // NOT USED if mainCirclePlacedOn != -1
    int mainCirclePlacedOn; // The id in the drawablesData vector of the object on which the main circle will be placed OR -1 if we should use the mainCirclePosition
    // b2Vec2 finishWinPosition; // finish position in the window space
    // int finishWinID; // finish window id

    //TODO add finish half size
    int numberOfFinish;
    std::vector<sf::Vector2f> finishWinPosition;
    std::vector<int> finishWinID;

    int numberOfFinishStatic;
    std::vector<sf::Vector2f> finishStaticWinPosition;
    std::vector<int> finishStaticWinID;
};

std::vector<LevelData> levelsData = {
    // Level 0
    {
        2, // numberOfWindows

        // videoModes
        {sf::VideoMode(400, 300), sf::VideoMode(400, 300)},

        // windowPositions
        {sf::Vector2i(100, 100), sf::Vector2i(800, 200)},

        0, // numberOfStaticWindows
        {}, // staticVideoModes
        {}, // staticWindowPositions

        0, // numberOfForceFieldWindows
        {}, // forceFieldVideoModes
        {}, // forceFieldWindowPositions

        // drawablesData
        {
            // drawablesData format :
            // {type, position, size, angle, density, friction, restitution, windowID}
            //
            // window static box at the middle of window 0
            {WINDOW_STATIC_BOX, sf::Vector2f(200, 200), sf::Vector2f(40, 10), 0, 0, 0, 0, 0},
            {WINDOW_STATIC_BOX, sf::Vector2f(290, 210), sf::Vector2f(60, 10), 10, 0, 0, 0, 0},
            {WINDOW_STATIC_BOX, sf::Vector2f(170, 150), sf::Vector2f(40, 10), 0, 0, 0, 0, 1},
            {WINDOW_STATIC_BOX, sf::Vector2f(260, 160), sf::Vector2f(60, 10), 10, 0, 0, 0, 1}
        },

        {0, 0}, // mainCirclePosition
        0, // mainCirclePlacedOn
        // {80, 100}, // finishWinPosition
        // 0 // finishWinID
        1, // numberOfFinish
        {sf::Vector2f(80.0, 100.0)}, // finishWinPosition
        {0}, // finishWinID

        0, // numberOfFinishStatic
        {}, // finishStaticWinPosition
        {} // finishStaticWinID
    },

    // Level 1
    {
        3, // numberOfWindows
        // 2, // numberOfWindows

        // videoModes
        {sf::VideoMode(400, 300), sf::VideoMode(400, 300), sf::VideoMode(400, 300)},
        // {sf::VideoMode(400, 300), sf::VideoMode(400, 300)},

        // windowPositions
        {sf::Vector2i(100, 400), sf::Vector2i(800, 200), sf::Vector2i(800, 400)},
        // {sf::Vector2i(100, 400), sf::Vector2i(800, 200)},

        0, // numberOfStaticWindows
        {}, // staticVideoModes
        {}, // staticWindowPositions

        0, // numberOfForceFieldWindows
        {}, // forceFieldVideoModes
        {}, // forceFieldWindowPositions

        // drawablesData
        // drawablesData format :
        // {type, position, size, angle, density, friction, restitution, windowID}
        {
            {WINDOW_STATIC_BOX, sf::Vector2f(200, 140), sf::Vector2f(60, 10), 0, 0, 0, 0, 1},
            {WINDOW_STATIC_BOX, sf::Vector2f(295, 115), sf::Vector2f(50, 8), -35, 0, 0, 0, 1},
            {WINDOW_STATIC_BOX, sf::Vector2f(105, 115), sf::Vector2f(50, 8), 35, 0, 0, 0, 1}
        },
        
        {0, 0}, // mainCirclePosition
        0, // mainCirclePlacedOn
        // {200, 150}, // finishWinPosition
        // 0 // finishWinID

        1, // numberOfFinish
        {sf::Vector2f(200.0, 150.0)}, // finishWinPosition
        {0}, // finishWinID

        0, // numberOfFinishStatic
        {}, // finishStaticWinPosition
        {} // finishStaticWinID
     },

    // Level 2
    {
        3, // numberOfWindows
        // 2, // numberOfWindows

        // videoModes
        {sf::VideoMode(400, 300), sf::VideoMode(400, 300)},
        // {sf::VideoMode(400, 300), sf::VideoMode(400, 300)},

        // windowPositions
        {sf::Vector2i(100, 300), sf::Vector2i(800, 100)},
        // {sf::Vector2i(100, 400), sf::Vector2i(800, 200)},

        0, // numberOfStaticWindows
        {}, // staticVideoModes
        {}, // staticWindowPositions

        1, // numberOfForceFieldWindows
        {sf::VideoMode(200, 150)}, // forceFieldVideoModes
        {sf::Vector2i(1680-200-50, 1050-150-50)}, // forceFieldWindowPositions

        // drawablesData
        // drawablesData format :
        // {type, position, size, angle, density, friction, restitution, windowID}
        {
            // {WINDOW_STATIC_BOX, sf::Vector2f(200, 140), sf::Vector2f(60, 10), 0, 0, 0, 0, 0},
            {WINDOW_STATIC_BOX, sf::Vector2f(70, 115), sf::Vector2f(40, 8), 0, 0, 0, 0, 0},
            {WINDOW_STATIC_BOX, sf::Vector2f(330, 115), sf::Vector2f(40, 8), 0, 0, 0, 0, 0},
            {WINDOW_STATIC_BOX, sf::Vector2f(10, 240), sf::Vector2f(80, 10), 90, 0, 0, 0, 0},
            {WINDOW_STATIC_BOX, sf::Vector2f(390, 240), sf::Vector2f(80, 10), 90, 0, 0, 0, 0},
            {WINDOW_STATIC_BOX, sf::Vector2f(290, 115), sf::Vector2f(108, 8), 30, 0, 0, 0, 0},
            {WINDOW_STATIC_BOX, sf::Vector2f(110, 115), sf::Vector2f(108, 8), -30, 0, 0, 0, 0},

            {WINDOW_STATIC_BOX, sf::Vector2f(200, 140), sf::Vector2f(60, 10), 0, 0, 0, 0, 1},
            {WINDOW_STATIC_BOX, sf::Vector2f(105, 170), sf::Vector2f(50, 8), -35, 0, 0, 0, 1},
            {WINDOW_STATIC_BOX, sf::Vector2f(295, 170), sf::Vector2f(50, 8), 35, 0, 0, 0, 1}
        },
        
        {0, 0}, // mainCirclePosition
        0, // mainCirclePlacedOn
        // {200, 150}, // finishWinPosition
        // 0 // finishWinID

        1, // numberOfFinish
        {sf::Vector2f(200.0, 200.0)}, // finishWinPosition
        {0}, // finishWinID

        0, // numberOfFinishStatic
        {}, // finishStaticWinPosition
        {} // finishStaticWinID
     },
    
    // Level 3
    // Two static windows add one force field window

    {
        0, // numberOfWindows

        // videoModes
        {},

        // windowPositions
        {},

        2, // numberOfStaticWindows
        {sf::VideoMode(300, 250), sf::VideoMode(300, 250)}, // staticVideoModes
        {sf::Vector2i(50, 400), sf::Vector2i(1680-300-50, 400)}, // staticWindowPositions

        1, // numberOfForceFieldWindows
        {sf::VideoMode(200, 150)}, // forceFieldVideoModes
        {sf::Vector2i(1680-200-50, 1050-150-50)}, // forceFieldWindowPositions

        // drawablesData
        // drawablesData format :
        // {type, position, size, angle, density, friction, restitution, windowID}
        {
            // WINDOW_STATIC_BOX in the middle of window 0
            {WINDOW_STATIC_BOX, sf::Vector2f(150, 150), sf::Vector2f(60, 10), 0, 0, 0, 0, 0},
            // WINDOW_STATIC_BOX in the middle of window 1
            {WINDOW_STATIC_BOX, sf::Vector2f(150, 150), sf::Vector2f(60, 10), 0, 0, 0, 0, 1}
        },
        
        {0, 0}, // mainCirclePosition
        0, // mainCirclePlacedOn
        // {200, 150}, // finishWinPosition
        // 0 // finishWinID

        0, // numberOfFinish
        {}, // finishWinPosition
        {}, // finishWinID

        1, // numberOfFinishStatic
        {sf::Vector2f(250, 160)}, // finishStaticWinPosition
        {1} // finishStaticWinID
    },


    // Level 4
    {
        1, // numberOfWindows

        // videoModes
        {sf::VideoMode(300, 200)},

        // windowPositions
        {sf::Vector2i(100, 400)},

        2, // numberOfStaticWindows
        {sf::VideoMode(200, 150), sf::VideoMode(400, 300)},
        {sf::Vector2i(50, 50), sf::Vector2i(1100, 700)},

        0, // numberOfForceFieldWindows
        {}, // forceFieldVideoModes
        {}, // forceFieldWindowPositions

        // drawablesData
        // drawablesData format :
        // {type, position, size, angle, density, friction, restitution, windowID}
        {
            {WINDOW_STATIC_BOX, sf::Vector2f(150, 150), sf::Vector2f(60, 10), 0, 0, 0, 0, 1},
            // {WINDOW_STATIC_BOX, sf::Vector2f(295, 115), sf::Vector2f(50, 8), -35, 0, 0, 0, 1},
            // {WINDOW_STATIC_BOX, sf::Vector2f(105, 115), sf::Vector2f(50, 8), 35, 0, 0, 0, 1}
        },
        
        {0, 0}, // mainCirclePosition
        0, // mainCirclePlacedOn
        // {200, 150}, // finishWinPosition
        // 0 // finishWinID

        0, // numberOfFinish
        {}, // finishWinPosition
        {0}, // finishWinID

        3, // numberOfFinishStatic
        {sf::Vector2f(100.0, 150.0), sf::Vector2f(200.0, 150.0), sf::Vector2f(300.0, 150.0)}, // finishStaticWinPosition
        {1, 1, 1} // finishStaticWinID
     }

//     // New level 3
//     {
//         3, // numberOfWindows

//         // videoModes
//         {sf::VideoMode(400, 300), sf::VideoMode(400, 300), sf::VideoMode(400, 300)},

//         // windowPositions
//         {sf::Vector2i(100, 400), sf::Vector2i(800, 200), sf::Vector2i(800, 400)},

//         0, // numberOfStaticWindows
//         // {sf::VideoMode(200, 150), sf::VideoMode(400, 300)},
//         // {sf::Vector2i(50, 50), sf::Vector2i(600, 700)},
//         {}, // staticVideoModes
//         {}, // staticWindowPositions

//         0, // numberOfForceFieldWindows
//         {}, // forceFieldVideoModes
//         {}, // forceFieldWindowPositions

//         // drawablesData 
//         {
//             {WINDOW_STATIC_BOX, sf::Vector2f(200, 200), sf::Vector2f(60, 10), 0, 0, 0, 0, 1},
//             // {WINDOW_STATIC_BOX, sf::Vector2f(295, 115), sf::Vector2f(50, 8), -35, 0, 0, 0, 1},
//             // {WINDOW_STATIC_BOX, sf::Vector2f(105, 115), sf::Vector2f(50, 8), 35, 0, 0, 0, 1}
//         },
        
//         {0, 0}, // mainCirclePosition
//         0, // mainCirclePlacedOn
//         {200, 150}, // finishWinPosition
//         0 // finishWinID
//      }
};



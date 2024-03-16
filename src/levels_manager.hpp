#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>

#include "window.hpp"
#include "levels_data.hpp"
#include "virtual_method_classes.hpp"
#include "finish.hpp"
#include "constants.h"


struct windows {
    std::vector<MyWindow*>& windows;
    std::vector<MyStaticWindow*>& staticWindows;
    std::vector<MyForceFieldWindow*>& forceFieldWindows;
    sf::ContextSettings windowsSettings;
};

struct gameObjects {
    std::vector<MyDrawable*> *drawables;
    MyDynamicCircle* mainCircle;
    MyFinish<MyWindow>* finish;
    std::vector<MyWindowStaticObject*> *windowStaticObjects;
};


void loadLevel(int levelID, struct gameObjects* gameObjects_struct, struct windows* windows_struct, b2World& world, float pixPerMeter)
{
    std::cout << "Loading level " << levelID << "..." << std::endl;
    LevelData levelData = levelsData[levelID];

    // We just need to clear the windowStaticObjects vector because it only contains pointers to drawables which will be deleted in the drawables vector
    gameObjects_struct->windowStaticObjects->clear();

    //TODO do not close the windows that would be reused for the next level
    // Clear the windows
    for (auto window : windows_struct->windows)
    {
        window->close();
    }
    for (auto window : windows_struct->staticWindows)
    {
        window->close();
    }
    for (auto window : windows_struct->forceFieldWindows)
    {
        window->close();
    }
    std::cout << "windows closed" << std::endl;

    /* #region Clear the drawables */

    // Clear the drawables bodies
    for (auto drawable : *gameObjects_struct->drawables)
    {
        if (drawable == (MyDrawable*)gameObjects_struct->mainCircle)
            continue;
        drawable->destroyBody(world);
    }
    std::cout << "Bodies destroyed" << std::endl;

    // Clear the drawables
    for (auto drawable : *gameObjects_struct->drawables)
    {
        if (drawable == (MyDrawable*)gameObjects_struct->mainCircle)
            continue;
        delete drawable;
    }

    gameObjects_struct->drawables->clear();
    gameObjects_struct->drawables->push_back((MyDrawable*)gameObjects_struct->mainCircle);

    /* #endregion */

    // index of the last window to be created + 1
    int windowsToBeCreated = levelData.numberOfWindows;

    /* #region Load windows */

    // Create new windows
    if (levelData.numberOfWindows > windows_struct->windows.size())
    {
        windowsToBeCreated = windows_struct->windows.size();
        for (int windowID = windows_struct->windows.size(); windowID < levelData.numberOfWindows; windowID++)
        {
            auto thisWindow = new MyWindow(levelData.videoModes[windowID], "Window", sf::Style::Titlebar, windows_struct->windowsSettings);
            thisWindow->setPosition(levelData.windowPositions[windowID]);
            windows_struct->windows.push_back(thisWindow);
            std::cout << "Create new window object for window " << windowID << std::endl;
        }
    }

    // Load the level windows
    for (int windowID = 0; windowID < windowsToBeCreated; windowID++)
    {
        std::cout << "Creating window " << windowID << " from existing object" << std::endl;
        windows_struct->windows[windowID]->create(levelData.videoModes[windowID], "Window " + std::to_string(windowID), sf::Style::Titlebar, windows_struct->windowsSettings);
        //!! When we call create on a window we need to reset vertical sync enabled
        windows_struct->windows[windowID]->setVerticalSyncEnabled(true);
        windows_struct->windows[windowID]->setPosition(levelData.windowPositions[windowID]);
    }

    // Create new static windows
    int staticWindowsToBeCreated = levelData.numberOfStaticWindows;

    if (levelData.numberOfStaticWindows > windows_struct->staticWindows.size())
    {
        staticWindowsToBeCreated = windows_struct->staticWindows.size();
        for (int windowID = windows_struct->staticWindows.size(); windowID < levelData.numberOfStaticWindows; windowID++)
        {
            auto thisWindow = new MyStaticWindow(levelData.staticVideoModes[windowID], "Static Window", sf::Style::Titlebar, windows_struct->windowsSettings, levelData.staticWindowPositions[windowID]);
            windows_struct->staticWindows.push_back(thisWindow);
            std::cout << "Create new static window object for window " << windowID << std::endl;
        }
    }

    // Load the level static windows
    for (int windowID = 0; windowID < staticWindowsToBeCreated; windowID++)
    {
        std::cout << "Creating static window " << windowID << " from existing object" << std::endl;
        windows_struct->staticWindows[windowID]->create(levelData.staticVideoModes[windowID], "Static Window " + std::to_string(windowID), sf::Style::Titlebar, windows_struct->windowsSettings);
        windows_struct->staticWindows[windowID]->setVerticalSyncEnabled(true);
        windows_struct->staticWindows[windowID]->setPosition(levelData.staticWindowPositions[windowID]);
    }

    // Create new force field windows
    int forceFieldWindowsToBeCreated = levelData.numberOfForceFieldWindows;

    if (levelData.numberOfForceFieldWindows > windows_struct->forceFieldWindows.size())
    {
        forceFieldWindowsToBeCreated = windows_struct->forceFieldWindows.size();
        for (int windowID = windows_struct->forceFieldWindows.size(); windowID < levelData.numberOfForceFieldWindows; windowID++)
        {
            auto thisWindow = new MyForceFieldWindow(levelData.forceFieldVideoModes[windowID], "Force Field Window", sf::Style::Titlebar, windows_struct->windowsSettings);
            windows_struct->forceFieldWindows.push_back(thisWindow);
            std::cout << "Create new force field window object for window " << windowID << std::endl;
        }
    }

    // Load the level force field windows
    for (int windowID = 0; windowID < forceFieldWindowsToBeCreated; windowID++)
    {
        std::cout << "Creating force field window " << windowID << " from existing object" << std::endl;
        windows_struct->forceFieldWindows[windowID]->create(levelData.forceFieldVideoModes[windowID], "Force Field Window " + std::to_string(windowID), sf::Style::Titlebar, windows_struct->windowsSettings);
        windows_struct->forceFieldWindows[windowID]->setVerticalSyncEnabled(true);
        windows_struct->forceFieldWindows[windowID]->setPosition(levelData.forceFieldWindowPositions[windowID]);
    }

    /* #endregion */

    /* #region main circle reset */
    //Update the main circle position
    if (levelData.mainCirclePlacedOn != -1)
    {
        sf::Vector2i winOffset = {0, 0};
        MyDrawableData placedOnDrawableData = levelData.drawablesData[levelData.mainCirclePlacedOn];
        // If main circle is placed on a finish or a window static object we need to add an offset to the position
        if (placedOnDrawableData.type == WINDOW_STATIC_BOX)
        {
            winOffset = windows_struct->windows[placedOnDrawableData.windowID]->getPosition();
        }
        gameObjects_struct->mainCircle->body->SetTransform(b2Vec2((placedOnDrawableData.position.x+winOffset.x)/pixPerMeter, (placedOnDrawableData.position.y+winOffset.y-MAIN_CIRCLE_RADIUS)/pixPerMeter), 0);
    }
    else
    {
        gameObjects_struct->mainCircle->body->SetTransform(b2Vec2(levelData.mainCirclePosition.x/pixPerMeter, levelData.mainCirclePosition.y/pixPerMeter), 0);
    }

    gameObjects_struct->mainCircle->body->SetLinearVelocity(b2Vec2(0, 0));
    gameObjects_struct->mainCircle->body->SetAngularVelocity(0);
    /* #endregion */

    /* #region set new finish position */
    gameObjects_struct->finish->setBodyPosition(levelData.finishWinPosition, pixPerMeter);
    /* #endregion */

    /* #region Create new drawables */ 
    for (int drawableID = 0; drawableID < levelData.drawablesData.size(); drawableID++)
    {
        MyDrawableData drawableData = levelData.drawablesData[drawableID];
        if (drawableData.type == WINDOW_STATIC_BOX)
        {
            if (drawableData.windowID < windows_struct->windows.size())
            {
                auto winStaticBox = new MyWindowStaticBox<MyWindow>(drawableData.position, drawableData.size, drawableData.angle, world, pixPerMeter, *windows_struct->windows[drawableData.windowID]);
                gameObjects_struct->drawables->push_back((MyDrawable*)winStaticBox);
                gameObjects_struct->windowStaticObjects->push_back((MyWindowStaticObject*)winStaticBox);
                continue;
            }

            if (drawableData.windowID < windows_struct->windows.size() + windows_struct->staticWindows.size())
            {
                auto winStaticBox = new MyWindowStaticBox<MyStaticWindow>(drawableData.position, drawableData.size, drawableData.angle, world, pixPerMeter, *windows_struct->staticWindows[drawableData.windowID - windows_struct->windows.size()]);
                gameObjects_struct->drawables->push_back((MyDrawable*)winStaticBox);
                //! We do not add the window static object to the windowStaticObjects vector because we do not want to update the position of the static windows
                continue;
            }

            std::cerr << "Unknown window id" << std::endl;
            break;
        }
        else if (drawableData.type == STATIC_BOX)
        {
            auto staticBox = new MyStaticBox(drawableData.position, drawableData.size, world, pixPerMeter);
            gameObjects_struct->drawables->push_back((MyDrawable*)staticBox);
        }
        else if (drawableData.type == DYNAMIC_BOX)
        {
            auto dynamicBox = new MyDynamicBox(drawableData.position, drawableData.size, world, pixPerMeter);
            gameObjects_struct->drawables->push_back((MyDrawable*)dynamicBox);
        }
        else if (drawableData.type == DYNAMIC_CIRCLE)
        {
            auto dynamicCircle = new MyDynamicCircle(drawableData.position, drawableData.size.x, world, pixPerMeter);
            gameObjects_struct->drawables->push_back((MyDrawable*)dynamicCircle);
        }
        else
        {
            std::cerr << "Unknown drawable type" << std::endl;
            break;
        }
    }
    /* #endregion */

    std::cout << "Level loaded !!" << std::endl;
}

void nextLevel(int& currentLevel, int maxLevel, struct gameObjects* gameObjects_struct, struct windows* windows_struct, b2World& world, float pixPerMeter)
{
    std::cout << "Loading next level..." << std::endl;
    currentLevel++;
    if (currentLevel > maxLevel)
    {
        std::cout << "No more levels" << std::endl;
        //TODO Add a Finish screen
        return;
    }
    loadLevel(currentLevel, gameObjects_struct, windows_struct, world, pixPerMeter);

}
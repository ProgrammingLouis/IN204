#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>

#include "window.hpp"
#include "levels_data.hpp"
#include "virtual_method_classes.hpp"
#include "finish.hpp"


void loadLevel(int levelID, std::vector<MyDrawable*> *drawables, std::vector<MyWindowStaticObject*> *windowStaticObjects, std::vector<MyWindow*>& windows, sf::ContextSettings windowsSettings, b2World& world, float pixPerMeter)
{
    LevelData levelData = levelsData[levelID];

    //TODO do not close the windows that would be reused for the next level
    // Clear the windows
    for (auto window : windows)
    {
        window->close();
    }
    std::cout << "windows closed" << std::endl;

    // Clear the drawables
    for (auto drawable : *drawables)
    {
        drawable->destroyBody(world);
    }
    std::cout << "Bodies destroyed" << std::endl;

    // Clear the drawables
    for (auto drawable : *drawables)
    {
        delete drawable;
    }

    drawables->clear();

    // index of the last window to be created + 1
    int windowsToBeCreated = levelData.numberOfWindows;

    // Create new windows
    if (levelData.numberOfWindows > windows.size())
    {
        windowsToBeCreated = windows.size();
        for (int windowID = windows.size(); windowID < levelData.numberOfWindows; windowID++)
        {
            auto thisWindow = new MyWindow(levelData.videoModes[windowID], "Window", sf::Style::Titlebar, windowsSettings);
            thisWindow->setVerticalSyncEnabled(true);
            thisWindow->setPosition(levelData.windowPositions[windowID]);
            windows.push_back(thisWindow);
            std::cout << "Create new window object for window " << windowID << std::endl;
        }
    }

    // Load the level
    for (int windowID = 0; windowID < windowsToBeCreated; windowID++)
    {
        std::cout << "Creating window " << windowID << " from existing object" << std::endl;
        windows[windowID]->create(levelData.videoModes[windowID], "Window " + std::to_string(windowID), sf::Style::Titlebar, windowsSettings);
        windows[windowID]->setVerticalSyncEnabled(true);
        windows[windowID]->setPosition(levelData.windowPositions[windowID]);
    }


    // Create new drawables
    for (int drawableID = 0; drawableID < levelData.drawablesData.size(); drawableID++)
    {
        MyDrawableData drawableData = levelData.drawablesData[drawableID];
        if (drawableData.type == WINDOW_STATIC_BOX)
        {
            auto winStaticBox = new MyWindowStaticBox(drawableData.position, drawableData.size, world, pixPerMeter, *windows[drawableData.windowID]);
            drawables->push_back((MyDrawable*)winStaticBox);
            windowStaticObjects->push_back((MyWindowStaticObject*)winStaticBox);
        }
        else if (drawableData.type == STATIC_BOX)
        {
            auto staticBox = new MyStaticBox(drawableData.position, drawableData.size, world, pixPerMeter);
            drawables->push_back((MyDrawable*)staticBox);
        }
        else if (drawableData.type == DYNAMIC_BOX)
        {
            auto dynamicBox = new MyDynamicBox(drawableData.position, drawableData.size, world, pixPerMeter);
            drawables->push_back((MyDrawable*)dynamicBox);
        }
        else if (drawableData.type == DYNAMIC_CIRCLE)
        {
            auto dynamicCircle = new MyDynamicCircle(drawableData.position, drawableData.size.x, world, pixPerMeter);
            drawables->push_back((MyDrawable*)dynamicCircle);
        }
        else if (drawableData.type == FINISH)
        {
            auto finish = new MyFinish(drawableData.position, drawableData.size, world, pixPerMeter, *windows[drawableData.windowID]);
            drawables->push_back((MyDrawable*)(finish));
            windowStaticObjects->push_back((MyWindowStaticObject*)finish);
        }
        else
        {
            std::cerr << "Unknown drawable type" << std::endl;
            break;
        }
    }

    
}

void nextLevel(int& currentLevel, int maxLevel, std::vector<MyDrawable*> *drawables, std::vector<MyWindowStaticObject*> *windowStaticObjects, std::vector<MyWindow*>& windows, sf::ContextSettings windowsSettings, b2World& world, float pixPerMeter)
{
    currentLevel++;
    if (currentLevel > maxLevel)
    {
        std::cout << "No more levels" << std::endl;
        //TODO Add a Finish screen
        return;
    }
    // We just need to clear the windowStaticObjects vector because it only contains pointers to drawables which will be deleted in the drawables vector
    windowStaticObjects->clear();
    loadLevel(currentLevel, drawables, windowStaticObjects, windows, windowsSettings, world, pixPerMeter);

}
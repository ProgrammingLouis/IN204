#pragma once

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>

#include "window.hpp"
#include "levels_data.hpp"
#include "drawable.hpp"
#include "finish.hpp"


void loadLevel(int levelID, std::vector<MyDrawable*> *drawables, std::vector<MyWindow*>& windows, sf::ContextSettings windowsSettings, b2World& world, float pixPerMeter)
{
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

    // Load the level
    for (int windowID = 0; windowID < levelsData[levelID].numberOfWindows; windowID++)
    {
        windows[windowID]->create(levelsData[levelID].videoModes[windowID], "Window " + std::to_string(windowID), sf::Style::Titlebar, windowsSettings);
        windows[windowID]->setVerticalSyncEnabled(true);
        windows[windowID]->setPosition(levelsData[levelID].windowPositions[windowID]);
    }


    // Create new drawables
    for (int drawableID = 0; drawableID < levelsData[levelID].drawablesData.size(); drawableID++)
    {
        MyDrawableData drawableData = levelsData[levelID].drawablesData[drawableID];
        if (drawableData.type == WINDOW_STATIC_BOX)
        {
            auto winStaticBox = new MyWindowStaticBox(drawableData.position, drawableData.size, world, pixPerMeter, *windows[drawableData.windowID]);
            drawables->push_back((MyDrawable*)(winStaticBox));
        }
        else if (drawableData.type == STATIC_BOX)
        {
            auto staticBox = new MyStaticBox(drawableData.position, drawableData.size, world, pixPerMeter);
            drawables->push_back((MyDrawable*)staticBox);
        }
        else if (drawableData.type == DYNAMIC_BOX)
        {
            auto dynamicBox = new MyDynamicBox(drawableData.position, drawableData.size, world, pixPerMeter);
            drawables->push_back((MyDrawable*)(dynamicBox));
        }
        else if (drawableData.type == DYNAMIC_CIRCLE)
        {
            auto dynamicCircle = new MyDynamicCircle(drawableData.position, drawableData.size.x, world, pixPerMeter);
            drawables->push_back((MyDrawable*)(dynamicCircle));
        }
        else if (drawableData.type == FINISH)
        {
            auto finish = new MyFinish(drawableData.position, drawableData.size, world, pixPerMeter, *windows[drawableData.windowID]);
            drawables->push_back((MyDrawable*)(finish));
        }
        else
        {
            std::cerr << "Unknown drawable type" << std::endl;
            break;
        }
    }

    
}

void nextLevel(int& currentLevel, int maxLevel, std::vector<MyDrawable*> *drawables, std::vector<MyWindow*>& windows, sf::ContextSettings windowsSettings, b2World& world, float pixPerMeter)
{
    currentLevel++;
    if (currentLevel > maxLevel)
    {
        std::cout << "No more levels" << std::endl;
        //TODO Add a Finish screen
        return;
    }
    loadLevel(currentLevel, drawables, windows, windowsSettings, world, pixPerMeter);

}
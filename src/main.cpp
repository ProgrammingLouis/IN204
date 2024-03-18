#include <SFML/Graphics.hpp>
#include <iostream>

// include Box2D
#include <Box2D/Box2D.h>
// #include "/usr/local/include/box2d/box2d.h"

#include <chrono>

// Local includes
#include "boxes.hpp"
#include "circles.hpp"
#include "window.hpp"
#include "finish.hpp"
#include "levels_data.hpp"
#include "levels_manager.hpp"
#include "virtual_method_classes.hpp"
#include "portals.hpp"


// Additinal SFML includes
#include <SFML/Audio.hpp>
#include <SFML/Window/Mouse.hpp>  // For desktop mouse position
#include <SFML/Window/Keyboard.hpp>  // For keyboard input

std::map<sf::Keyboard::Key, bool> keysPressed;

int main()
{
    //TODO Add cheat alert rules

    // Print SFML and Box2D version
    std::cout << "SFML version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << "." << SFML_VERSION_PATCH << std::endl;
    std::cout << "Box2D version: " << b2_version.major << "." << b2_version.minor << "." << b2_version.revision << std::endl;

    // Print desktop video modes
    auto desktopVideoMode = sf::VideoMode::getDesktopMode();
    //!! For some reason the desktop video mode is not the same as the screen resolution and it must be divided by 2
    desktopVideoMode.width /= 2;
    desktopVideoMode.height /= 2;
    std::cout << "Desktop video mode divided by 2 : " << desktopVideoMode.width << "x" << desktopVideoMode.height << "x" << desktopVideoMode.bitsPerPixel << std::endl;

    
    auto windowsSettings = sf::ContextSettings();
    windowsSettings.antialiasingLevel = 8;

    int currentLevel = 0;


    std::vector<MyWindow*> windows;
    std::vector<MyStaticWindow*> staticWindows;
    std::vector<MyForceFieldWindow*> forceFieldWindows;

    struct windows windows_struct = {windows, staticWindows, forceFieldWindows, windowsSettings};
    
    
    //TODO fix the window drag from the title bar 

    /* #region Loading windows for level 1 */
    auto window = new MyWindow(levelsData[0].videoModes[0], "MainWindow", sf::Style::Titlebar, windowsSettings);
    // mainWindow.setFramerateLimit(0);
    window->setVerticalSyncEnabled(true);
    window->setPosition(levelsData[0].windowPositions[0]);
    windows.push_back(window);

    auto window1 = new MyWindow(levelsData[0].videoModes[1], "Second window", sf::Style::Titlebar, windowsSettings);
    // secondWindow.setFramerateLimit(0);
    window1->setVerticalSyncEnabled(true);
    window1->setPosition(levelsData[0].windowPositions[1]);
    windows.push_back(window1);
    /* #endregion */

    /* #region textures and sprites definitions */

    // Load texture
    sf::Texture texture;
    if (!texture.loadFromFile("assets/imgs/SFML.png"))
    {
        std::cerr << "Error loading texture 'assets/imgs/SFML.png'" << std::endl;
        return 1;
    }
    texture.setSmooth(true);

    // Create a sprite
    auto sprite = sf::Sprite(texture);
    sprite.setTexture(texture);

    // Load screw head texture
    sf::Texture screwHeadTexture;
    if (!screwHeadTexture.loadFromFile("assets/imgs/screw_head.png"))
    {
        std::cerr << "Error loading texture 'assets/imgs/screw_head.png'" << std::endl;
        return 1;
    }
    screwHeadTexture.setSmooth(true);

    // Create a sprite
    auto screwHeadSprite = sf::Sprite(screwHeadTexture);
    screwHeadSprite.setTexture(screwHeadTexture);
    screwHeadSprite.setOrigin(screwHeadSprite.getLocalBounds().width/2, screwHeadSprite.getLocalBounds().height/2);
    screwHeadSprite.setScale(0.04, 0.04);
    std::vector<float> screwHeadRotations;
    for (int i = 0; i < 4; i++)
    {
        screwHeadRotations.push_back(rand()%360);
    }
    // screwHeadSprite.setScale(0.1, 0.1);
    // screwHeadSprite.setRotation(45);

    // Load force field texture
    sf::Texture forceFieldTexture;
    if (!forceFieldTexture.loadFromFile("assets/imgs/force_field.png"))
    {
        std::cerr << "Error loading texture 'assets/imgs/force_field.png'" << std::endl;
        return 1;
    }
    forceFieldTexture.setRepeated(true);

    // Create a sprite
    auto forceFieldSprite = sf::Sprite(forceFieldTexture);
    forceFieldSprite.setTexture(forceFieldTexture);
    forceFieldSprite.setOrigin(forceFieldSprite.getLocalBounds().width/2, forceFieldSprite.getLocalBounds().height/2);
    forceFieldSprite.setScale(0.5, 0.5);


    // Portal textures
    PortalTextures_struct portalTextures;
    loadPortalTextures(portalTextures);

    // Main circle
    sf::Texture mainCircleTexture;
    if (!mainCircleTexture.loadFromFile("assets/imgs/alienGreen_round.png"))
    {
        std::cerr << "Error loading texture 'assets/imgs/alienGreen_round.png'" << std::endl;
        return 1;
    }



    /* #endregion */

    /* #region sounds */
    // Load a sound to play
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("assets/sounds/level_lost.ogg"))
    {
        std::cerr << "Error loading sound 'assets/sounds/assets/sounds/level_lost.ogg'" << std::endl;
        return 1;
    }

    // Create a sound
    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.setVolume(50);

    /* #endregion */

    /* #region Box2D Init */
    b2Vec2 gravity(0.0f, 60.0f);
    b2World world(gravity);
    // world.SetAllowSleeping(true);
    // world.SetContinuousPhysics(true);
    //!! World size should be less than 2km (2000m) in any direction
    //!! Bodies size should be between 0.1m and 10m

    float pixPerMeter = desktopVideoMode.width / 500.0f;

    // Simulation parameters
    float timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 8;
    int32 positionIterations = 8;

    /* #endregion */
    
    /* #region objects definitions */

    std::vector<MyDrawable*> levelDrawables;
    std::vector<MyWindowStaticObject*> levelWindowStaticObjects;  //! For objects that need to move with their window, hence who have the updatePositionIfDrag() method
    std::vector<MyDrawable*> allLevelDrawables; //TODO this is only used for the main circle, maybe delete it
    std::vector<MyDynamicObject*> levelDynamicObject; //TODO use this or delete it, and clear it in the levels_manager
    std::vector<MyFinish<MyWindow>*> levelFinish;
    std::vector<MyFinish<MyStaticWindow>*> levelFinishStatic;

    MyDynamicCircle* mainCircle = new MyDynamicCircle(sf::Vector2f(200, 200), 20, world, pixPerMeter, mainCircleTexture);
    allLevelDrawables.push_back((MyDrawable*)mainCircle);
    levelDynamicObject.push_back((MyDynamicObject*)mainCircle);


    MyFinish<MyWindow>* finish = new MyFinish<MyWindow>(sf::Vector2f(0, 0), sf::Vector2f(40, 20), world, pixPerMeter, *(windows[0]));
    levelDrawables.push_back((MyDrawable*)finish);
    levelFinish.push_back(finish);

    struct gameObjects gameObjectsStruct = {&levelDrawables, mainCircle, &levelWindowStaticObjects, &levelDynamicObject, &levelFinish, &levelFinishStatic};

    /* #endregion */
    // Load level 1
    // nextLevel(currentLevel, levelsData.size()-1, &levelDrawables, mainCircle, finish, &levelWindowStaticObjects, windows, staticWindows, forceFieldWindows, windowsSettings, world, pixPerMeter);
    loadLevel(currentLevel, &gameObjectsStruct, &windows_struct, world, pixPerMeter);

    // // Request focus for all windows
    // for (auto& window : windows)
    // {
    //     window->requestFocus();
    // }
    // for (auto& staticWindow : staticWindows)
    // {
    //     staticWindow->requestFocus();
    // }
    // for (auto& forceFieldWindow : forceFieldWindows)
    // {
    //     forceFieldWindow->requestFocus();
    // }


    // MyDynamicBox* box = new MyDynamicBox(sf::Vector2f(350, 100), sf::Vector2f(20, 20), world, pixPerMeter);
    // levelDrawables.push_back((MyDrawable*)box);
    // levelDynamicObject.push_back((MyDynamicObject*)box);

    // MyStaticBox* staticBox = new MyStaticBox(sf::Vector2f(600, 400), sf::Vector2f(20, 20), world, pixPerMeter);
    // levelDrawables.push_back((MyDrawable*)staticBox);

    // // Put dynamic box on top of the static box
    // MyDynamicBox* box2 = new MyDynamicBox(sf::Vector2f(600, 380), sf::Vector2f(20, 20), world, pixPerMeter);
    // levelDrawables.push_back((MyDrawable*)box2);

    // // Add a static window
    // MyStaticWindow* staticWindow = new MyStaticWindow(sf::VideoMode(200, 200), "Static window", sf::Style::Titlebar, windowsSettings, sf::Vector2i(100, 100));
    // staticWindows.push_back(staticWindow);

    // // Add a force field window
    // MyForceFieldWindow* forceFieldWindow0 = new MyForceFieldWindow0(sf::VideoMode(200, 200), "Force field window", sf::Style::Resize, windowsSettings);
    // forceFieldWindows.push_back(forceFieldWindow0);

    std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
    float deltaTime = 0.0f;

    //*** MAIN LOOP ***//
    std::cout << "Starting main loop" << std::endl;
    bool allWindowsClosed = false;
    int frameCount = 0;
    while (!allWindowsClosed)
    {
        // std::cout << "Frame " << frameCount << std::endl;

        //!! Don't delete this
        // auto now = std::chrono::steady_clock::now();
        // deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - lastTime).count()*100.0f;
        // lastTime = now;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            mainCircle->body->ApplyLinearImpulseToCenter(b2Vec2(-100.0f, 0.0f), true);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            mainCircle->body->ApplyLinearImpulseToCenter(b2Vec2(100.0f, 0.0f), true);
        }

        //TODO maybe delete this cause it will close even if Escape is pressed on another app
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            for (auto& window : windows)
            {
                window->close();
            }
            for (auto& staticWindow : staticWindows)
            {
                staticWindow->close();
            }
            for (auto& forceFieldWindow : forceFieldWindows)
            {
                forceFieldWindow->close();
            }
        }
        //TODO delete this for final game
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            if (!keysPressed[sf::Keyboard::Space])
            {
                nextLevel(currentLevel, levelsData.size()-1, &gameObjectsStruct, &windows_struct, world, pixPerMeter);
                keysPressed[sf::Keyboard::Space] = true;
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            if (!keysPressed[sf::Keyboard::R])
            {
                // Restart level
                sound.play();
                loadLevel(currentLevel, &gameObjectsStruct, &windows_struct, world, pixPerMeter);
                keysPressed[sf::Keyboard::R] = true;
            }
        }

        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            keysPressed[sf::Keyboard::Space] = false;
        }
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            keysPressed[sf::Keyboard::R] = false;
        }

        // // If circle is in the finish zone, go to the next level
        // //TODO check if the finish box is right
        // if (mainCircle->body->GetLinearVelocity().Length() < 0.1f && finish->isInside(mainCircle->body->GetPosition()))
        // {
        //     //TODO uncomment this for final game
        //     // nextLevel(currentLevel, levelsData.size()-1, &levelDrawables, mainCircle, &levelWindowStaticObjects, windows, staticWindows, forceFieldWindows, windowsSettings, world, pixPerMeter);
        // }

        // if mainCicle falls off the screen, restart the level
        if (mainCircle->body->GetPosition().y*pixPerMeter > desktopVideoMode.height || mainCircle->body->GetPosition().y*pixPerMeter < 0 || mainCircle->body->GetPosition().x*pixPerMeter > desktopVideoMode.width || mainCircle->body->GetPosition().x*pixPerMeter < 0)
        {
            // Restart level
            std::cout << "Restarting level because mainCircle fell off the screen" << std::endl;
            loadLevel(currentLevel, &gameObjectsStruct, &windows_struct, world, pixPerMeter);
            sound.play();
        }

        // std::cout << "line 291" << std::endl;
        //!!
        //TODO change this because window static objects that are in a static window do not need to be updated
        for (auto& winStaticObject : levelWindowStaticObjects)
        {
            winStaticObject->updatePositionIfDrag(pixPerMeter);
        }
        // finish->updatePositionIfDrag(pixPerMeter);
        for (auto& finish : levelFinish)
        {
            finish->updatePositionIfDrag(pixPerMeter);
        }

        // std::cout << "line 304" << std::endl;

        // Apply upward force to all levelDynamicObject in all forceFieldWindows
        //TODO optimize this, calculate the object current windows only once per frame
        for (auto& forceFieldWindow : forceFieldWindows)
        {
            if (forceFieldWindow->isOpen()) {
                // std::cout << "forceFieldWindow is open" << std::endl;
                for (auto& dynamicObject : levelDynamicObject)
                {
                    // std::cout << "getting forceFieldWindow position" << std::endl;
                    const sf::Vector2i winPos = forceFieldWindow->getPosition();
                    // std::cout << "winPos: " << winPos.x << ", " << winPos.y << std::endl;
                    auto dynamicObjectPos = dynamicObject->body->GetPosition();
                    // std::cout << "dynamicObjectPos: " << dynamicObjectPos.x*pixPerMeter << ", " << dynamicObjectPos.y*pixPerMeter << std::endl;
                    if (dynamicObject->body->GetPosition().x*pixPerMeter > winPos.x && dynamicObject->body->GetPosition().x*pixPerMeter < winPos.x+forceFieldWindow->getSize().x && dynamicObject->body->GetPosition().y*pixPerMeter > winPos.y && dynamicObject->body->GetPosition().y*pixPerMeter < winPos.y+forceFieldWindow->getSize().y)
                    {
                        // std::cout << "Applying force to dynamicObject" << std::endl;
                        dynamicObject->body->SetAwake(true);
                        dynamicObject->body->ApplyForceToCenter(b2Vec2(0.0f+100.0f*dynamicObject->body->GetMass()*std::sin(forceFieldWindow->angle*3.14159/180), -100.0f*dynamicObject->body->GetMass()*std::cos(forceFieldWindow->angle*3.14159/180)), true);
                    }
                }
            }
        }

        // std::cout << "line 322" << std::endl;

        // // apply a force to mainCricle to offset gravity
        // mainCircle->body->ApplyForceToCenter(b2Vec2(0.0f, -100.0f*mainCircle->body->GetMass()), true);
        
        world.Step(timeStep, velocityIterations, positionIterations);
        
        // std::cout << "line 329" << std::endl;

        /* #region Windows update */
        allWindowsClosed = true;
        int winID =0;
        for (auto& window : windows)
        {
            bool winStartedDragging = window->pollEvents();
            if (winStartedDragging) std::cout << "Window " << winID << " started dragging" << std::endl;
            
            window->calculateNewPosDrag(windows, staticWindows);

            //!!
            //TODO determine object inside and outside the dragging window

            //TODO mabe should put the body in sleep mode here
            if (winStartedDragging) {
                for (auto& dynamicObject : levelDynamicObject)
                {
                    dynamicObject->startDagPos = dynamicObject->body->GetPosition();
                }
            }

            if (window->getIsDragging()) {
                for (auto& dynamicObject : levelDynamicObject)
                {
                    if (dynamicObject->body->GetPosition().x*pixPerMeter > window->getPosition().x && dynamicObject->body->GetPosition().x*pixPerMeter < window->getPosition().x+window->getSize().x && dynamicObject->body->GetPosition().y*pixPerMeter > window->getPosition().y && dynamicObject->body->GetPosition().y*pixPerMeter < window->getPosition().y+window->getSize().y)
                    {
                        b2Vec2 bodyPosWithoutDelta = dynamicObject->body->GetPosition();
                        sf::Vector2i winDelta = window->newPos - window->getPosition();
                        dynamicObject->body->SetTransform(b2Vec2(bodyPosWithoutDelta.x+winDelta.x/pixPerMeter, bodyPosWithoutDelta.y+winDelta.y/pixPerMeter), dynamicObject->body->GetAngle());
                    }
                }
            }

            if (window->getIsDragging()) {
                window->setPosition(window->newPos);
            }

            if (window->isOpen()) {
                window->clear();
                window->draw(levelDrawables, pixPerMeter);
                window->draw(allLevelDrawables, pixPerMeter);
                window->display();
                allWindowsClosed = false;
            }
            winID++;
        }

        // std::cout << staticWindows.size() << std::endl;
        for (auto& staticWindow : staticWindows)
        {
            if (staticWindow->isOpen()) {
                staticWindow->pollEvents();
                staticWindow->resetPosition();

                staticWindow->clear();
                
                //TODO do not run draw on static objects for static windows
                staticWindow->draw(levelDrawables, pixPerMeter);
                staticWindow->draw(allLevelDrawables, pixPerMeter);

                //TODO put this in a function
                for (int i = 0; i < 2; i++)
                {
                    for (int j = 0; j < 2; j++)
                    {
                        int margin = 15;
                        screwHeadSprite.setPosition(i*(staticWindow->getSize().x-2*margin)+margin, j*(staticWindow->getSize().y-2*margin)+margin);
                        screwHeadSprite.setRotation(screwHeadRotations[i*2+j]);
                        staticWindow->sf::RenderWindow::draw(screwHeadSprite);
                    }
                }
                staticWindow->display();
                allWindowsClosed = false;
            }
        }

        for (auto& forceFieldWindow : forceFieldWindows)
        {
            if (forceFieldWindow->isOpen()) {
                forceFieldWindow->pollEvents();
                if (forceFieldWindow->getIsDragging()) {
                    forceFieldWindow->setPosition(forceFieldWindow->startDragWindowPos+forceFieldWindow->mouseDelta);
                }

                forceFieldWindow->clear();
                forceFieldWindow->draw(levelDrawables, pixPerMeter, forceFieldSprite);
                forceFieldWindow->draw(allLevelDrawables, pixPerMeter, forceFieldSprite);
                forceFieldWindow->display();
                allWindowsClosed = false;
            }
        }

        /* #endregion */
        
        // std::cout << "line 423" << std::endl;

        frameCount++;
    }

    // Free memory
    //TODO Check if this is the right way to free memory

    std::cout << "Clearing memory" << std::endl;
    for (auto& window : windows)
    {
        delete window;
    }
    for (auto& staticWindow : staticWindows)
    {
        delete staticWindow;
    }
    for (auto& forceFieldWindow : forceFieldWindows)
    {
        delete forceFieldWindow;
    }
    std::cout << "Cleared windows" << std::endl;

    for (auto& drawable : levelDrawables)
    {
        delete drawable;
    }
    std::cout << "Cleared levelDrawables" << std::endl;
    
    delete mainCircle;
    delete finish;
    std::cout << "Cleared allLevelDrawables" << std::endl;

    return 0;
}
#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>


class Button
{
    public:
        sf::Texture defaultTexture;
        sf::Sprite defaultSprite;
        sf::Texture hoverTexture;
        sf::Sprite hoverSprite;
        sf::Vector2f position;
        float scale;
        bool isHover = false;

        Button(const std::string& defaultTexturePath, const std::string& hoverTexturePath, const sf::Vector2f& position, float scale) : position(position), scale(scale)
        {
            if (!defaultTexture.loadFromFile(defaultTexturePath)) {
                std::cerr << "Error loading texture '" << defaultTexturePath << "'" << std::endl;
            }
            defaultSprite.setTexture(defaultTexture);
            defaultSprite.setOrigin(defaultSprite.getGlobalBounds().width/2, defaultSprite.getGlobalBounds().height/2);
            defaultSprite.setPosition(position);
            defaultSprite.scale(scale, scale);

            if (!hoverTexture.loadFromFile(hoverTexturePath)) {
                std::cerr << "Error loading texture '" << hoverTexturePath << "'" << std::endl;
            }
            hoverSprite.setTexture(hoverTexture);
            hoverSprite.setOrigin(hoverSprite.getGlobalBounds().width/2, hoverSprite.getGlobalBounds().height/2);
            hoverSprite.setPosition(position);
            hoverSprite.scale(scale, scale);
        }

        virtual void setPosition(const sf::Vector2f& position) {
            this->position = position;
            defaultSprite.setPosition(position);
            hoverSprite.setPosition(position);
        }

        void calculateHover(sf::Vector2f mousePos) {
            sf::Vector2f topLeftCornerPos = position - sf::Vector2f(defaultSprite.getGlobalBounds().width/2, defaultSprite.getGlobalBounds().height/2);
            isHover = mousePos.x > topLeftCornerPos.x && mousePos.x < topLeftCornerPos.x + defaultSprite.getGlobalBounds().width && mousePos.y > topLeftCornerPos.y && mousePos.y < topLeftCornerPos.y + defaultSprite.getGlobalBounds().height;
        }

        virtual void draw(sf::RenderWindow& window) {
            if (isHover) window.draw(hoverSprite);
            else window.draw(defaultSprite);
        }
};


class LevelButton : public Button
{
    public:
        int level; //! 0 based level index, which is displayed as level+1
        sf::Text numberText;

        LevelButton(const std::string& defaultTexturePath, const std::string& hoverTexturePath, const sf::Vector2f& position, float scale, int level, sf::Font& font) : Button(defaultTexturePath, hoverTexturePath, position, scale), level(level)
        {
            numberText.setFont(font);
            numberText.setString(std::to_string(level+1));
            numberText.setFillColor(sf::Color::White);
            numberText.setOrigin(numberText.getGlobalBounds().width/2, numberText.getGlobalBounds().height/2);
            numberText.setPosition(sf::Vector2f(position.x-15, position.y-20));
        }

        void setPosition(const sf::Vector2f& position) override {
            Button::setPosition(position);
            numberText.setPosition(sf::Vector2f(position.x-15, position.y-20));
        }

        void draw(sf::RenderWindow& window) override {
            Button::draw(window);
            window.draw(numberText);
        }
};


enum MenuEvent
{
    MenuEvent_NONE,
    MenuEvent_LEVEL_SELECTED,
    MenuEvent_QUIT
};

enum MenuTab
{
    MenuTab_MAIN,
    MenuTab_LEVELS
};

class Menu : public sf::RenderWindow
{
public:
    sf::Font menuFont;
    sf::VideoMode videoMode;

    int selectedLevel = 0;

    MenuTab currentTab = MenuTab_MAIN;

    Button playButton = Button("assets/imgs/menu/playDefault.png", "assets/imgs/menu/playHover.png", sf::Vector2f(100, 100), 0.33);
    Button quitButton = Button("assets/imgs/menu/quitDefault.png", "assets/imgs/menu/quitHover.png", sf::Vector2f(100, 300), 0.33);
    
    // Button backButton = Button("assets/imgs/menu/backDefault.png", "assets/imgs/menu/backHover.png", sf::Vector2f(100, 100), 0.33);

    std::vector<Button*> MainTabButtons;
    std::vector<Button*> LevelTabButtons;

    public:
        Menu(const sf::VideoMode& videoMode, const std::string& title, sf::Uint32 style, const sf::ContextSettings& settings, int numberOfLevels) : sf::RenderWindow(videoMode, title, style, settings), videoMode(videoMode)
        {
            setVerticalSyncEnabled(true);
            playButton.setPosition(sf::Vector2f(videoMode.width/2, videoMode.height*2/4));
            quitButton.setPosition(sf::Vector2f(videoMode.width/2, videoMode.height*3/4));

            MainTabButtons.push_back(&playButton);
            MainTabButtons.push_back(&quitButton);

            int levelPerRow = 5;
            for (int i = 0; i < numberOfLevels; i++) {
                LevelButton* levelButton = new LevelButton("assets/imgs/menu/levelDefault.png", "assets/imgs/menu/levelHover.png", sf::Vector2f(400, 100), 0.3, i, menuFont);
                levelButton->setPosition(sf::Vector2f(videoMode.width/2 + (i%levelPerRow - levelPerRow/2)*100, videoMode.height*(i/levelPerRow+2)/4));

                LevelTabButtons.push_back(levelButton);
            }
        }

        int loadFonts() {
            if (!menuFont.loadFromFile("assets/fonts/Kenney_Bold.ttf")) {
                std::cerr << "Error loading font 'assets/fonts/Kenney_Bold.ttf'" << std::endl;
                return 1;
            }
            return 0;
        }

        // int loadSprites() {
        //     // load play texture
        //     if (!playTexture.loadFromFile("assets/imgs/menu/playDefault.png")) {
        //         std::cerr << "Error loading texture 'assets/sprites/playDefault.png'" << std::endl;
        //         return 1;
        //     }
        //     playSprite.setTexture(playTexture);
        //     playSprite.scale(0.33, 0.33);


        //     if (!quitTexture.loadFromFile("assets/imgs/menu/quitDefault.png")) {
        //         std::cerr << "Error loading texture 'assets/sprites/quitDefault.png'" << std::endl;
        //         return 1;
        //     }
        //     quitSprite.setTexture(quitTexture);

        //     // quitSprite = newQuitSprite;

        //     return 0;
        // }

        int loadAssets() {
            if (loadFonts() != 0) return 1;
            return 0;
        }

        void draw() {
            if (currentTab == MenuTab_MAIN) {
                // Draw Title Text
                //! This will create a sf::Text object every time the draw function is called
                //! but this is needed otherwise getGlobalBounds() will return 0 for some reason :(
                sf::Text titleText("WinRoll", menuFont, 50);
                titleText.setFillColor(sf::Color::Blue);
                // titleText.setOutlineColor(sf::Color::Red);
                // titleText.setOutlineThickness(5);

                titleText.setOrigin(titleText.getGlobalBounds().width/2, titleText.getGlobalBounds().height/2);
                titleText.setPosition(videoMode.width/2, videoMode.height*1/4);
                sf::RenderWindow::draw(titleText);

                for (Button* button : MainTabButtons) {
                    button->draw(*this);
                }
            }
            else if (currentTab == MenuTab_LEVELS) {
                // Draw Level Select Text
                sf::Text titleText("Levels", menuFont, 50);
                titleText.setFillColor(sf::Color(229, 37, 84, 255));
                // titleText.setFillColor(sf::Color(42, 46, 55, 100));
                titleText.setOutlineColor(sf::Color(42, 46, 55, 100));
                // titleText.setOutlineColor(sf::Color(229, 37, 84, 255));
                titleText.setOutlineThickness(2);

                titleText.setOrigin(titleText.getGlobalBounds().width/2, titleText.getGlobalBounds().height/2);
                titleText.setPosition(videoMode.width/2, videoMode.height*1/4);
                sf::RenderWindow::draw(titleText);

                for (Button* button : LevelTabButtons) {
                    button->draw(*this);
                }
            }
        }

        MenuEvent pollEvents() {
            for (auto event = sf::Event{}; sf::RenderWindow::pollEvent(event);)
            {
                if (event.type == sf::Event::Closed) close();
                else if (event.type == sf::Event::MouseMoved) {
                    std::vector<Button*>* buttons;
                    if (currentTab == MenuTab_MAIN) buttons = &MainTabButtons;
                    else if (currentTab == MenuTab_LEVELS) buttons = &LevelTabButtons;

                    for (Button* button : *buttons) {
                        button->calculateHover(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
                    }
                }
                else if (event.type == sf::Event::MouseButtonPressed) {
                    if (currentTab == MenuTab_MAIN) {
                        if (playButton.isHover) {
                            currentTab = MenuTab_LEVELS;
                        }
                        else if (quitButton.isHover) {
                            close();
                            return MenuEvent_QUIT;
                        }
                    }
                    else if (currentTab == MenuTab_LEVELS) {
                        for (Button* button : LevelTabButtons) {
                            if (button->isHover) {
                                selectedLevel = static_cast<LevelButton*>(button)->level;
                                return MenuEvent_LEVEL_SELECTED;
                            }
                        }
                    }
                }
            }
            return MenuEvent_NONE;
        }
};
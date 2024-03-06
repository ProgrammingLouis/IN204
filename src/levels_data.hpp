#pragma once

#include <SFML/Graphics.hpp>

namespace level1 {
    int numberOfWindows = 2;
    std::vector<sf::VideoMode> videoModes = {sf::VideoMode(400, 300), sf::VideoMode(400, 300)};
    std::vector<sf::Vector2i> windowPositions = {sf::Vector2i(100, 100), sf::Vector2i(800, 200)};


}
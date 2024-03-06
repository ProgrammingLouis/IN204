#pragma once

#include <SFML/Graphics.hpp>

namespace level1 {
    int numberOfWindows = 2;
    std::vector<sf::VideoMode> videoModes = {sf::VideoMode(800, 600), sf::VideoMode(400, 300)};
    std::vector<sf::Vector2i> windowPositions = {sf::Vector2i(10, 0), sf::Vector2i(400, 300)};
}
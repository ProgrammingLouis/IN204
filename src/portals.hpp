#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>


enum PortalColor
{
    BLUE,
    GREEN,
    PINK,
    PURPLE,
    RED
};

typedef struct PortalTextures
{
    sf::Texture blueTexture;
    sf::Texture greenTexture;
    sf::Texture pinkTexture;
    sf::Texture purpleTexture;
    sf::Texture redTexture;
} PortalTextures_struct;

int loadPortalTextures(PortalTextures_struct& textures)
{
    if (!textures.blueTexture.loadFromFile("assets/portal.png"))
    {
        return 1;
    }
    if (!textures.greenTexture.loadFromFile("assets/portal_green.png"))
    {
        return 1;
    }
    if (!textures.pinkTexture.loadFromFile("assets/portal_pink.png"))
    {
        return 1;
    }
    if (!textures.purpleTexture.loadFromFile("assets/portal_purple.png"))
    {
        return 1;
    }
    if (!textures.redTexture.loadFromFile("assets/portal_red.png"))
    {
        return 1;
    }
    return 0;
};

// class MyPortal : public sf::Drawable
// {
//     public:
//         PortalColor color;

//         MyPortal(PortalColor color, sf::Vector2f position, sf::Vector2f size, sf::Texture& texture) : color(color)
//         {
//             shape.setSize(size);
//             shape.setPosition(position);
//             shape.setTexture(&texture);
//         }
// }
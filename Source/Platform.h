#pragma once
#include <SFML/Graphics.hpp>

class Platform
{
    public:
        Platform();
        void update();
        void draw(sf::RenderWindow& window);
        std::vector<sf::Sprite>* getPlatforms();
        void createPlatform(float height, int type = 0);

        sf::Texture m_large;

    private:
        sf::Sprite m_rect;
        std::vector<sf::Sprite> m_platforms;
        sf::Texture m_small;
        sf::Texture m_medium;
        sf::Texture m_Ssmall;
        sf::Texture m_Smedium;
};

#pragma once
#include <SFML/Graphics.hpp>

class Platforms
{
    public:
        Platforms();
        void draw(const sf::RenderWindow& window);
        void update(float dt);
        void createPlatforms();
        std::vector<sf::RectangleShape>* getPlatforms();

    private:
    std::vector<sf::RectangleShape> m_platforms;


};

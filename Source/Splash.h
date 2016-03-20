#pragma once
#include <SFML/Graphics.hpp>

class Splash
{
    public:
        Splash();
        void run();
        void render();
        void handleInput();

    private:

        sf::Texture m_text;
        sf::Sprite m_sprite;

        bool m_running;
};

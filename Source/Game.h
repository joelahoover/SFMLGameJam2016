#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Platform.h"
#include "Player.h"
#include "AssetManager.h"
#include "Animator.h"

class Game
{
    public:
        Game();
        void run();
    private:
        void handleEvents();
        void update(sf::Time dt);
        void draw();

        void restart();

        AssetManager m_manager;

        sf::Time m_timer;


        sf::RenderWindow m_window;
        sf::View m_view;

        Platform m_platforms;
        Player m_player;

        const float m_gravity;
        const float m_maxY;

        sf::Vector2f m_vel;

        //Wall and floor stuff
        sf::Texture m_wallText;
        sf::Texture m_floorText;
        sf::Sprite m_lWall;
        sf::Sprite m_rWall;
        sf::Sprite m_lWall1;
        sf::Sprite m_rWall1;
        sf::Sprite m_floor;
        sf::FloatRect m_floorRect;


        static const sf::Time timePerFrame;

        float m_playerSpeed;
        int m_attempt;

        int m_floorNumber;
        int m_currentFloor;

        bool m_moveCamera;

        sf::SoundBuffer m_themeMusicB;
        sf::SoundBuffer m_jumpB;
        sf::SoundBuffer m_failB;
        sf::SoundBuffer m_bounceB;

        sf::Music m_themeMusic;
        sf::Sound m_jump;
        sf::Sound m_fail;
        sf::Sound m_bounce;

        bool m_paused;
        bool m_lost;
        int m_cameraSpeed;

        sf::RectangleShape m_lostScreen;
        sf::FloatRect m_playerBody;

        sf::Font m_font;
        sf::Text m_time;
        sf::Text m_score;
        sf::Text m_level;
        sf::Text m_message;

        int m_sat;
        int m_points;
        bool m_increase;

        sf::Texture m_splashT;
        sf::Sprite m_splash;
        bool m_start;
        sf::Text m_splashText;
        sf::RectangleShape m_splashBG;
};

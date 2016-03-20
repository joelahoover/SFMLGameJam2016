#pragma once
#include <SFML/Graphics.hpp>
#include "Animator.h"

enum class pDir {
        LEFT, RIGHT, NONE,
};

class Player
{
    public:
        Player();
        void update(float dt);
        void draw(sf::RenderWindow& window);
        void jump();
        void bounce(sf::Vector2f direction, float dt);

        sf::FloatRect getBounds();
        bool isFalling();
        bool isJumping();
        void setPosition(sf::Vector2f position);
        void setPosition(float x, float y);

        void setStatus(bool isGrounded, bool floor=false);
        int getAttempt();
        void setFailure(int num);
        pDir pDirection;
    private:
        sf::Texture m_text;
        sf::Texture m_paraText;
        sf::Sprite m_rect;

        sf::Vector2i m_playerSize = sf::Vector2i(67, 56);
        //sf::Sprite m_playerSprite;
        Animator m_playerAnimator = Animator(m_rect);


        float approach(float goal, float current, float dt);
        bool m_canJump;
        bool m_falling;
        bool m_jumping;
        bool m_bouncing;
        bool m_onFloor;
        bool m_onPlatform;

        bool m_canMoveRight;
        bool m_canMoveLeft;

        float m_bounceCounter;

        sf::Vector2f m_vel;
        sf::Vector2f m_goal;

        const float m_gravity;
        const float m_maxY;

        float m_topHeight;
        int m_attempt;

        float m_playerSpeed;
        float m_currSpeed;

        float m_fallSpeed;

};

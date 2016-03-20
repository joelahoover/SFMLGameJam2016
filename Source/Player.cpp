#include "Player.h"
#include <iostream>


Player::Player() : m_gravity(0.5), m_maxY(50)
{
    m_canJump = true;
    m_falling = false;
    m_jumping = false;
    m_bouncing = false;

    m_onFloor = true;
    m_onPlatform = false;

    m_canMoveLeft = true;
    m_canMoveRight = true;
    m_vel = sf::Vector2f(0, 0);
    m_goal = sf::Vector2f(0, 0);

    m_bounceCounter = 50;

    m_paraText.loadFromFile("data/para.png");

    m_rect.setPosition(300, 800);

    m_topHeight = 800;
    m_attempt = 0;

    m_playerSpeed = 600;
    pDirection = pDir::NONE;
    m_fallSpeed = 15;


    auto& idle_l = m_playerAnimator.CreateAnimation("idle_l", "data/left.png", sf::seconds(0.5), true);
    idle_l.AddFrames(sf::Vector2i(0,0), m_playerSize, 1);

    auto& idle_r = m_playerAnimator.CreateAnimation("idle_r", "data/right.png", sf::seconds(0.5), true);
    idle_r.AddFrames(sf::Vector2i(268,0), m_playerSize, 1);

    auto& walkLeft = m_playerAnimator.CreateAnimation("walk_l", "data/left.png", sf::seconds(0.5), true);
    walkLeft.AddFrames(sf::Vector2i(0,0), m_playerSize, 5);

    auto& walkRight = m_playerAnimator.CreateAnimation("walk_r", "data/right.png", sf::seconds(0.5), true);
    walkRight.AddFrames(sf::Vector2i(0,0), m_playerSize, 5);


    m_playerAnimator.SwitchAnimation("idle_l");
    m_rect.scale(1.2, 1.2);

}

void Player::update(float dt)
{

    if(m_attempt < 2)
        m_playerAnimator.Update(sf::seconds(dt));
    //Update top hight
    if(m_rect.getPosition().y < m_topHeight)
    {
        m_topHeight = m_rect.getPosition().y;
    }

    if(m_attempt == 1)
    {
        m_attempt = 2;
        m_rect.setTexture(m_paraText);
        m_rect.setTextureRect(sf::IntRect(80, 0, 80, 130));
        m_rect.scale(0.9, 0.9);
        m_rect.move(0, -32);
        m_fallSpeed = 5;
        m_playerSpeed = 150;
        m_bouncing = false;
        m_falling = true;
        m_rect.setPosition(400, m_rect.getPosition().y);
    }


    //If we bounce from the walls
    if(m_bouncing)
    {
        if(m_bounceCounter <= 0)
        {
            m_canMoveLeft = true;
            m_canMoveRight = true;
        }
        m_bounceCounter -= 3;
    }
    //If in air apply gravity
    if(m_falling || m_jumping)
    {
        if(m_vel.y < m_maxY)
            m_vel.y += m_gravity;
        if(m_vel.y >= m_fallSpeed)
            m_vel.y = m_fallSpeed;
        m_rect.move(m_vel);

    }

    if(m_jumping)
    {
        m_onFloor = false;
        m_onPlatform = false;
        m_canJump = false;

        if(m_vel.y < 0)
        {
            m_vel.y += 0.5;
        }
        else
        {
            m_jumping = false;
            m_falling = true;
        }
    }


    //Player movement
    if(pDirection == pDir::LEFT && m_canMoveLeft)
    {
        if(m_playerAnimator.GetCurrentAnimationName() != "walk_l" && m_attempt < 2)
        {
            m_playerAnimator.SwitchAnimation("walk_l");
        }

        if(m_attempt == 2)
        {
            m_rect.setTextureRect(sf::IntRect(0, 0, 80, 130));
        }

        if(m_currSpeed > -m_playerSpeed * dt && !m_bouncing)
        {
            m_currSpeed -= (m_playerSpeed * dt) / 10;
        }
        else
            m_currSpeed = -m_playerSpeed * dt;

        m_rect.move(m_currSpeed, 0);
        if(m_rect.getPosition().x + m_rect.getGlobalBounds().width < 740 && m_rect.getPosition().x > 60)
            m_vel.x = 0;
    }
    if(pDirection == pDir::RIGHT && m_canMoveRight)
    {
        if(m_playerAnimator.GetCurrentAnimationName() != "walk_r"  && m_attempt < 2)
        {
            m_playerAnimator.SwitchAnimation("walk_r");
        }

        if(m_attempt == 2)
        {
            m_rect.setTextureRect(sf::IntRect(160, 0, 80, 130));
        }

        if(m_currSpeed < m_playerSpeed * dt && !m_bouncing)
        {
            m_currSpeed += (m_playerSpeed * dt) / 10;
        }
        else
            m_currSpeed = m_playerSpeed * dt;


        m_rect.move(m_currSpeed, 0);
        if(m_rect.getPosition().x + m_rect.getGlobalBounds().width < 740 && m_rect.getPosition().x > 60)
            m_vel.x = 0;
    }

    if(pDirection == pDir::NONE)
    {
        m_currSpeed = 0;

        if(m_attempt < 2)
        {
            if(m_playerAnimator.GetCurrentAnimationName() == "walk_l")
                m_playerAnimator.SwitchAnimation("idle_l");
            else if(m_playerAnimator.GetCurrentAnimationName() == "walk_r")
                m_playerAnimator.SwitchAnimation("idle_r");
        }
        else if(m_attempt == 2)
        {
            m_rect.setTextureRect(sf::IntRect(80, 0, 80, 130));
        }

    }


}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(m_rect);
}

void Player::jump()
{
    if(m_canJump)
    {
        m_vel.y = -18;
        m_jumping = true;
        m_onFloor = false;
        m_onPlatform = false;
    }
}


void Player::bounce(sf::Vector2f direction, float dt)
{
    if(m_onFloor || m_onPlatform || m_bouncing)
    {
        if(m_vel.x > 0)
            m_vel.x = -2;
        else if(m_vel.x < 0)
            m_vel.x = 2;
        if(direction.x > 0)
            m_rect.setPosition(m_rect.getPosition().x - m_currSpeed, m_rect.getPosition().y);
        else
            m_rect.setPosition(m_rect.getPosition().x - m_currSpeed, m_rect.getPosition().y);
    }
    else if(m_falling)
    {
        if(m_vel.x > 0)
            m_vel.x = -2;
        else if(m_vel.x < 0)
            m_vel.x = 2;
        if(direction.x > 0)
            m_rect.setPosition(m_rect.getPosition().x - m_currSpeed, m_rect.getPosition().y);
        else
            m_rect.setPosition(m_rect.getPosition().x - m_currSpeed, m_rect.getPosition().y);
    }
    else
    {
        if(!m_bouncing)
        {
            if(direction.x > 0)
            {
                m_vel.x = m_playerSpeed * dt;
                m_canMoveLeft = false;
            }
            else if(direction.x < 0)
            {
                m_canMoveRight = false;
                m_vel.x = -m_playerSpeed * dt;
            }

            m_vel.y = -20;
            m_bouncing = true;
            m_bounceCounter = 50;


        }
    }
}

sf::FloatRect Player::getBounds()
{
    return m_rect.getGlobalBounds();
}

void Player::setPosition(sf::Vector2f position)
{
    m_rect.setPosition(position);
}

void Player::setPosition(float x, float y)
{
    m_rect.setPosition(x, y);
}

void Player::setStatus(bool isGrounded, bool floor)
{
    if(isGrounded)
    {
        m_falling = false;
        m_jumping = false;
        m_canJump = true;
        m_bouncing = false;
        if(floor)
            m_onFloor = true;
        else
            m_onPlatform = true;
        m_canMoveLeft = true;
        m_canMoveRight = true;
        m_vel.x = 0;
        m_vel.y = 0;
        m_bouncing = false;
    }
    else
    {
        if(!m_onFloor && !m_jumping)
        {
            m_canJump = false;
            m_falling = true;
         }
    }
}

bool Player::isFalling()
{
    return m_falling;
}

int Player::getAttempt()
{
    return m_attempt;
}

float Player::approach(float goal, float current, float dt)
{
    float difference = goal - current;

    if(difference > dt)
        return current + dt;
    if(difference < -dt)
        return current -dt;

    return goal;
}


bool Player::isJumping()
{
    return m_jumping;
}

void Player::setFailure(int num)
{
    m_attempt = num;
    if(num == 0)
    {
        m_playerAnimator.SwitchAnimation("idle_l");
        m_rect.scale(1.2, 1.2);
        m_canJump = true;
        m_falling = false;
        m_jumping = false;
        m_bouncing = false;

        m_onFloor = true;
        m_onPlatform = false;

        m_canMoveLeft = true;
        m_canMoveRight = true;
        m_vel = sf::Vector2f(0, 0);
        m_goal = sf::Vector2f(0, 0);

        m_bounceCounter = 50;
        m_rect.setPosition(300, 800);
        m_topHeight = 800;
        m_fallSpeed = 15;
        m_playerSpeed = 600;
    }
}


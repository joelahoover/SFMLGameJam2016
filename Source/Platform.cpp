#include "Platform.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>

#include "ResourcePath.h"

Platform::Platform()
{
    m_small.loadFromFile(resourcePath() + "data/cloud_s.png");
    m_medium.loadFromFile(resourcePath() + "data/cloud_m.png");
    m_large.loadFromFile(resourcePath() + "data/cloud_l.png");

    m_Ssmall.loadFromFile(resourcePath() + "data/spike_s.png");
    m_Smedium.loadFromFile(resourcePath() + "data/spike_m.png");
    std::srand((unsigned)time(0));

    int tempNum = 700;
    for(int i = 0; i < 500; ++i)
    {
        this->createPlatform(tempNum);
        tempNum -= 150;
    }
}

void Platform::update()
{

}

void Platform::draw(sf::RenderWindow& window)
{
    for(const auto plat : m_platforms)
        window.draw(plat);
}

std::vector<sf::Sprite>* Platform::getPlatforms()
{
    return &m_platforms;
}

void Platform::createPlatform(float height, int type)
{
    int tempSize;
    sf::Vector2f m_platformPosition(0,height);

    //Normal platforms, first attempt
    if(type == 0)
    {
        //X pos random
        m_platformPosition.x = (rand()%600) + 50;
        m_rect.setPosition(m_platformPosition);

        //Size random
        tempSize = (rand()%3);
        switch(tempSize)
        {

            case 0:
                m_rect.setTexture(m_small, true);
            break;
            case 1:
                m_rect.setTexture(m_medium, true);
            break;
            default:
                m_rect.setTexture(m_large, true);
            break;
        }

    }
    //Spikes, second attemtp
    else if(type == 1)
    {
        //m_rect.setFillColor(sf::Color::Red);
        //X pos random
        m_platformPosition.x = (rand()%600) + 50;
        m_rect.setPosition(m_platformPosition);

        //Size random
        tempSize = (rand()%2);
        //Change this to spikes later
        switch(tempSize)
        {
            case 0:
                m_rect.setTexture(m_Ssmall, true);
            break;
            case 1:
                m_rect.setTexture(m_Smedium, true);
            break;
            default:
            break;
        }
    }

    //If the platform is to far to the right move it left
    if(m_platformPosition.x + m_rect.getGlobalBounds().left > 740)
    {
        m_rect.setPosition(m_platformPosition.x - (m_rect.getGlobalBounds().left + m_rect.getGlobalBounds().width - 740) - (rand()%200), m_platformPosition.y);
    }

    m_platforms.push_back(m_rect);


}

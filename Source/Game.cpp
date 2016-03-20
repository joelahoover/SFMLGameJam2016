#include "Game.h"
#include <iostream>
#include <string>

const sf::Time Game::timePerFrame = sf::seconds(1.0f / 60.0f);

Game::Game() : m_window(sf::VideoMode(800, 900), "Orange guy jumping on the cloud then fall ?? XD"), m_gravity(0.2), m_maxY(50)
{
    m_floorNumber = 0;
    m_currentFloor = 0;

    if(!m_themeMusicB.loadFromFile("data/audio/theme.wav"))
        std::cout << "Error loading theme sound " << std::endl;

    m_jumpB.loadFromFile("data/audio/jump.wav");
    m_failB.loadFromFile("data/audio/fail.wav");
    m_bounceB.loadFromFile("data/audio/bounce.wav");

    //m_themeMusic.setBuffer(m_themeMusicB);
    m_themeMusic.openFromFile("data/audio/theme.wav");
    m_themeMusic.setVolume(70);
    m_themeMusic.play();

    m_jump.setBuffer(m_jumpB);
    m_fail.setBuffer(m_failB);
    m_bounce.setBuffer(m_bounceB);
    m_bounce.setVolume(80);
    m_themeMusic.setLoop(true);

    m_wallText.loadFromFile("data/wall2.png");
    m_floorText.loadFromFile("data/cloud_l.png");

    m_lWall.setTexture(m_wallText);
    m_rWall.setTexture(m_wallText);
    m_lWall1.setTexture(m_wallText);
    m_rWall1.setTexture(m_wallText);

    m_floor.setTexture(m_floorText);

    m_window.setFramerateLimit(60);
    m_view.setSize(800,900);
    m_view.setCenter(400,450);

    m_floor.setScale(2, 1);
    m_floorRect = sf::FloatRect(0, 870, 800, 50);

    m_lWall.setPosition(0, 500);
    m_rWall.setPosition(750, 500);
    m_lWall1.setPosition(0, -(m_lWall1.getGlobalBounds().height) + 500);
    m_rWall1.setPosition(750, -(m_rWall1.getGlobalBounds().height) + 500);
    m_floor.setPosition(35, 850);

    m_vel = sf::Vector2f(0, 0);
    m_playerSpeed = 600;
    m_attempt = 0;

    m_moveCamera = false;
    m_cameraSpeed = 200;
    m_paused = false;
    m_lost = false;

    m_lostScreen.setFillColor(sf::Color(20, 180, 180, 150));
    m_lostScreen.setSize(sf::Vector2f(500, 500));
    m_lostScreen.setOrigin(m_lostScreen.getGlobalBounds().width / 2.0f, m_lostScreen.getGlobalBounds().height / 2.0f);

    m_playerBody = sf::FloatRect(0, 0, 35, 30);

    m_points = 0;
    m_sat = 0;

    m_font.loadFromFile("data/arial.ttf");
    m_time.setFont(m_font);
    m_score.setFont(m_font);
    m_level.setFont(m_font);
    m_message.setFont(m_font);

    m_time.setColor(sf::Color::Black);
    m_score.setColor(sf::Color::Black);
    m_level.setColor(sf::Color::Black);
    m_message.setColor(sf::Color::Black);

    m_time.setString("Time: " + std::to_string(m_sat));
    m_score.setString("Score: " + std::to_string(m_points));
    m_level.setString("Floor: " + std::to_string(m_floorNumber));
    m_message.setString("You Failed!  Press R to restart.");
    m_increase = false;

    m_splashT.loadFromFile("data/splash.png");
    m_splash.setTexture(m_splashT);
    m_splash.setPosition(0,0);
    m_start = false;

    m_splashBG.setFillColor(sf::Color(30, 100, 80, 250));

    m_splashText.setFont(m_font);
    m_splashText.setCharacterSize(40);
    m_splashText.setColor(sf::Color(10, 255, 80));
    m_splashText.setString("Press Enter to play! \n\nCoding: Godsend72 \nArt: Anh Nguyen \nSound: Ba Thanh Nguyen");

    m_splashText.setPosition(m_view.getCenter().x - m_splashText.getGlobalBounds().width / 2.0f, m_view.getCenter().y - m_splashText.getGlobalBounds().height / 2.0f);
    m_splashBG.setPosition(m_splashText.getGlobalBounds().left - 10, m_splashText.getGlobalBounds().top - 10);
    m_splashBG.setSize(sf::Vector2f(m_splashText.getGlobalBounds().width + 20, m_splashText.getGlobalBounds().height + 20));
}

void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while(m_window.isOpen())
    {
        handleEvents();
        m_timer += clock.getElapsedTime();
        timeSinceLastUpdate += clock.restart();
        while(timeSinceLastUpdate > timePerFrame)
        {
            timeSinceLastUpdate -= timePerFrame;
            handleEvents();
            if(!m_paused && m_start)
                update(timePerFrame);

        }
        draw();
    }
}

void Game::handleEvents()
{
    sf::Event event;
    while(m_window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            m_window.close();

        if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        {
            m_window.close();
        }
        if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R && m_lost)
        {
            restart();
        }
        if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return && !m_start)
        {
            m_start = true;
            restart();
        }
    }
}

void Game::update(sf::Time dt)
{
    m_playerBody.left = m_player.getBounds().left + 15;
    m_playerBody.top = m_player.getBounds().top + 100;
    m_player.update(dt.asSeconds());
    m_lostScreen.setPosition(m_view.getCenter());

    //Jump
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        m_player.jump();
        if(m_jump.getStatus() != sf::Sound::Playing && m_player.isJumping())
        {
            m_platforms.createPlatform(m_platforms.getPlatforms()->back().getPosition().y - 150);
            m_jump.play();
        }
    }
    //Movement
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        m_player.pDirection = pDir::LEFT;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        m_player.pDirection = pDir::RIGHT;
    }
    else
    {
        m_player.pDirection = pDir::NONE;
    }

    //If we fall on the floor
    if(m_player.getBounds().intersects(m_floorRect))
    {
        m_player.setPosition(m_player.getBounds().left, m_floorRect.top - m_player.getBounds().height);
        //Player on ground and on floor
        m_player.setStatus(true, true);

    }
    //If we collide with the walls

    if(m_player.getBounds().intersects(m_lWall.getGlobalBounds()) || m_player.getBounds().intersects(m_lWall1.getGlobalBounds()))
    {
        if(m_player.isJumping())
            m_bounce.play();
        m_player.bounce(sf::Vector2f(2, -m_playerSpeed * dt.asSeconds()), dt.asSeconds());
    }
    if(m_player.getBounds().intersects(m_rWall.getGlobalBounds()) || m_player.getBounds().intersects(m_rWall1.getGlobalBounds()))
    {
        if(m_player.isJumping())
            m_bounce.play();
        m_player.bounce(sf::Vector2f(-2, m_playerSpeed * dt.asSeconds()), dt.asSeconds());
    }

    int platLevel = 0;
    bool plaformCheck = false;
    sf::Vector2f platPos;
    //Check if player is on platforms, handle it
    for(auto& platform : *m_platforms.getPlatforms())
    {
        //Get only upper one pixel of platform
        sf::FloatRect tempPlatform = sf::FloatRect(platform.getGlobalBounds().left, platform.getGlobalBounds().top + 15, platform.getGlobalBounds().width, 15);
        //If we land on a platform when we jump
        if(tempPlatform.intersects(sf::FloatRect(m_player.getBounds().left,
                                                                m_player.getBounds().top + m_player.getBounds().height,
                                                                m_player.getBounds().width, 1)))
        {
            if(m_attempt < 2)
            {
                plaformCheck = true;
                platPos = platform.getPosition();


                platLevel = platform.getPosition().y / 150 - 6;
                m_currentFloor = std::abs(platLevel);


                if(m_floorNumber < std::abs(platLevel))
                {
                    //Add points for every platform you jump
                    if(std::abs(platLevel) - m_floorNumber > 1)
                        m_points += 150;
                    else
                        m_points += 50;


                    //Every 30 seconds increase speed by 20
                    if(m_sat % 30 == 0 && m_increase)
                    {
                        m_cameraSpeed += 20;
                        std::cout << "increase" << std::endl;
                        m_increase = false;
                    }
                    if(m_sat % 31 == 0 || m_sat % 7 == 0 || m_sat % 61 == 0 || m_sat % 91 == 0 || m_sat % 121 == 0 || m_sat % 151 == 0 || m_sat % 181 == 0 || m_sat % 211 == 0)
                    {
                        m_increase = true;
                    }

                    //std::cout << "Current: " << m_currentFloor << std::endl;
                    //std::cout << "Max floor: " << m_floorNumber << std::endl;

                    //delete platforms
                    if(m_attempt == 1)
                    {
                        m_platforms.getPlatforms()->erase(m_platforms.getPlatforms()->begin(), m_platforms.getPlatforms()->begin() + (std::abs(platLevel) - m_floorNumber));
                    }
                    m_floorNumber = std::abs(platLevel);

                }
            }
        }
        if(tempPlatform.intersects(m_playerBody) && m_attempt >= 2)
        {

            m_paused = true;
            m_fail.play();
            m_lost = true;
        }

    }
    if(plaformCheck)
    {
        if(m_player.isFalling())
        {
            m_player.setStatus(true, false);
            //set the position of the player on top of the platform
            m_player.setPosition(m_player.getBounds().left, platPos.y + 15 - m_player.getBounds().height);

        }
    }
    else
    {
        //If we walk of the platform
        m_player.setStatus(false, false);
    }







    if(m_floorNumber - m_currentFloor > 2)
    {
        if(m_attempt == 0)
        {
            //Delete all platforms bellow the player except the oe he is standing on
            int tempFloor = m_currentFloor -1;

            m_floorNumber = m_currentFloor;

            m_platforms.getPlatforms()->erase(m_platforms.getPlatforms()->begin(), m_platforms.getPlatforms()->begin() + tempFloor);
            m_fail.play();
            m_attempt = 1;
            m_view.setCenter(400, m_player.getBounds().top);
            m_floor.setPosition(1000, 0);
            m_floorRect.left = 1000;
        }

    }
    else if(m_player.getBounds().top > m_view.getCenter().y + 500 && m_currentFloor > 2)
    {
        if(m_attempt == 0)
        {
            m_cameraSpeed = 200;
            //Delete all platforms bellow the player except the oe he is standing on
            int tempFl = m_player.getBounds().top / 150 - 6;
            m_currentFloor = std::abs(tempFl);

            int tempFloor = m_currentFloor -1;

            m_floorNumber = m_currentFloor;

            m_platforms.getPlatforms()->erase(m_platforms.getPlatforms()->begin(), m_platforms.getPlatforms()->begin() + tempFloor);

            m_player.setPosition(m_player.getBounds().left, m_platforms.getPlatforms()->at(0).getPosition().y - m_player.getBounds().height);
            m_platforms.getPlatforms()->at(0).setPosition(m_player.getBounds().left - m_platforms.getPlatforms()->at(0).getGlobalBounds().width / 2.0f, m_platforms.getPlatforms()->at(0).getPosition().y);

            m_platforms.getPlatforms()->at(0).setTexture(m_platforms.m_large, true);

            if(m_platforms.getPlatforms()->at(0).getPosition().x + m_platforms.getPlatforms()->at(0).getGlobalBounds().left > 740)
            {
                m_platforms.getPlatforms()->at(0).setPosition(m_platforms.getPlatforms()->at(0).getPosition().x -
                                    (m_platforms.getPlatforms()->at(0).getGlobalBounds().left + m_platforms.getPlatforms()->at(0).getGlobalBounds().width - 740), m_platforms.getPlatforms()->at(0).getPosition().y);
            }

            m_fail.play();
            m_attempt = 1;
            m_view.setCenter(400, m_player.getBounds().top);
            m_floor.setPosition(1000, 0);
            m_floorRect.left = 1000;
        }
    }

    //Create spikes
    if(m_attempt == 1 && (m_player.getBounds().top > m_platforms.getPlatforms()->at(0).getPosition().y + 50) || m_player.getBounds().top > m_view.getCenter().y + 500)
    {
        m_player.setFailure(1);
        m_fail.play();
        m_attempt = 2;
        m_platforms.getPlatforms()->clear();

        int tempPos = m_player.getBounds().top + 1000;

        for(int i = 0; i < 700; ++i)
        {
            m_platforms.createPlatform(tempPos, 1);
            tempPos += 200;
        }
    }

    //If we are falling add points for every 10 miliseconds
    if(m_attempt == 2)
    {
        if(m_timer.asMicroseconds() % 10 == 0)
        {
            m_points += 5;
        }
    }


    if(m_player.getBounds().top <= m_lWall.getGlobalBounds().top + m_lWall.getGlobalBounds().height / 2.0f)
    {
        m_lWall1.setPosition(m_lWall1.getGlobalBounds().left, m_lWall.getGlobalBounds().top - m_lWall1.getGlobalBounds().height);
        m_rWall1.setPosition(m_rWall1.getGlobalBounds().left, m_rWall.getGlobalBounds().top - m_rWall1.getGlobalBounds().height);
    }
    if(m_player.getBounds().top <= m_lWall1.getGlobalBounds().top + m_lWall1.getGlobalBounds().height / 2.0f)
    {
        m_lWall.setPosition(m_lWall.getGlobalBounds().left, m_lWall1.getGlobalBounds().top - m_lWall.getGlobalBounds().height);
        m_rWall.setPosition(m_rWall.getGlobalBounds().left, m_rWall1.getGlobalBounds().top - m_rWall.getGlobalBounds().height);
    }

    if(m_player.getBounds().top < 0 && !m_moveCamera)
    {
        m_moveCamera = true;
    }

    //camera auto movement
    if(m_moveCamera)
    {
        m_view.move(0, dt.asSeconds() * -50);
    }
    else
    {
        m_view.setCenter(400, m_player.getBounds().top);
    }

    //Camera moving
    if(m_player.getBounds().top < 0)
    {
        m_view.move(0, -m_cameraSpeed * dt.asSeconds());
    }
    if(m_player.getBounds().top < m_view.getCenter().y - 300 )
    {
        //Boost points
        if(m_timer.asMilliseconds() % 10 == 0)
        m_points += 15;
        m_view.move(0,-4);
    }

    //If we are falling
    if(m_attempt == 2)
    {
        m_view.setCenter(400, m_player.getBounds().top + 300);
        if(m_player.getBounds().top >= m_lWall.getGlobalBounds().top + m_lWall.getGlobalBounds().height / 2.0f)
        {
            m_lWall1.setPosition(m_lWall1.getGlobalBounds().left, m_lWall.getGlobalBounds().top + m_lWall.getGlobalBounds().height);
            m_rWall1.setPosition(m_rWall1.getGlobalBounds().left, m_rWall.getGlobalBounds().top + m_rWall.getGlobalBounds().height);
        }
        if(m_player.getBounds().top >= m_lWall1.getGlobalBounds().top + m_lWall1.getGlobalBounds().height / 2.0f)
        {
            m_lWall.setPosition(m_lWall.getGlobalBounds().left, m_lWall1.getGlobalBounds().top + m_lWall1.getGlobalBounds().height);
            m_rWall.setPosition(m_rWall.getGlobalBounds().left, m_rWall1.getGlobalBounds().top + m_rWall1.getGlobalBounds().height);
        }

    }

    m_sat = m_timer.asSeconds();
    //if(m_sat / 60 > 0)
    m_time.setString("Time: " + std::to_string(m_sat / 60) + ":" + std::to_string((m_sat % 60) / 10) + std::to_string((m_sat % 60) % 10));
    //else
      //  m_time.setString("Time: 0:" + std::to_string(m_sat / 10) + std::to_string(m_sat % 10));



    m_score.setString("Score: " + std::to_string(m_points));

    m_level.setString("Floor: " + std::to_string(m_floorNumber));


    m_time.setPosition(m_view.getCenter().x - m_time.getGlobalBounds().width / 2.0f, m_view.getCenter().y - 450);
    m_score.setPosition(m_view.getCenter().x - 330, m_view.getCenter().y - 450);
    m_level.setPosition(m_view.getCenter().x - 330, m_view.getCenter().y - 420);
}



void Game::draw()
{
    m_window.setView(m_view);
    if(m_attempt == 0)
        m_window.clear(sf::Color(30,224,250));
    else
        m_window.clear(sf::Color(250,30,111));


    m_window.draw(m_lWall);
    m_window.draw(m_rWall);
    m_window.draw(m_lWall1);
    m_window.draw(m_rWall1);

    m_window.draw(m_floor);


    m_platforms.draw(m_window);
    m_player.draw(m_window);


    if(m_lost)
    {
        m_window.draw(m_lostScreen);
        m_window.draw(m_message);
        m_message.setPosition(m_lostScreen.getPosition().x - 200, m_lostScreen.getPosition().y - 200);
        m_score.setPosition(m_lostScreen.getPosition().x - 200, m_lostScreen.getPosition().y - 100 );
        m_time.setPosition(m_lostScreen.getPosition().x - 200, m_lostScreen.getPosition().y - 50);
        m_level.setPosition(m_lostScreen.getPosition().x - 200, m_lostScreen.getPosition().y);
    }

    //draw the gui
    m_window.draw(m_time);
    m_window.draw(m_level);
    m_window.draw(m_score);

    if(!m_start)
    {
        m_window.draw(m_splash);
        m_window.draw(m_splashBG);
        m_window.draw(m_splashText);
    }
    m_window.display();
}



void Game::restart()
{
    m_lWall.setPosition(0, 500);
    m_rWall.setPosition(750, 500);
    m_lWall1.setPosition(0, -(m_lWall1.getGlobalBounds().height) + 500);
    m_rWall1.setPosition(750, -(m_rWall1.getGlobalBounds().height) + 500);

    m_floor.setPosition(35, 850);
    m_floorRect = sf::FloatRect(0, 870, 800, 50);

    m_view.setCenter(m_view.getCenter().x, 400);
    m_floorNumber = 0;
    m_currentFloor = 0;

    m_platforms.getPlatforms()->clear();

    int tempNum = 700;
    for(int i = 0; i < 1000; ++i)
    {
        m_platforms.createPlatform(tempNum);
        tempNum -= 150;
    }



    m_player.setFailure(0);
    m_playerSpeed = 600;
    m_attempt = 0;

    m_moveCamera = false;
    m_lost = false;
    m_paused = false;

    m_points = 0;
    m_sat = 0;
    m_timer = sf::Time::Zero;
}


















#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;

constexpr unsigned BALL_SIZE = 30;
constexpr size_t BALL_NUM = 5;

struct Circle
{
    sf::Vector2f position;
    sf::Vector2f speed;
    sf::CircleShape ball;
};

void init(Circle (&circles)[BALL_NUM])
{
    for(size_t i = 0; i < std::size(circles); ++i)
    {
        circles[i].ball.setRadius(BALL_SIZE);
        
    }
    
    circles[0].ball.setPosition(70, 500);
    circles[1].ball.setPosition(700, 200);
    circles[2].ball.setPosition(230, 100);
    circles[3].ball.setPosition(390,480);
    circles[4].ball.setPosition(100, 400);
    
    circles[0].ball.setFillColor(sf::Color(244, 164, 96));
    circles[1].ball.setFillColor(sf::Color(147, 112, 219));
    circles[2].ball.setFillColor(sf::Color(30, 144, 255));
    circles[3].ball.setFillColor(sf::Color(0, 255, 127));
    circles[4].ball.setFillColor(sf::Color(220, 20, 60));

    circles[0].speed = {100.f, 1.f};
    circles[1].speed = {50.f, 8.f};
    circles[2].speed = {5.f, 200.f};
    circles[3].speed = {50.f, 50.f};
    circles[4].speed = {65.f, 300.f};
}

void update(Circle (&circles)[BALL_NUM], sf::Clock& clock) 
{
    float deltaTime = clock.restart().asSeconds();
    for(size_t i = 0; i < std::size(circles); ++i)
    {
        if ((circles[i].ball.getPosition().x + 2 * BALL_SIZE >= WINDOW_WIDTH) && (circles[i].speed.x > 0))
        {
            circles[i].speed.x = -circles[i].speed.x;
        }
        if ((circles[i].ball.getPosition().x < 0) && (circles[i].speed.x < 0))
        {
            circles[i].speed.x = -circles[i].speed.x;
        }
        if ((circles[i].ball.getPosition().y + 2 * BALL_SIZE >= WINDOW_HEIGHT) && (circles[i].speed.y > 0))
        {
            circles[i].speed.y = -circles[i].speed.y;
        }
        if ((circles[i].ball.getPosition().y < 0) && (circles[i].speed.y < 0))
        {
            circles[i].speed.y = -circles[i].speed.y;
        }
        circles[i].ball.setPosition(circles[i].ball.getPosition() + circles[i].speed * deltaTime);
    } 
}

void pollEvent(sf::RenderWindow &window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        default:
            break;
        }
    }  
}

void redrawFrame(sf::RenderWindow &window, Circle (&circles)[BALL_NUM])
{
    window.clear();
    for(size_t i = 0; i < std::size(circles); ++i)
    {
      window.draw(circles[i].ball);
    }
    window.display();
}

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT}), "Balls",
        sf::Style::Default, settings);
    
    sf::Clock clock;
    Circle circles[BALL_NUM];

    init(circles);
    while (window.isOpen())
    {
        pollEvent(window);
        update(circles, clock);
        redrawFrame(window, circles);
    }
}

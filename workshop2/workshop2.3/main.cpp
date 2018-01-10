#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>
#include <ctime>
#include <cassert>

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

struct PRNG
{
    unsigned seed = 0;
};

void initGenerator(PRNG& generator)
{
    // Получаем случайное зерно последовательности
    generator.seed = unsigned(std::time(nullptr));
}

// Генерирует число на отрезке [minValue, maxValue].
unsigned random(PRNG& generator, unsigned minValue, unsigned maxValue)
{
    // Проверяем корректность аргументов
    assert(minValue < maxValue);
    // Итеративно изменяем текущее число в генераторе
    generator.seed = (generator.seed * 73129 + 95121);
    
    // Приводим число к отрезку [minValue, maxValue]
    return (generator.seed % (maxValue + 1 - minValue)) + minValue;
}


void init(Circle (&circles)[BALL_NUM], PRNG& generator)
{
    for(size_t i = 0; i < std::size(circles); ++i)
    {
        circles[i].ball.setRadius(BALL_SIZE);
        circles[i].ball.setOrigin(BALL_SIZE, BALL_SIZE);
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

    for(size_t i = 0; i < std::size(circles); ++i)
    {
        float speedX = random(generator, 0.f, 700.f);
        float speedY = random(generator, 0.f, 700.f);
        circles[i].speed = {speedX,speedY};
    }
}

void checkImpact(Circle (&circles)[BALL_NUM])
{
    for (size_t fi = 0; fi <  std::size(circles); ++fi)
    {
        for (size_t si = fi + 1; si < std::size(circles); ++si)
        {
            sf::Vector2f deltaPos = circles[fi].ball.getPosition() - circles[si].ball.getPosition();
            float distance = std::hypot(deltaPos.x, deltaPos.y);
            // проверяем столкновение fi, si
            if(distance <= 2 * BALL_SIZE)
            {
                sf::Vector2f deltaSpeed = circles[fi].speed - circles[si].speed;
                circles[fi].speed = circles[fi].speed - (deltaSpeed.x * deltaPos.x + deltaSpeed.y * deltaPos.y) / (distance * distance) * deltaPos;
                deltaPos = -deltaPos;
                deltaSpeed = -deltaSpeed;
                circles[si].speed = circles[si].speed - (deltaSpeed.x * deltaPos.x + deltaSpeed.y * deltaPos.y) / (distance * distance) * deltaPos;
            }

        }
    }
}

void update(Circle (&circles)[BALL_NUM], sf::Clock& clock) 
{
    checkImpact(circles);
    float deltaTime = clock.restart().asSeconds();
    for(size_t i = 0; i < std::size(circles); ++i)
    {
        if ((circles[i].ball.getPosition().x + BALL_SIZE >= WINDOW_WIDTH) && (circles[i].speed.x > 0))
        {
            circles[i].speed.x = -circles[i].speed.x;
        }
        if ((circles[i].ball.getPosition().x - BALL_SIZE < 0) && (circles[i].speed.x < 0))
        {
            circles[i].speed.x = -circles[i].speed.x;
        }
        if ((circles[i].ball.getPosition().y +  BALL_SIZE >= WINDOW_HEIGHT) && (circles[i].speed.y > 0))
        {
            circles[i].speed.y = -circles[i].speed.y;
        }
        if ((circles[i].ball.getPosition().y - BALL_SIZE < 0) && (circles[i].speed.y < 0))
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
    PRNG generator;
    initGenerator(generator);
    
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT}), "Balls",
        sf::Style::Default, settings);
    
    sf::Clock clock;
    Circle circles[BALL_NUM];

    init(circles, generator);
    while (window.isOpen())
    {
        pollEvent(window);
        update(circles, clock);
        redrawFrame(window, circles);
    }
}

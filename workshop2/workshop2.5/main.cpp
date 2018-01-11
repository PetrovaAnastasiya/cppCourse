#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>
#include <ctime>
#include <cassert>
#include <random>

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
    std::mt19937 engine;
};

void initGenerator(PRNG &generator)
{
    // РџРѕР»СѓС‡Р°РµРј СЃР»СѓС‡Р°Р№РЅРѕРµ Р·РµСЂРЅРѕ РїРѕСЃР»РµРґРѕРІР°С‚РµР»СЊРЅРѕСЃС‚Рё
    generator.seed = unsigned(std::time(nullptr));
}

// Р“РµРЅРµСЂРёСЂСѓРµС‚ С‡РёСЃР»Рѕ РЅР° РѕС‚СЂРµР·РєРµ [minValue, maxValue].
unsigned random(PRNG &generator, unsigned minValue, unsigned maxValue)
{
    // РџСЂРѕРІРµСЂСЏРµРј РєРѕСЂСЂРµРєС‚РЅРѕСЃС‚СЊ Р°СЂРіСѓРјРµРЅС‚РѕРІ
    assert(minValue < maxValue);
    // РС‚РµСЂР°С‚РёРІРЅРѕ РёР·РјРµРЅСЏРµРј С‚РµРєСѓС‰РµРµ С‡РёСЃР»Рѕ РІ РіРµРЅРµСЂР°С‚РѕСЂРµ
    generator.seed = (generator.seed * 73129 + 95121);

    // РџСЂРёРІРѕРґРёРј С‡РёСЃР»Рѕ Рє РѕС‚СЂРµР·РєСѓ [minValue, maxValue]
    return (generator.seed % (maxValue + 1 - minValue)) + minValue;
}

void initGeneratorColor(PRNG &generator)
{
    // РСЃРїРѕР»СЊР·СѓРµРј РІСЂРµРјСЏ СЃ 1 СЏРЅРІР°СЂСЏ 1970 РіРѕРґР° РІ СЃРµРєСѓРЅРґР°С… РєР°Рє СЃР»СѓС‡Р°Р№РЅРѕРµ Р·РµСЂРЅРѕ
    const unsigned seed = unsigned(std::time(nullptr));
    generator.engine.seed(seed);
}

// Р“РµРЅРµСЂРёСЂСѓРµС‚ РёРЅРґРµРєСЃ РІ РґРёР°РїР°Р·РѕРЅРµ [0, size)
size_t getRandomIndex(PRNG &generator, size_t size)
{
    // РЎРѕР·РґР°С‘Рј СЂР°СЃРїСЂРµРґРµР»РµРЅРёРµ
    std::uniform_int_distribution<size_t> distribution(0, size - 1);

    // Р’С‹С‡РёСЃР»СЏРµРј РїСЃРµРІРґРѕСЃР»СѓС‡Р°Р№РЅРѕРµ С‡РёСЃР»Рѕ: РІС‹Р·РѕРІРµРј СЂР°СЃРїСЂРµРґРµР»РµРЅРёРµ РєР°Рє С„СѓРЅРєС†РёСЋ,
    //  РїРµСЂРµРґР°РІ РіРµРЅРµСЂР°С‚РѕСЂ РїСЂРѕРёР·РІРѕР»СЊРЅС‹С… С†РµР»С‹С… С‡РёСЃРµР» РєР°Рє Р°СЂРіСѓРјРµРЅС‚.
    return distribution(generator.engine);
}
void pollEvent(sf::RenderWindow &window, int &flag, sf::Vector2f &mousePosition)
{
    flag = 0;     
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::MouseButtonPressed:
             mousePosition = {float(event.mouseButton.x), float(event.mouseButton.y)};
             flag = 1;             
             break;
        default:
            break;
        }
    }
}

void init(Circle &circles, PRNG &generator, sf::Vector2f &mousePosition)
{
    sf::Color color[8] = {{0, 0, 204}, {0, 255, 153}, {51, 0, 51}, {255, 255, 0}, {153, 204, 255}, {102, 255, 0}, {153, 0, 0}, {204, 255, 153}};

    int divider = 2;
    const size_t colorFirst = getRandomIndex(generator, std::size(color));
    const size_t colorSecond = getRandomIndex(generator, std::size(color));

    sf::Color colorRandom((color[colorFirst].r + color[colorSecond].r) / divider, (color[colorFirst].g + color[colorSecond].g) / divider, (color[colorFirst].b + color[colorSecond].b) / divider);
    float speedX = random(generator, 0.f, 300.f);
    float speedY = random(generator, 0.f, 300.f);
    circles.speed = {speedX, speedY};
    circles.ball.setFillColor(sf::Color(colorRandom));
    circles.ball.setPosition(mousePosition);
    circles.ball.setRadius(BALL_SIZE);
    circles.ball.setOrigin(BALL_SIZE, BALL_SIZE);

}


void checkImpact(std::vector<Circle>(&circles))
{
    for (size_t fi = 0; fi < circles.size(); ++fi)
    {
        for (size_t si = fi + 1; si < circles.size(); ++si)
        {
            sf::Vector2f deltaPos = circles[fi].ball.getPosition() - circles[si].ball.getPosition();
            float distance = std::hypot(deltaPos.x, deltaPos.y);
            // РїСЂРѕРІРµСЂСЏРµРј СЃС‚РѕР»РєРЅРѕРІРµРЅРёРµ fi, si
            if (distance <= 2 * BALL_SIZE)
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

void update(std::vector<Circle>(&circles), sf::Clock &clock)
{
    checkImpact(circles);
    float deltaTime = clock.restart().asSeconds();
    for (size_t i = 0; i < circles.size(); ++i)
    {
        if ((circles[i].ball.getPosition().x + BALL_SIZE >= WINDOW_WIDTH) && (circles[i].speed.x > 0))
        {
            circles[i].speed.x = -circles[i].speed.x;
        }
        if ((circles[i].ball.getPosition().x - BALL_SIZE < 0) && (circles[i].speed.x < 0))
        {
            circles[i].speed.x = -circles[i].speed.x;
        }
        if ((circles[i].ball.getPosition().y + BALL_SIZE >= WINDOW_HEIGHT) && (circles[i].speed.y > 0))
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


void redrawFrame(sf::RenderWindow &window, std::vector<Circle>(&circles))
{
    window.clear(sf::Color(234, 234, 234));
    for (size_t i = 0; i < circles.size(); ++i)
    {
        window.draw(circles[i].ball);
    }
    std::cout << circles.size() << std::endl;
    window.display();
}

void doCheck(sf::Vector2f &mousePosition, std::vector<Circle>(&circles), int &flag, PRNG &generator)
{
    int flagToCreate = 0;
    for (size_t i = 0; i < circles.size(); ++i) 
    {
        sf::Vector2f deltaPosition = mousePosition - circles[i].ball.getPosition();
        float distance = std::hypot(deltaPosition.x, deltaPosition.y);
         
        if(distance < 2 * BALL_SIZE)
        {
            flagToCreate = 1;
        }
    }
    if (flagToCreate == 0)
    {
        Circle ball;
        init(ball, generator, mousePosition);
        circles.push_back(ball);
    }
}

int main()
{
    PRNG generator;
    initGenerator(generator);
    initGeneratorColor(generator);
    int flag = 0;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Balls",
        sf::Style::Default, settings);

    sf::Clock clock;
    std::vector<Circle> circles;
    sf::Vector2f mousePosition;

    while (window.isOpen())
    {
        flag = 0;
        pollEvent(window, flag, mousePosition);
        if(flag == 1) 
        {
            doCheck(mousePosition, circles, flag, generator);
        }
        update(circles, clock);
        redrawFrame(window, circles);
    }
}
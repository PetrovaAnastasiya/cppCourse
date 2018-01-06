#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <algorithm>

float toDegrees(float radians)
{
    return float(double(radians) * 180.0 / M_PI);
}

void onMouseMove(const sf::Event::MouseMoveEvent& event, sf::Vector2f& mousePosition)
{
    std::cout << "mouse x=" << event.x << ", y=" << event.y << std::endl;
    mousePosition = {float(event.x), float(event.y)};
}

void init(sf::ConvexShape& pointer)
{
    pointer.setPointCount(3);
    pointer.setPoint(0, {40,0});
    pointer.setPoint(1, {-20,-20});
    pointer.setPoint(2, {-20,20});
    pointer.setPosition({400, 300});
    pointer.setFillColor (sf::Color(0xFF,0x80,0x00));
}

void pollEvent(sf::RenderWindow& window, sf::Vector2f& mousePosition)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::MouseMoved:
            onMouseMove(event.mouseMove, mousePosition);
            break;
        default:
            break;
                
        }
    }
}

void update(const sf::Vector2f& mousePosition, sf::Clock& clock, sf::ConvexShape& pointer)
{
    const sf::Vector2f mousePositionDelta = mousePosition - pointer.getPosition();
    float angleMouseDegrees = toDegrees(atan2(mousePositionDelta.y, mousePositionDelta.x));
    float deltaTime = clock.restart().asSeconds();
    const float turnSpeed = 15.0 * deltaTime;
    float anglePointerDegrees = pointer.getRotation();
    if (angleMouseDegrees < 0)
    {
        angleMouseDegrees = angleMouseDegrees + 360;
    } 
    float realAngleDegrees = 0;
    float deltaAngleDegrees = angleMouseDegrees - anglePointerDegrees; 
    if(deltaAngleDegrees >= 0 && deltaAngleDegrees <= 180)
    {
        realAngleDegrees = anglePointerDegrees + std::min(turnSpeed, deltaAngleDegrees);
    }
    if(deltaAngleDegrees >= 0 && deltaAngleDegrees > 180)
    {
        realAngleDegrees = anglePointerDegrees - std::min(turnSpeed, deltaAngleDegrees);
    }
    if(deltaAngleDegrees <= 0 && deltaAngleDegrees >= -180)
    {
        realAngleDegrees = anglePointerDegrees - std::min(turnSpeed, std::abs(deltaAngleDegrees));
    }
    if(deltaAngleDegrees <= 0 && deltaAngleDegrees < -180)
    {
        realAngleDegrees = anglePointerDegrees + std::min(turnSpeed, std::abs(deltaAngleDegrees));
    }
    //std::cout << "angleMouseDegrees =" << angleMouseDegrees << ", anglePointerDegrees=" << anglePointerDegrees<< std::endl;
    pointer.setRotation(realAngleDegrees);
}

void redrawFrame(sf::RenderWindow& window, sf::ConvexShape& pointer)
{
    window.clear();
    window.draw(pointer);
    window.display();
}

int main()
{
    constexpr unsigned WINDOW_WIDTH = 800;
    constexpr unsigned WINDOW_HEIGHT = 600;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT}),
        "Pointer follows mouse", sf::Style::Default, settings);
    sf::ConvexShape pointer;
    sf::Vector2f mousePosition;
    sf::Clock clock;
    init(pointer);
    while (window.isOpen())
    {
        pollEvent(window, mousePosition);
        update(mousePosition, clock, pointer);
        redrawFrame(window, pointer);
    }
}
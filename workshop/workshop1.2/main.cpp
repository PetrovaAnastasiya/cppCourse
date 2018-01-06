#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <algorithm>
//Перевод радиан в градусы
float toDegrees(float radians)
{
    return float(double(radians) * 180.0 / M_PI);
}
//Вывод координат мыши
void onMouseMove(const sf::Event::MouseMoveEvent& event, sf::Vector2f& mousePosition)
{
    std::cout << "mouse x=" << event.x << ", y=" << event.y << std::endl;
    //Обновление позиции мыши
    mousePosition = {float(event.x), float(event.y)};
}
//Инициализация указателя (позиция, цвет)
void init (sf::ConvexShape &pointer)
{
    //Позиция указателя
    pointer.setPosition({400, 320});
    //Форма указателя
    pointer.setPointCount(7);
    pointer.setPoint(0,{0,-25});
    pointer.setPoint(1,{0,-50});
    pointer.setPoint(2,{50, 0});
    pointer.setPoint(3,{0, 50});
    pointer.setPoint(4,{0, 25});
    pointer.setPoint(5,{-50,25});
    pointer.setPoint(6,{-50,-25});
    //Цвет указателя
    pointer.setFillColor(sf::Color(0xFF,0xFF, 0x00));
    //Цвет контура указателя
    pointer.setOutlineColor(sf::Color(0x00,0x00,0x00));
    //Толщина указателя
    pointer.setOutlineThickness(2);
}    
//Обработка событий (закрытие окна, движение мыши)
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
//Поворот и движение стрелки
void update(const sf::Vector2f& mousePosition, sf::Clock& clock, sf::ConvexShape& pointer)
{
    //Поворот мыши вслед за стрелкой
    const sf::Vector2f mousePositionDelta = mousePosition - pointer.getPosition();
    float angleMouseDegrees = toDegrees(atan2(mousePositionDelta.y, mousePositionDelta.x));
    float deltaTime = clock.restart().asSeconds();
    const float turnSpeed = 90.0 * deltaTime;
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
    //Установка поворота указателя
    pointer.setRotation(realAngleDegrees);
    //Обновление позиции указателя
    float distancePointerMouse = std::hypot(mousePositionDelta.x, mousePositionDelta.y);
    sf::Vector2f direction = {mousePositionDelta.x / distancePointerMouse, mousePositionDelta.x / distancePointerMouse};
    float speedMotionMax = 20.0;
    float speedMotion= speedMotionMax * deltaTime;
    pointer.setPosition(pointer.getPosition() + direction * speedMotion);
    
}
//Отрисовка указателя 
void redrawFrame(sf::RenderWindow& window, sf::ConvexShape& pointer)
{
    window.clear(sf::Color(0xFF,0xFF,0xFF));
    window.draw(pointer);
    window.display();
}

int main()
{ 

    constexpr unsigned WINDOW_WIDTH = 800;
    constexpr unsigned WINDOW_HEIGHT = 600;
    //Инициализация окна
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT}),
        "Pointer follows mouse", sf::Style::Default, settings);
    //Объявление указателя
    sf::ConvexShape pointer;
    //Объявление позиции мыши
    sf::Vector2f mousePosition;
    //Объявление часов
    sf::Clock clock;
    //Инициализация указателя(вызов функции)
    init(pointer);
    //Основной цикл
    while (window.isOpen())
    {
       //Обработка событий (закрытие окна, движение мыши)(вызов функции)
        pollEvent(window, mousePosition);
        //Поворот и движение стрелки (вызов функции)
        update(mousePosition, clock, pointer);
        //Отрисовка указателя (вызов функции)
        redrawFrame(window, pointer);
    }
}
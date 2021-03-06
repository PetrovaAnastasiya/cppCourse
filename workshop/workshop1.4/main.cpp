#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cmath>

//Инициализация кота и точки
void init(sf::Texture &texture,sf::Texture &textureRedPoint, sf::Sprite &cat, sf::Sprite &redPoint)
{
    texture.loadFromFile("cat.png", sf::IntRect(0, 0, 38, 35));
    cat.setTexture(texture);
    cat.setPosition(350,250);
    cat.setOrigin(18, 19);

    textureRedPoint.loadFromFile("red.png", sf::IntRect(0, 0, 32, 32));
    redPoint.setTexture(textureRedPoint);
    redPoint.setOrigin(16, 16);
}
//Движение кота к огоньку
void update(const sf::Vector2f& mousePosition, sf::Clock& clock, sf::Sprite &cat, sf::Sprite &redPoint)
{
  redPoint.setPosition(mousePosition);
  const sf::Vector2f mousePositionDelta = mousePosition - cat.getPosition();
  float deltaTime = clock.restart().asSeconds();
  float distancePointCat = std::hypot(mousePositionDelta.x, mousePositionDelta.y);
  //sf::Vector2f direction = {mousePositionDelta.x / distancePointCat, mousePositionDelta.y / distancePointCat};
  float speedMotionMax = 90.0;
  //float speedMotion= speedMotionMax * deltaTime;
  //cat.setPosition(cat.getPosition() + direction * speedMotion);      

  if (redPoint.getPosition().x < cat.getPosition().x)
  {
    cat.setScale(-1,1);
  }
  if (redPoint.getPosition().x > cat.getPosition().x)
  {
    cat.setScale(1,1);
  }
 
  if (distancePointCat != 0)
  {
      sf::Vector2f direction = {mousePositionDelta.x / distancePointCat, mousePositionDelta.y / distancePointCat};
      float speedMotion = std::min(speedMotionMax * deltaTime, distancePointCat);

      cat.setPosition(cat.getPosition() + direction * speedMotion); 
  }
  
}
//Обработка событий (закрытие окна, нажатие мыши)
void pollEvent(sf::RenderWindow& window, sf::Vector2f& mousePosition, int &flag)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::MouseButtonPressed:
            //onMouseReleased(event.MouseButtonEvent, mousePosition);
            mousePosition = {float(event.mouseButton.x), float(event.mouseButton.y)};
            //Мышь нажата (ФЛАГ)
            flag = 1;
            break;
        default:
            break;
                
        }
    }
}
//Отрисовка кота и точки
void redrawFrame(sf::RenderWindow& window, sf::Sprite& cat, sf::Sprite& redPoint, int &flag)
{
    window.clear(sf::Color(255,255,255));
    window.draw(cat);
    if (flag == 1)
    {
        window.draw(redPoint);
    }
    window.display();
}

int main()
{
    constexpr unsigned WINDOW_WIDTH = 800;
    constexpr unsigned WINDOW_HEIGHT = 600;
    //Флаг на нажатие лкм
    int flag = 0;
    //Инициализация окна
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT}), "Cat and point",
        sf::Style::Default, settings);
    //Объявление часов
    sf::Clock clock;   
    //Объявление текстур и спрайтов
    sf::Texture texture;
    sf::Sprite cat;
    sf::Texture textureRedPoint;
    sf::Sprite redPoint;
    //Объявление позиции мыши
    sf::Vector2f mousePosition = {350, 250};    
    //Инициализация кота и точки(вызов функции)
    init(texture, textureRedPoint, cat, redPoint);
    //Основной цикл
    while (window.isOpen())
    {
        //Обработка событий (закрытие окна, нажатие мыши)(вызов функции)
        pollEvent(window, mousePosition, flag);
        //Движение кота к огоньку (вызов функции)
        update(mousePosition, clock, cat, redPoint);
        //Отрисовка указателя (вызов функции)
        redrawFrame(window, cat, redPoint, flag);
    }
}
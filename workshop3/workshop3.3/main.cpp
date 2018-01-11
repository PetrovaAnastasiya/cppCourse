#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;

void onMouseMove(sf::Event::MouseMoveEvent &event, sf::Vector2f &mousePosition)
{
    mousePosition = {float(event.x), float(event.y)};
    sf::Vector2f delta = sf::Vector2f({WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}) - mousePosition;
    float length = std::hypot(delta.x, delta.y);
    std::cout << length << std::endl;
}
void pollEvents(sf::RenderWindow &window, sf::Vector2f &mousePosition)
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
void newLine(sf::VertexArray(&line), sf::Vector2f mousePosition)
{

    line[0].position = sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    line[1].position = mousePosition;
    line[0].color = sf::Color::Black;
    line[1].color = sf::Color::Black;
}

void redrawFrame(sf::RenderWindow &window, sf::VertexArray &line)
{
    window.clear(sf::Color(0xFF, 0xFF, 0xFF));
    window.draw(line);
    window.display();
}

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Text",
        sf::Style::Default, settings);

    sf::Vector2f mousePosition;
    sf::VertexArray line(sf::Lines, 2);
    while (window.isOpen())
    {
        pollEvents(window, mousePosition);
        redrawFrame(window, line);
        newLine(line, mousePosition);
    }
}
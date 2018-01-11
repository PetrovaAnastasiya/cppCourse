#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
struct Circle
{
    sf::Vector2f position;
    sf::CircleShape ball;
    int radius;
    
};
void init(Circle &circle, sf::Font &font, sf::Text &text)
{
    circle.radius = 100;
    circle.position = {400, 300};

    text.setFont (font);
    text.setString("PAD");
    text.setCharacterSize(50);
    text.setFillColor(sf::Color::Blue);
    text.setPosition(circle.position.x - 50, circle.position.y - 30);


    circle.ball.setPosition(circle.position);
    circle.ball.setRadius(circle.radius);
    circle.ball.setFillColor(sf::Color(0, 255, 102));
    circle.ball.setOutlineColor(sf::Color(111, 58, 0));
    circle.ball.setOutlineThickness(5);
    circle.ball.setOrigin(circle.radius, circle.radius);
}
void pollEvents(sf::RenderWindow &window)
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

void redrawFrame(sf::RenderWindow &window, Circle &circle, sf::Text &text)
{
    window.clear(sf::Color(243, 243, 243));
    window.draw(circle.ball);
    window.draw(text);
    window.display();
}

int main()
{
    constexpr unsigned WINDOW_WIDTH = 800;
    constexpr unsigned WINDOW_HEIGHT = 600;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Text",
        sf::Style::Default, settings);

    sf::Font font;
    sf::Text text;
    
    font.loadFromFile("arial.ttf");

    Circle circle;

    init(circle, font, text);
    while (window.isOpen())
    {
        pollEvents(window);
        redrawFrame(window, circle, text);
    }
}
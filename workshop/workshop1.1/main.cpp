#include <SFML/Graphics.hpp>
#include <cmath>

void init (sf::ConvexShape &arrow)
{
    arrow.setPosition({400, 320});
    arrow.setPointCount(7);
    arrow.setPoint(0,{0,0});
    arrow.setPoint(1,{-50,50});
    arrow.setPoint(2,{-25,50});
    arrow.setPoint(3,{-25,100});
    arrow.setPoint(4,{25,100});
    arrow.setPoint(5,{25,50});
    arrow.setPoint(6,{50,50});
    arrow.setFillColor(sf::Color(0xFF,0xFF, 0x00));
    arrow.setOutlineColor(sf::Color(0x00,0x00,0x00));
    arrow.setOutlineThickness(3);
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

void redrawFrame(sf::RenderWindow& window, sf::ConvexShape& arrow)
{
    window.clear(sf::Color(0xFF,0xFF,0xFF));
    window.draw(arrow);
    window.display();
}

int main()
{
    constexpr unsigned WINDOW_WIDTH = 800;
    constexpr unsigned WINDOW_HEIGHT = 600;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT}), "Arrow",
        sf::Style::Default, settings);

    sf::ConvexShape arrow;    
    init(arrow);    
    while (window.isOpen())
    {
        pollEvents(window);
        redrawFrame(window, arrow);
    }
}
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

void init(sf::Texture &texture, sf::Sprite &cat)
{
    texture.loadFromFile("cat.png", sf::IntRect(0, 0, 45, 45));
    cat.setTexture(texture);
    cat.setPosition(350,250);

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

void redrawFrame(sf::RenderWindow& window, sf::Sprite& cat)
{
    window.clear(sf::Color(255,255,255));
    window.draw(cat);
    window.display();
}

int main()
{
    constexpr unsigned WINDOW_WIDTH = 800;
    constexpr unsigned WINDOW_HEIGHT = 600;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT}), "Cat",
        sf::Style::Default, settings);
    
    sf::Texture texture;
    sf::Sprite cat;
    init(texture, cat);
    while (window.isOpen())
    {
        pollEvents(window);
        redrawFrame(window, cat);
    }
}
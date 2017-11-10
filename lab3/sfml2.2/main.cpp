#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

int main()
{
    constexpr int pointCount = 500;
    const float radius = 100;
    float time = 0;
    const int x0 = 300;
    const int y0 = 300;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({600, 600}), "rose",
        sf::Style::Default, settings);
    sf::Clock clock;
    sf::ConvexShape rose;
    rose.setPosition({400, 320});
    rose.setFillColor(sf::Color(0xFF, 0x00, 0x6D));

    rose.setPointCount(pointCount);
    for (int pointNo = 0; pointNo < pointCount; ++pointNo)
    {
        float angle = float(2 * M_PI * pointNo) / float(pointCount);
        float roseRadius = 200 * sin(6 * angle);
        sf::Vector2f point = {
            roseRadius * std::sin(angle),
            roseRadius * std::cos(angle)};
        rose.setPoint(pointNo, point);
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        
        constexpr float periodY = 4;
        
        float dt = clock.restart().asSeconds();
        time = dt + time;
        float wavePhase = time * float(2 * M_PI);
        float x = x0 + radius * std::cos(wavePhase / periodY);
        float y = y0 + radius * std::sin(wavePhase / periodY);
        const sf::Vector2f offset = {x, y};

        rose.setPosition(offset);
    
        window.clear();
        window.draw(rose);
        window.display();
    }
}
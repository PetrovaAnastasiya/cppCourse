#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

void initEye(sf::ConvexShape &eye,  float eyeX, float eyeY)
{
    constexpr int pointCount = 200;
    const sf::Vector2f eyeRadius = {70.f,150.f};
    eye.setPosition(eyeX,eyeY);
    eye.setFillColor(sf::Color(0xFF, 0xFF, 0xFF));
    eye.setPointCount(pointCount);
    for (int pointNo = 0; pointNo < pointCount; ++pointNo)
    {
        float angle = float(2 * M_PI * pointNo) / float(pointCount);
        sf::Vector2f point = {
            eyeRadius.x * std::sin(angle),
            eyeRadius.y * std::cos(angle)
        };
        eye.setPoint(pointNo, point);
    }
}    
void initPupil(sf::ConvexShape &pupil, float pupilX, float pupilY)
{
    constexpr int pointCount = 200;
    const sf::Vector2f pupilRadius = {14.f,25.f};
    pupil.setPosition(pupilX,pupilY);
    pupil.setFillColor(sf::Color());
    pupil.setPointCount(pointCount);
    for (int pointNo = 0; pointNo < pointCount; ++pointNo)
    {
        float angle = float(2 * M_PI * pointNo) / float(pointCount);
        sf::Vector2f point = {
            pupilRadius.x * std::sin(angle),
            pupilRadius.y * std::cos(angle)
        };
        pupil.setPoint(pointNo, point);
    }
}    
sf::Vector2f toEuclidean(float radiusX, float radiusY, float angle)
{
    return {
        radiusX * std::cos(angle),
        radiusY * std::sin(angle)
    };
}
void update(const sf::Vector2f& mousePosition, const sf::Vector2f& pupilFirstPosition,
            const sf::Vector2f& pupilSecondPosition, sf::ConvexShape& eyeFirst, 
            sf::ConvexShape& pupilFirst, sf::ConvexShape& eyeSecond, sf::ConvexShape& pupilSecond) 
{
    const sf::Vector2f deltaFirstPupil = mousePosition - pupilFirstPosition;
    float angleFirstPupil = atan2(deltaFirstPupil.y, deltaFirstPupil.x);
    const sf::Vector2f pupilFirstOffset = toEuclidean(50, 70, angleFirstPupil);
    pupilFirst.setPosition(pupilFirstPosition + pupilFirstOffset);
    
    const sf::Vector2f deltaSecondPupil = mousePosition - pupilSecondPosition;
    float angleSecondPupil = atan2(deltaSecondPupil.y, deltaSecondPupil.x);
    const sf::Vector2f pupilSecondOffset = toEuclidean(50, 70, angleSecondPupil);
    pupilSecond.setPosition(pupilSecondPosition + pupilSecondOffset);
}
void onMouseMove(const sf::Event::MouseMoveEvent& event, sf::Vector2f& mousePosition)
{
    mousePosition = {float(event.x), float(event.y)};
}

void pollEvent(sf::RenderWindow &window, sf::Vector2f& mousePosition)
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

void redrawFrame(sf::RenderWindow& window, sf::ConvexShape& eyeFirst, 
                 sf::ConvexShape& pupilFirst, sf::ConvexShape& eyeSecond, sf::ConvexShape& pupilSecond)
{
    window.clear(sf::Color(234,234,234));
    window.draw(eyeFirst);
    window.draw(pupilFirst);
    window.draw(eyeSecond);
    window.draw(pupilSecond);
    window.display();
}

int main()
{
    constexpr unsigned WINDOW_WIDTH = 800;
    constexpr unsigned WINDOW_HEIGHT = 600;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(
        sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT}), "Eyes",
        sf::Style::Default, settings);

    sf::ConvexShape eyeFirst;   
    sf::ConvexShape eyeSecond;    
    sf::ConvexShape pupilFirst; 
    sf::ConvexShape pupilSecond;    
    sf::Vector2f mousePosition;

    initPupil(pupilFirst, 300, 300);
    initPupil(pupilSecond, 500, 300);
    initEye(eyeFirst, 300, 300);
    initEye(eyeSecond, 500, 300);
   
    const sf::Vector2f pupilFirstPosition = {300.f,300.f};
    const sf::Vector2f pupilSecondPosition = {500.f,300.f};
    
    
    while (window.isOpen())
    {
        pollEvent(window, mousePosition);
        update(mousePosition, pupilFirstPosition, pupilSecondPosition, eyeFirst, pupilFirst, eyeSecond, pupilSecond);
        redrawFrame(window, eyeFirst, pupilFirst, eyeSecond, pupilSecond);
    }
}
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

#include "iostream"
#include "math.h"

const int WIDTH = 1000;
const int HEIGHT = 1000;
const int MAX_DEEP = 30;
const int BORDER_DEEP = -1;
const double EBS = 1e-6;
const double TEBS = 1 / EBS;
const int TEBSi = TEBS;
const sf::Time DELAY_TIME = sf::seconds(1.5);
const std::vector<sf::Vector2f> INIT_SHAPE = 
{
    {475, 775},
    {525, 775},
    {525, 825},
    {475, 825}
};

typedef std::vector<sf::Vector2f> Rect;

sf::Vector2f turn_ccwise(sf::Vector2f v, float angle)
{
    angle = -angle;
    sf::Vector2f cur;

    cur.x = v.x * cos(angle) - v.y * sin(angle);
    cur.y = v.x * sin(angle) + v.y * cos(angle);

    return cur;
}

sf::Vector2f turn_cwise(sf::Vector2f v, float angle)
{
    return(turn_ccwise(v, -angle));
}

float vlength(sf::Vector2f v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

sf::Vector2f scale(sf::Vector2f v, float length)
{
    sf::Vector2f cur;
    float cur_length = vlength(v);

    cur.x = v.x / cur_length * length;
    cur.y = v.y / cur_length * length;

    return cur;
}

sf::Color getColor(int deep)
{
    float deepf = deep * 1.0 / MAX_DEEP;

    if(deepf < 0.5)
    {
        return sf::Color( (1 - deepf / 0.5) * 255, 0, deepf / 0.5 * 255);
    }
    else
    {
        deepf -= 0.5;
        return sf::Color(0, deepf / 0.5 * 255, (1 - deepf / 0.5) * 255);
    }
}

void draw(sf::RenderWindow *window, Rect shape, int deep = 0)
{
    sf::ConvexShape rect;

    rect.setPointCount(4);
    for(int i = 0; i < 4; i++)
    {
        rect.setPoint(i, shape[i]);
    }
    if(deep <= BORDER_DEEP)
    {
        rect.setOutlineThickness(1);
        rect.setOutlineColor(sf::Color::Black);
    }
    rect.setFillColor(getColor(deep));

    window->draw(rect);

    if(deep == MAX_DEEP)
    {
        return;
    }

    float angle = (rand() % TEBSi) / TEBS * M_PI_2;
    float side = vlength(shape[0] - shape[1]);

    if(side < 1.0)
    {
        return;
    }

    sf::Vector2f base = (shape[1] - shape[0]);
    sf::Vector2f nlbase = scale(base, side * cos(angle));
    sf::Vector2f nrbase = scale(-base, side * sin(angle));


    Rect leftRect = {
        shape[0] + turn_ccwise(nlbase, angle + M_PI_2),
        shape[0] + turn_ccwise(nlbase, angle + M_PI_2) + turn_ccwise(nlbase, angle),
        shape[0] + turn_ccwise(nlbase, angle),
        shape[0]
    };

    Rect rightRect = {
        shape[1] + turn_cwise(nrbase, M_PI_2 - angle) + turn_cwise(nrbase, M_PI - angle),
        shape[1] + turn_cwise(nrbase, M_PI - angle),
        shape[1],
        shape[1] + turn_cwise(nrbase, M_PI_2 - angle)
    };

    draw(window, leftRect, deep + 1);
    draw(window, rightRect, deep + 1);
}

int main()
{
    srand(time(NULL));

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "pifagor_tree", sf::Style::Default, settings);

    while( window.isOpen() )
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        sf::Clock timer;

        window.clear(sf::Color::White);
        draw(&window, INIT_SHAPE);
        window.display();

        sf::Time elapsed = timer.getElapsedTime();
        std::cout << "Rendered for " << elapsed.asSeconds() << " seconds.\n";
        sf::sleep(std::max(sf::seconds(0), DELAY_TIME - elapsed));
    }

    return 0;
}
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

#include "iostream"

const int WIDTH = 720;
const int HEIGHT = 720;
const int MAX_DEEP = 4;
const int BORDER_DEEP = 3;
const sf::Time DELAY_TIME = sf::seconds(1);

void draw(sf::RenderWindow *window, int x, int y, int width, int height, int deep = 0)
{
    sf::RectangleShape shape;
    shape.setSize({(float)width, (float)height});
    shape.setPosition({(float)x, (float)y});

    if(deep == BORDER_DEEP)
    {
        shape.setOutlineThickness(5);
        shape.setOutlineColor(sf::Color::Black);
        window->draw(shape);
    }
    if(deep == MAX_DEEP || width <= 1 || height <= 1)
    {    
        shape.setFillColor(sf::Color(25 + rand() % 180, 20 + rand() % 180, 20 + rand() % 180));
        window->draw(shape);
        return;
    }

    int divide_dot_vertical = width / 4 + rand() % (width / 2);
    int divide_dot_horizontal = height / 4 + rand() % (height / 2);

    draw(window, x, y, divide_dot_vertical, divide_dot_horizontal, deep + 1);
    draw(window, x + divide_dot_vertical, y, std::max(0, width - divide_dot_vertical), divide_dot_horizontal, deep + 1);
    draw(window, x, y + divide_dot_horizontal, divide_dot_vertical, std::max(0, height - divide_dot_horizontal), deep + 1);
    draw(window, x + divide_dot_vertical, y + divide_dot_horizontal, std::max(0, width - divide_dot_vertical), std::max(0, height - divide_dot_horizontal), deep + 1);
}

int main()
{
    srand(time(NULL));

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "cubism");

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
        draw(&window, 0, 0, WIDTH, HEIGHT);
        window.display();


        sf::Time elapsed = timer.getElapsedTime();
        std::cout << "Rendered for " << elapsed.asSeconds() << " seconds.\n";
        sf::sleep(std::max(sf::seconds(0), DELAY_TIME - elapsed));
    }

    return 0;
}
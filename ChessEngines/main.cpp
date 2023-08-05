#include <SFML/Graphics.hpp>
#include "Board.h"
#include "GameBoard.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Chess", sf::Style::Titlebar | sf::Style::Close);

    sf::Image icon;
    if (!icon.loadFromFile("black_king.png")) { std::cout << "Failed to load king.png"; }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    GameBoard b;

    while (window.isOpen()) {
        window.clear(sf::Color(50, 46, 43));

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                b.hold(&window, sf::Vector2f(event.mouseButton.x, event.mouseButton.y));

            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
                b.drop(&window, sf::Vector2f(event.mouseButton.x, event.mouseButton.y));

            if (event.type == sf::Event::MouseMoved)
                b.hover(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
        }

        b.drawBoard(&window);
        window.display();
    }

    return 0;
}
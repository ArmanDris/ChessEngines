#include <SFML/Graphics.hpp>
#include "Board.h"
#include "GameBoard.h"
#include "Engine.h"
#include "RandomEngine.h"
#include "DrunkEngine.h"
#include "HippieEngine.h"


int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Chess", sf::Style::Default);

    sf::Image icon;
    if (!icon.loadFromFile("black_king.png")) { std::cout << "Failed to load king.png"; }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    GameBoard b;
    HippieEngine p1;
    DrunkEngine p2;

    b.setPlayer(&p1, Color::White);
    b.setPlayer(&p2, Color::Black);

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

            if (event.type == sf::Event::Resized)
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
        }

        b.drawBoard(&window);
        b.preformCPUMoves();
        window.display();
    }

    return 0;
}
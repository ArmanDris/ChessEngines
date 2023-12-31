#include <SFML/Graphics.hpp>
#include "Board.h"
#include "GameBoard.h"
#include "SimpleEngines.h"
#include "MinMaxEngine.h"
#include "MinMaxNoPruning.h"

int main() {

    // Board initialization
    GameBoard b;

    MinMaxEngine mm;
    b.setPlayer(&mm, Color::Black);

    // Window Setup
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktop, "Chess", sf::Style::Default);
    window.setPosition(sf::Vector2i(-8, 0));
    sf::Image icon;
    icon.loadFromFile("black_king.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Game Loop
    while (window.isOpen()) {
        window.clear(sf::Color(50, 46, 43));

        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close(); 
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left)
                    b.hold(window, sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                break;
            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Left)
                    b.drop(window, sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                break;
            case sf::Event::MouseMoved:
                b.hover(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                case sf::Keyboard::Space: b.triggerMove(); break;
                case sf::Keyboard::Left: b.undoMove(); break;
                }
                break;
            case sf::Event::Resized:
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                break;
            }
        }

        b.drawBoard(window);
        b.preformCPUMoves(500);

        window.display(); // Do not do this first or else will display frame will old info
    }

    return 0;
}
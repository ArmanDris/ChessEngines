#include <SFML/Graphics.hpp>
#include "Board.h"
#include "GameBoard.h"
#include "Engine.h"
#include "SimpleEngines.h"
#include "MinMaxEngine.h"


int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Chess", sf::Style::Default);

    sf::Image icon;
    if (!icon.loadFromFile("black_king.png")) { std::cout << "Failed to load king.png"; }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    RandomEngine random;
    DrunkEngine drunk;
    HippieEngine hippie;
    BotFeasterEngine botF;
    MinMaxEngine minMax;
    GameBoard b;

    b.setPlayer(nullptr, Color::White);
    b.setPlayer(&minMax, Color::Black);

    while (window.isOpen()) {
        window.clear(sf::Color(50, 46, 43));
        b.drawBoard(&window);
        window.display();

        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close(); 
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    b.hold(&window, sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                }
                break;
            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    b.drop(&window, sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                }
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

        b.preformCPUMoves(3000);
    }

    return 0;
}
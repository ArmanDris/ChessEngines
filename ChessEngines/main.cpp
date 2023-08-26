#include <SFML/Graphics.hpp>
#include "Board.h"
#include "GameBoard.h"
#include "Engine.h"
#include "SimpleEngines.h"
#include "MinMaxEngine.h"

int main() {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    sf::RenderWindow window(desktop, "Chess", sf::Style::Default);

    sf::Image icon;
    icon.loadFromFile("black_king.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    GameBoard b;

    RandomEngine random;
    BotFeasterEngine botF;
    MinMaxEngine minMax;

    //minMax.returnMove(b);

    b.importFEN("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");

    b.setPlayer(&botF, Color::White);
    b.setPlayer(&botF, Color::Black);

    while (window.isOpen()) {
        window.clear(sf::Color(50, 46, 43));
        b.drawBoard(window);
        window.display();

        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close(); 
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    b.hold(window, sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                }
                break;
            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    b.drop(window, sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
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

        b.preformCPUMoves();
    }

    return 0;
}
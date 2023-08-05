#include "Ui.h"

sf::Vector2f Ui::getTopLeftCorner(sf::RenderWindow* w, sf::Vector2i square) {
	sf::Vector2f centerOfWindow(w->getSize().x/2, w->getSize().y / 2);
	int boardLength = squareLength * 8;
	sf::Vector2f topLeftBoardCorner(centerOfWindow.x - boardLength / 2, centerOfWindow.y - boardLength / 2);
	return sf::Vector2f(squareLength * square.x + topLeftBoardCorner.x, squareLength * square.y + topLeftBoardCorner.y);
}

int Ui::squareLength = 75;
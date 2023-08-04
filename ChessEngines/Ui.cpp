#include "Ui.h"

sf::Vector2f Ui::getTopLeftCorner(sf::Vector2i square) {
	return sf::Vector2f(squareLength * square.x + 200, squareLength * square.y + 100);
}

int Ui::squareLength = 75;
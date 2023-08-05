#include "Piece.h"

Piece::Piece(char pieceId, char color, sf::Vector2i sq):id(pieceId), color(color)
{
	setSquare(sq);
}

char Piece::getId() {
	return id;
}

sf::FloatRect Piece::getBoundingBox(sf::RenderWindow* w) {
	sf::Vector2f coords = Ui::getTopLeftCorner(w, square);
	return sf::FloatRect(coords.x, coords.y, Ui::squareLength, Ui::squareLength);
}

void Piece::drawPiece(sf::RenderWindow* w, sf::Texture* t) {
	if (!visible) return;
	drawPiece(w, t, Ui::getTopLeftCorner(w, square));
}

void Piece::drawPiece(sf::RenderWindow* w, sf::Texture* t, sf::Vector2f coords) {
	sf::Sprite s;
	s.setTexture(*t);
	s.setScale(5, 5);
	s.setPosition(coords);
	w->draw(s);
}

void Piece::setSquare(sf::Vector2i sq) {
	square = sq;
}

sf::Vector2i Piece::getSquare() {
	return square;
}

char Piece::getColor() {
	return color;
}
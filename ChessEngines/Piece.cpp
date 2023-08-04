#include "Piece.h"

Piece::Piece(char pieceId, char color, sf::Texture* texture, sf::RenderWindow* window) :id(pieceId), color(color), t(texture), w(window), s(sf::Sprite()) {

	s.setTexture(*t);
	s.setScale(5, 5);
	square = sf::Vector2i(0, 0);
}

Piece::Piece(char pieceId, char color, sf::Texture* texture, sf::RenderWindow* window, sf::Vector2i sq)
	: Piece(pieceId, color, texture, window)
{
	setSquare(sq);
}

char Piece::getId() {
	return id;
}

sf::FloatRect Piece::getBoundingBox() {
	return s.getGlobalBounds();
}

void Piece::drawPiece() {
	if (!visible) return;
	s.setPosition(Ui::getTopLeftCorner(square));
	w->draw(s);
}

void Piece::drawPiece(sf::Vector2f coords) {
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
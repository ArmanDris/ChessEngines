#include "Piece.h"

std::string typeToString(Type type)
{
	switch (type) {
	case Type::None:   return "None";
	case Type::Pawn:   return "Pawn";
	case Type::Rook:   return "Rook";
	case Type::Knight: return "Knight";
	case Type::Bishop: return "Bishop";
	case Type::Queen:  return "Queen";
	case Type::King:   return "King";
	default:           return "Unknown";
	}
}

Piece::Piece()
{
	id = Type::None;
	color = Color::None;
	square = sf::Vector2i(-1, -1);
}

Piece::Piece(Type pieceId, Color color, sf::Vector2i sq):id(pieceId), color(color)
{
	setSquare(sq);
}

Type Piece::getId() {
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

Color Piece::getColor() {
	return color;
}
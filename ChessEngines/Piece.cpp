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

std::string colorToString(Color color)
{
	switch (color) {
	case Color::White:   return "W";
	case Color::Black:   return "B";
	case Color::None:    return "None";
	default:             return "Unknown";
	}
}

Piece::Piece() {}

Piece::Piece(Type pieceId, Color color):type(pieceId), color(color) {}

Type Piece::getType() const {
	return type;
}

int Piece::getValue() const
{
	switch (type) {
	case Type::None: return 0;
	case Type::Pawn: return 1;
	case Type::Knight: return 3;
	case Type::Bishop: return 3;
	case Type::Rook: return 5;
	case Type::Queen: return 9;
	case Type::King: return 100;
	}
	return 0;
}

Color Piece::getColor() const {
	return color;
}

#include "Piece.h"

Piece::Piece() {}

Piece::Piece(Type t, Color c)
	: data_(static_cast<unsigned int>(t) | static_cast<unsigned int>(c)) {}

Type Piece::getType() const
{
	return static_cast<Type>(data_ & type_mask);
}

Color Piece::getColor() const
{
	return static_cast<Color>(data_ & color_mask);
}

int Piece::getValue() const {
	Type type = this->getType();
	switch(type) {
		case None: return 0;
		case Pawn: return 1;
		case Rook: return 5;
		case Knight: return 3;
		case Bishop: return 3;
		case King: return 100;
		case Queen: return 9;
	}
}

std::string Piece::typeToString() const
{
	unsigned int type_bits = data_ & type_mask;

	switch(type_bits) {
		case None: return "None";
		case Pawn: return "Pawn";
		case Rook: return "Rook";
		case Knight: return "Knight";
		case Bishop: return "Bishop";
		case King: return "King";
		case Queen: return "Queen";
	}
}

std::string Piece::colorToString() const
{
	unsigned int color_bits = data_ & color_mask;

	switch(color_bits) {
		case White: return "White";
		case Black: return "Black";
	}
}

Piece::operator bool() const
{
	return getType() != Type::None;
}
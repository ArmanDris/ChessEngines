#pragma once
#include <bitset>
#include <string>

// Define constants for type and color
const unsigned int type_mask = 0b111;  // First 3 bits for type
const unsigned int color_mask = 0b1000; // Last bit for color

enum Type {
	None   = 0b000,
	Pawn   = 0b001,
	Rook   = 0b010,
	Knight = 0b011,
	Bishop = 0b100,
	King   = 0b101,
	Queen  = 0b110,
};

enum Color {
	White = 0b1000,
	Black = 0b0000,
};

class Piece {
public:
	Piece();
	Piece(Type t, Color c);

	Type getType() const;
	Color getColor() const;
	int getValue() const;
	std::string typeToString() const;
	std::string colorToString() const;
	explicit operator bool() const;

private:
	unsigned int data_ = 0;
};
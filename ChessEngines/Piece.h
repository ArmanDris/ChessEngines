#pragma once
#include <SFML/Graphics.hpp>

enum class Type { None, Pawn, Rook, Knight, Bishop, Queen, King };
enum class Color { None, White, Black };

std::string typeToString(Type type);

std::string colorToString(Color color);

class Piece
{
public:
	Piece();
	Piece(Type pieceId, Color color);
	Piece(const Piece& p) = default;

	Type getType() const;
	Color getColor() const;

	explicit operator bool() const {
		return type != Type::None;
	}

	int getValue() const;=

protected:
	Type type = Type::None;
	Color color = Color::None;
};


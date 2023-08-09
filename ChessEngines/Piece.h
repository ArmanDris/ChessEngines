#pragma once
#include <SFML/Graphics.hpp>
#include "Ui.h"

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
	void drawPiece(sf::RenderWindow* w, sf::Vector2f coords, const sf::Texture* t) const;

	explicit operator bool() const {
		return type != Type::None;
	}

protected:
	Type type;
	Color color;
};


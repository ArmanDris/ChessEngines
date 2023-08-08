#pragma once
#include <SFML/Graphics.hpp>
#include "Ui.h"

enum class Type { None, Pawn, Rook, Knight, Bishop, Queen, King };
enum class Color { None, White, Black };

std::string typeToString(Type type);


class Piece
{
public:
	Piece();
	Piece(Type pieceId, Color color, sf::Vector2i sq);
	Piece(const Piece& p) = default;

	bool moved = false;
	bool justMovedUpTwo = false;
	bool visible = true;

	void setSquare(sf::Vector2i square);
	Type getId();
	Color getColor();
	sf::Vector2i getSquare();
	sf::FloatRect getBoundingBox(sf::RenderWindow* w);
	void drawPiece(sf::RenderWindow* w, sf::Texture* texture);
	void drawPiece(sf::RenderWindow* w, sf::Texture* texture, sf::Vector2f coords);

	explicit operator bool() const {
		return id != Type::None;
	}

protected:
	Type id;
	Color color;
	sf::Vector2i square;
};


#pragma once
#include <SFML/Graphics.hpp>
#include "Ui.h"

class Piece
{
public:
	// Constructor for the squares of the board
	Piece(char pieceId, char color, sf::Vector2i sq = sf::Vector2i(0, 0));
	Piece(const Piece& p) = default;

	bool moved = false;
	bool justMovedUpTwo = false;
	bool visible = true;

	void setSquare(sf::Vector2i square);
	char getId();
	char getColor();
	sf::Vector2i getSquare();
	sf::FloatRect getBoundingBox(sf::RenderWindow* w);
	void drawPiece(sf::RenderWindow* w, sf::Texture* texture);
	void drawPiece(sf::RenderWindow* w, sf::Texture* texture, sf::Vector2f coords);

protected:
	char id;
	char color;
	sf::Vector2i square;
};


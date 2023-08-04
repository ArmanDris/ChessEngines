#pragma once
#include <SFML/Graphics.hpp>
#include "Ui.h"

class Piece
{
public:
	// Constructor for the squares of the board
	Piece(char pieceId, char color, sf::Texture* texture, sf::RenderWindow* window);
	Piece(char pieceId, char color, sf::Texture* texture, sf::RenderWindow* window, sf::Vector2i sq);
	Piece(const Piece& p) = default;

	bool moved = false;
	bool justMovedUpTwo = false;
	bool visible = true;

	char getId();
	void setSquare(sf::Vector2i square);
	sf::Vector2i getSquare();
	char getColor();
	sf::FloatRect getBoundingBox();
	void drawPiece();
	void drawPiece(sf::Vector2f coords);

protected:
	char id;
	char color;
	sf::Texture* t;
	sf::Sprite s;
	sf::RenderWindow* w;
	sf::Vector2i square;
};


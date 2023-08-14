#pragma once
#include <SFML/Graphics.hpp>
#include "Piece.h"
#include "Ui.h"
#include <iostream>
#include <vector>
#include <fstream>

class Board
{
public:
	Board();

	bool makeMove(sf::Vector2i oldSquare, sf::Vector2i newSquare);
	void saveLog(std::string fileName = "log.txt");

	// Overload [] operator to access elements using sf::Vector2i
	Piece& operator[](sf::Vector2i square)
	{
		return board[square.x][square.y];
	}

protected:
	Piece board[8][8] = {
		{Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
		{Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
		{Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
		{Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
		{Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
		{Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
		{Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()},
		{Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()}
	};

	std::vector<std::tuple<Color, Type, sf::Vector2i, Color, Type,  sf::Vector2i>> log;

	bool whiteTurn = true;

	bool whiteVictory = false;
	bool blackVictory = false;
	bool draw = false;

	void placeStartingPieces();
	void movePiece(sf::Vector2i oldSquare, sf::Vector2i newSquare);
	void castle(sf::Vector2i oldSquare, sf::Vector2i newSquare);
	void changeTurn();

	bool legal_move(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool piece_is_attacking_square(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool pawn_is_attacking_square(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool rook_is_attacking_square(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool knight_is_attacking_square(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool bishop_is_attacking_square(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool queen_is_attacking_square(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool king_is_attacking_square(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;

	bool moveIsCastle(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool moveIsEnPassent(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool hasPieceMoved(sf::Vector2i startingSquare) const;
	bool hasPawnJustMovedUpTwo(sf::Vector2i sq) const;

	bool square_is_attacked_by(sf::Vector2i sq, Color color) const;
	bool isKingInCheck(Color color) const;
	bool willMoveCauseCheckForColor(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool checkGameOver();


	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> get_moves();

	void logMove(sf::Vector2i oldSquare, sf::Vector2i newSquare);
};
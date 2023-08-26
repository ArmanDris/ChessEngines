#pragma once
#include <SFML/Graphics.hpp>
#include "Piece.h"
#include "Ui.h"
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <ctime> // for std::time_t and std::tm
#include <iomanip> // for std::put_time

class Board
{
public:
	Board();

	bool makeMove(sf::Vector2i oldSquare, sf::Vector2i newSquare);
	void undoMove();
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> get_moves();

	Piece getPiece(sf::Vector2i square) const { return board[square.x][square.y]; }
	bool isWhiteTurn() const { return whiteTurn; }
	bool isWhiteVictory() const { return whiteVictory; }
	bool isBlackVictory() const { return whiteVictory; }
	bool isDraw() const { return draw; }

	void importFEN(std::string FEN);

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

	std::vector<std::tuple<Piece, sf::Vector2i, Piece,  sf::Vector2i>> log;

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

	bool checkGameOver();
	bool square_is_attacked_by(sf::Vector2i sq, Color color) const;
	bool isKingInCheck(Color color) const;
	bool willMoveCauseCheckForColor(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool fiftyMoveRule() const;

	void logMove(sf::Vector2i oldSquare, sf::Vector2i newSquare);
	void saveLog(std::string fileName = "log.txt");
};
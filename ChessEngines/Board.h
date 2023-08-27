#pragma once
#include <SFML/Graphics.hpp>
#include "Piece.h"
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

	bool makeMove(const sf::Vector2i oldSquare, const sf::Vector2i newSquare);
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
	void movePiece(const sf::Vector2i& oldSquare, const sf::Vector2i& newSquare);
	void castle(const sf::Vector2i& oldSquare, const sf::Vector2i& newSquare);
	void changeTurn();

	bool legal_move(const sf::Vector2i& oldSquare, const sf::Vector2i& newSquare) const;
	bool piece_is_attacking_square(const sf::Vector2i& oldSquare, const sf::Vector2i& newSquare) const;
	bool pawn_is_attacking_square(const sf::Vector2i& oldSquare, const sf::Vector2i& newSquare) const;
	bool rook_is_attacking_square(const sf::Vector2i& oldSquare, const sf::Vector2i& newSquare) const;
	bool knight_is_attacking_square(const sf::Vector2i& oldSquare, const sf::Vector2i& newSquare) const;
	bool bishop_is_attacking_square(const sf::Vector2i& oldSquare, const sf::Vector2i& newSquare) const;
	bool queen_is_attacking_square(const sf::Vector2i& oldSquare, const sf::Vector2i& newSquare) const;
	bool king_is_attacking_square(const sf::Vector2i& oldSquare, const sf::Vector2i& newSquare) const;

	bool moveIsCastle(const sf::Vector2i& oldSquare, const sf::Vector2i& newSquare) const;
	bool moveIsEnPassent(const sf::Vector2i& oldSquare, const sf::Vector2i& newSquare) const;
	bool hasPieceMoved(const sf::Vector2i& startingSquare) const;
	bool hasPawnJustMovedUpTwo(const sf::Vector2i& sq) const;

	bool checkGameOver();
	bool square_is_attacked_by(const sf::Vector2i& sq, const Color& color) const;
	bool isKingInCheck(const Color& color) const;
	bool willMoveCauseCheckForColor(const sf::Vector2i& oldSquare, const sf::Vector2i& newSquare) const;
	bool fiftyMoveRule() const;

	void logMove(const sf::Vector2i oldSquare, const sf::Vector2i newSquare); // Passing by reference slows this down for some reason
	void saveLog(std::string fileName = "log.txt");
};
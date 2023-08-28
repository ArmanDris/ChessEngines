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

	bool makeMove(const sf::Vector2i old_square, const sf::Vector2i new_square);
	void undoMove();

	const Piece& getPiece(sf::Vector2i square) const { return board[square.x][square.y]; }
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
	std::vector<std::tuple<Piece, sf::Vector2i, Piece,  sf::Vector2i>> critical_log; // Only logs king and rook moves for castling

	bool whiteTurn = true;

	bool whiteVictory = false;
	bool blackVictory = false;
	bool draw = false;
	void placeStartingPieces();
	void movePiece(const sf::Vector2i& old_square, const sf::Vector2i& new_square);
	bool moveIsCastle(const sf::Vector2i& old_square, const sf::Vector2i& new_square);
	bool moveIsEnPassent(const sf::Vector2i& old_square, const sf::Vector2i& new_square);
	void castle(const sf::Vector2i& old_square, const sf::Vector2i& new_square);
	void changeTurn();
	void logMove(const sf::Vector2i old_square, const sf::Vector2i new_square); // Passing by reference slows this down for some reason
	void saveLog(std::string fileName = "log.txt");

	bool fiftyMoveRule() const;

	// New Logic
	using move = std::pair<sf::Vector2i, sf::Vector2i>;
	std::vector<move> generatePsudoLegalMoves();
};
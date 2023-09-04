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
	using move = std::pair<sf::Vector2i, sf::Vector2i>;

	Board();

	void makeMove(const sf::Vector2i old_square, const sf::Vector2i new_square);
	void makeSafeMove(const sf::Vector2i old_square, const sf::Vector2i new_square, const std::vector<move>& legal_moves);
	void undoMove();
	void softUndoMove();

	const Piece& getPiece(sf::Vector2i square) const { return getPiece(square.x, square.y); }
	const Piece& getPiece(int x, int y) const { return board[x][y]; }
	std::vector<move> getMoves();
	const move getLastMove() const;
	bool isWhiteTurn() const { return whiteTurn; }
	bool isWhiteVictory() const { return whiteVictory; }
	bool isBlackVictory() const { return blackVictory; }
	bool isDraw() const { return draw; }
	bool isGameOver() const;

	void importFEN(std::string FEN);
	sf::Vector2i stringToVec(std::string s);
	Piece charToPiece(char c);

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

	std::vector<std::tuple<Piece, sf::Vector2i, Piece, sf::Vector2i>> log;

	bool whiteTurn = true;

	bool whiteVictory = false;
	bool blackVictory = false;
	bool draw = false;

	void movePiece(const sf::Vector2i& old_square, const sf::Vector2i& new_square);
	void castle(const sf::Vector2i& old_square, const sf::Vector2i& new_square);
	void changeTurn();
	void logMove(const sf::Vector2i& old_square, const sf::Vector2i& new_square); // Passing by reference slows this down for some reason
	void saveLog(std::string fileName = "log.txt");

	// Logic from after optimisation refactor:
	std::vector<move> generateLegalMoves();
	void generatePsudoLegalMoves(Color& c, std::vector<move>& vec_to_append_moves_to);
	void appendPsudoLegalPawnMoves(const sf::Vector2i& sq, const Color& c, std::vector<move>& moves);
	void appendPsudoLegalRookMoves(const sf::Vector2i& sq, const Color& c, std::vector<move>& moves);
	void appendPsudoLegalKnightMoves(const sf::Vector2i& sq, const Color& c, std::vector<move>& moves);
	void appendPsudoLegalBishopMoves(const sf::Vector2i& sq, const Color& c, std::vector<move>& moves);
	void appendPsudoLegalKingMoves(const sf::Vector2i& sq, const Color& c, std::vector<move>& moves);
	void appendPsudoLegalQueenMoves(const sf::Vector2i& sq, const Color& c, std::vector<move>& moves);

	bool hasPieceMoved(const sf::Vector2i& sq);
	void checkGameOver(const std::vector<move>& legal_moves);
	bool insufficientMaterial() const;
};
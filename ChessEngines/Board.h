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
	void makeSafeMove(const sf::Vector2i old_square, const sf::Vector2i new_square);
	void undoMove();

	const Piece& getPiece(sf::Vector2i square) const { return pieceAt(square.x, square.y); }
	const Piece& getPiece(int x, int y) const { return pieceAt(x, y); }
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
	Piece board[8 * 8] = {
		Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(),
		Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(),
		Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(),
		Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(),
		Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(),
		Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(),
		Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(),
		Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece()
	};

	Piece& pieceAt(int x, int y) { return board[x + 8 * y]; }
	const Piece& pieceAt(int x, int y) const { return board[x + 8 * y]; }
	Piece& pieceAt(sf::Vector2i square) { return pieceAt(square.x, square.y); }
	const Piece& pieceAt(sf::Vector2i square) const { return pieceAt(square.x, square.y); }

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
	void checkGameOver();
	std::tuple<bool, sf::Vector2i, sf::Vector2i> insufficientMaterial() const;

	// Used only for checking logic
	bool isSquareAttacked(const sf::Vector2i& sq, const sf::Vector2i& tgt);
	bool isPawnAttacking(const sf::Vector2i& sq, const sf::Vector2i& tgt);
	bool isRookAttacking(const sf::Vector2i& sq, const sf::Vector2i& tgt);
	bool isKnightAttacking(const sf::Vector2i& sq, const sf::Vector2i& tgt);
	bool isBishopAttacking(const sf::Vector2i& sq, const sf::Vector2i& tgt);
	bool isKingAttacking(const sf::Vector2i& sq, const sf::Vector2i& tgt);
	bool isQueenAttacking(const sf::Vector2i& sq, const sf::Vector2i& tgt);
};

#pragma once
#include <SFML/Graphics.hpp>
#include "Piece.h"
#include "Ui.h"
#include <iostream>
#include <vector>
#include <fstream>

// Piece IDs
// Pawn: p
// Rook: r
// Knight: n
// Bishop: b
// Queen: q
// King: k

class Board
{
public:
	Board();

	bool makeMove(sf::Vector2i oldSquare, sf::Vector2i newSquare);
	bool checkGameOver();

	~Board();

protected:
	Piece* board[8][8] = {
		{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
		{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
		{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
		{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
		{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
		{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
		{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
		{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}
	};

	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> log;

	bool whiteTurn = true;
	bool whiteVictory = false;
	bool blackVictory = false;
	bool draw = false;

	void placeStartingPieces();
	void placePiece(char id, char color, sf::Vector2i square);
	bool makeMove(Piece* piece, sf::Vector2i newSquare);
	void movePiece(Piece* piece, sf::Vector2i newSquare);
	void takePiece(sf::Vector2i square);
	void removeAllMovedUpTwo();
	void castle(Piece* piece, sf::Vector2i newSquare);
	void promotePawns();
	void changeTurn();

	void importBoard(Piece * b[8][8]);

	bool validMove(Piece* piece, sf::Vector2i newSquare);
	bool pieceCanMoveTo(Piece* piece, sf::Vector2i newSquare);
	bool validMovePawn(Piece* piece, sf::Vector2i newSquare);
	bool validMoveRook(Piece* piece, sf::Vector2i newSquare);
	bool validMoveKnight(Piece* piece, sf::Vector2i newSquare);
	bool validMoveBishop(Piece* piece, sf::Vector2i newSquare);
	bool validMoveQueen(Piece* piece, sf::Vector2i newSquare);
	bool validMoveKing(Piece* piece, sf::Vector2i newSquare);
	bool isSquareInCheck(sf::Vector2i sq, char color);
	bool isKingInCheck(char color);
	bool willMoveCauseCheckForColor(Piece* p, sf::Vector2i newSquare);
	bool hasRookMoved(sf::Vector2i startingSquare);

	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> getPossibleMoves();

	void logMove(sf::Vector2i oldSquare, sf::Vector2i newSquare);
	void saveLog(std::string fileName = "log.txt");
};
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
// Empty: e

class Board
{
public:
	Board();

	bool makeMove(sf::Vector2i oldSquare, sf::Vector2i newSquare);
	bool checkGameOver();

	~Board();

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

	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> log;

	bool whiteTurn = true;
	bool whiteVictory = false;
	bool blackVictory = false;
	bool draw = false;

	void placeStartingPieces();
	void placePiece(Type id, Color color, sf::Vector2i square);
	bool makeMove(Piece& piece, sf::Vector2i newSquare);
	void movePiece(Piece& piece, sf::Vector2i newSquare);
	void removeAllMovedUpTwo();
	void castle(Piece& piece, sf::Vector2i newSquare);
	void promotePawns();
	void changeTurn();
	void setTurn(char turn);

	bool validMove(Piece* piece, sf::Vector2i newSquare);
	bool pieceCanMoveTo(Piece* piece, sf::Vector2i newSquare);
	bool validMovePawn(Piece* piece, sf::Vector2i newSquare);
	bool validMoveRook(Piece* piece, sf::Vector2i newSquare);
	bool validMoveKnight(Piece* piece, sf::Vector2i newSquare);
	bool validMoveBishop(Piece* piece, sf::Vector2i newSquare);
	bool validMoveQueen(Piece* piece, sf::Vector2i newSquare);
	bool validMoveKing(Piece* piece, sf::Vector2i newSquare);
	bool isSquareInCheck(sf::Vector2i sq, Color color);
	bool isKingInCheck(Color color);
	bool willMoveCauseCheckForColor(Piece* p, sf::Vector2i newSquare);
	bool hasRookMoved(sf::Vector2i startingSquare);
	bool moveIsCastling(Piece* piece, sf::Vector2i newSquare);

	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> getPossibleMoves();

	void logMove(sf::Vector2i oldSquare, sf::Vector2i newSquare);
	void saveLog(std::string fileName = "log.txt");
};
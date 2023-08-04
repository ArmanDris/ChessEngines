#pragma once
#include <SFML/Graphics.hpp>
#include "Piece.h"
#include "Ui.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <windows.h>    
#include <random>

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
	// Constructor
	Board(sf::RenderWindow* w);
	//Board(sf::RenderWindow* w, Board engineOne);
	//Board(sf::RenderWindow* w, Board engineOne, Board engineTwo);

	void drawBoard();
	bool makeMove(sf::Vector2i oldSquare, sf::Vector2i newSquare);
	bool makeRandomBlackMove();
	bool makeRandomWhiteMove();

	void hold(sf::Vector2f p);
	void drop(sf::Vector2f p);
	void hover(sf::Vector2f p);

	bool isGameOver();
	void resetBoard();

	// Destructor
	~Board();

private:
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

	sf::RenderWindow* window;

	sf::Texture black_pawnTexture;
	sf::Texture black_rookTexture;
	sf::Texture black_knightTexture;
	sf::Texture black_bishopTexture;
	sf::Texture black_queenTexture;
	sf::Texture black_kingTexture;

	sf::Texture white_pawnTexture;
	sf::Texture white_rookTexture;
	sf::Texture white_knightTexture;
	sf::Texture white_bishopTexture;
	sf::Texture white_queenTexture;
	sf::Texture white_kingTexture;

	sf::Texture dot_texture;
	sf::Texture circle_texture;

	sf::Font font;

	bool whiteTurn = true;
	bool whiteVictory = false;
	bool blackVictory = false;
	bool draw = false;

	Piece* holdingPiece = nullptr;
	sf::Vector2f mouseCoords = sf::Vector2f(-1, -1);

	void drawPlayerTurn();
	void drawPotenialMoves();
	void placeStartingPieces();
	void placePiece(char id, char color, sf::Vector2i square);
	bool makeMove(Piece* piece, sf::Vector2i newSquare);
	void movePiece(Piece* piece, sf::Vector2i newSquare);
	void takePiece(sf::Vector2i square);
	void removeAllMovedUpTwo();
	void castle(Piece* piece, sf::Vector2i newSquare);
	void promotePawns();

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

	Piece* getPieceAt(sf::Vector2f p);
	sf::Vector2i getSquareAt(sf::Vector2f p);
	void changeTurn();
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> getPossibleMoves();
	void logMove(sf::Vector2i oldSquare, sf::Vector2i newSquare);
	void saveLog(std::string fileName = "log.txt");
};
#pragma once
#include "Board.h"
#include "Engine.h"

class GameBoard : Board
{
public:
	GameBoard();
	void setPlayer(Engine* player, char color);
	void drawBoard(sf::RenderWindow* w);
	void handleCPUMoves();
	void resetBoard();

	void hold(sf::RenderWindow* w, sf::Vector2f p);
	void drop(sf::RenderWindow* w, sf::Vector2f p);
	void hover(sf::Vector2f p);

	~GameBoard();

private:
	Engine* white_player = nullptr;
	Engine* black_player = nullptr;

	sf::Clock c;
	int move_delay_ms = 1000;
	int win_delay_ms = 1000;

	void makeWhiteMove(Piece* board_copy[8][8]);
	void makeBlackMove(Piece* board_copy[8][8]);

	Piece* holdingPiece = nullptr;
	sf::Vector2f mouseCoords = sf::Vector2f(-1, -1);

	sf::Texture* getTexture(Piece* p);
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

	void drawPlayerTurn(sf::RenderWindow* w);
	void drawPotenialMoves(sf::RenderWindow* w);
	Piece* getPieceAt(sf::RenderWindow* w, sf::Vector2f p);
	sf::Vector2i getSquareAt(sf::RenderWindow* w, sf::Vector2f p);
};


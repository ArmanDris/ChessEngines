#pragma once
#include "Board.h"
#include "Engine.h"

class GameBoard : public Board
{
public:
	GameBoard();
	GameBoard(Engine* white, Engine* black);
	void setPlayer(Engine* player, Color color);
	void drawBoard(sf::RenderWindow* w) const;
	void preformCPUMoves();
	void triggerMove();
	void resetBoard();

	void hold(sf::RenderWindow* w, sf::Vector2f p);
	void drop(sf::RenderWindow* w, sf::Vector2f p);
	void hover(sf::Vector2f p);

	~GameBoard();

private:
	Engine* white_player = nullptr;
	Engine* black_player = nullptr;

	sf::Clock c;
	int move_delay_ms = 1500;
	int win_delay_ms = 10000;

	void makeWhiteMove();
	void makeBlackMove();

	Piece holdingPiece = Piece();
	sf::Vector2f mouseCoords = sf::Vector2f(-1, -1);
	sf::Vector2i holdingPiece_original_square = sf::Vector2i(-1, -1);

	const sf::Texture* getTexture(sf::Vector2i sq) const;
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

	void drawPlayerTurn(sf::RenderWindow* w) const;
	void drawPotenialMoves(sf::RenderWindow* w) const;
	sf::Vector2i getSquareAt(sf::RenderWindow* w, sf::Vector2f p) const;
};


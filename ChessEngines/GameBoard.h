#pragma once
#include "Board.h"
#include "Engine.h"

class GameBoard
{
public:
	GameBoard();
	GameBoard(Engine* white, Engine* black);
	void importFEN(std::string fen);
	void setPlayer(Engine* player, Color color);
	void drawBoard(sf::RenderWindow& w);
	void preformCPUMoves(int move_delay_ms = 1000);
	void triggerMove();
	void start_tournement(int num_games);

	void hold(sf::RenderWindow& w, sf::Vector2f p);
	void drop(sf::RenderWindow& w, sf::Vector2f p);
	void hover(sf::Vector2f p);

	void undoMove();

private:
	Board b = Board();
	Engine* white_player = nullptr;
	Engine* black_player = nullptr;

	sf::Clock c;

	Piece holdingPiece = Piece();
	sf::Vector2f mouseCoords = sf::Vector2f(-1, -1);
	sf::Vector2i holdingPiece_original_square = sf::Vector2i(-1, -1);

	int square_length = 75;

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
	const sf::Texture* getTexture(sf::Vector2i sq) const;

	sf::Font font;

	void makeWhiteMove();
	void makeBlackMove();

	void resetBoard();

	void drawPiece(sf::RenderWindow& w, sf::Vector2f coords, const sf::Texture* t) const;

	void drawPlayerTurn(sf::RenderWindow& w) const;
	void drawPotenialMoves(sf::RenderWindow& w);
	sf::Vector2i getSquareAt(sf::RenderWindow& w, sf::Vector2f p) const;
	sf::Vector2f getTopLeftCorner(sf::RenderWindow& w, sf::Vector2i square) const;
};

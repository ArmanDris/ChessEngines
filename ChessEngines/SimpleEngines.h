#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include "Board.h"
#include "Piece.h"
#include "Engine.h"
#include <random>

class EngineHelpers : public Board
{
protected:
	bool wins_game(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool takes_hanging(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool supportsUndefended(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool moveOutOfDanger(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool checks_enemy(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool takes(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;

	bool trades_positive(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool protects_high_value(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool trades_equal_value(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
	bool moveIsPushPawn(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;

	int get_lowest_attacker(sf::Vector2i sq, Color c) const;
	bool does_move_hang_pice(sf::Vector2i oldSquare, sf::Vector2i newSquare) const;
};

class RandomEngine : public Board, public Engine
{
public:
	std::pair<sf::Vector2i, sf::Vector2i> returnMove(const Board& b) override;
};

class DrunkEngine : public Engine, public EngineHelpers
{
public:
	std::pair<sf::Vector2i, sf::Vector2i> returnMove(const Board& b) override;
};

class HippieEngine : public Engine, public EngineHelpers
{
public:
	std::pair<sf::Vector2i, sf::Vector2i> returnMove(const Board& b) override;
};

class BotFeasterEngine : public Engine, public EngineHelpers
{
	std::pair<sf::Vector2i, sf::Vector2i> returnMove(const Board& b) override;
};

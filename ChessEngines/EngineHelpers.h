#pragma once
#include "Engine.h"

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
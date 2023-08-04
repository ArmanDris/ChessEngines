#pragma once
#include <SFML/Graphics.hpp>

class Ui
{
public:
	static sf::Vector2f getTopLeftCorner(sf::Vector2i square);
	static int squareLength;
private:
	Ui() {} // Private constructor to prevent direct instantiation
	Ui(const Ui&) = delete; // Disable copy constructor
	Ui& operator=(const Ui&) = delete; // Disable assignment operator
};


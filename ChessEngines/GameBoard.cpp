#include "GameBoard.h"

GameBoard::GameBoard() {
	if (!font.loadFromFile("Nunito-VariableFont_wght.ttf")) { std::cout << "Error!"; }

	if (!black_pawnTexture.loadFromFile("black_pawn.png")) { std::cout << "Error!"; }
	if (!black_rookTexture.loadFromFile("black_rook.png")) { std::cout << "Error!"; }
	if (!black_knightTexture.loadFromFile("black_knight.png")) { std::cout << "Error!"; }
	if (!black_bishopTexture.loadFromFile("black_bishop.png")) { std::cout << "Error!"; }
	if (!black_queenTexture.loadFromFile("black_queen.png")) { std::cout << "Error!"; }
	if (!black_kingTexture.loadFromFile("black_king.png")) { std::cout << "Error!"; }

	if (!white_pawnTexture.loadFromFile("white_pawn.png")) { std::cout << "Error!"; }
	if (!white_rookTexture.loadFromFile("white_rook.png")) { std::cout << "Error!"; }
	if (!white_knightTexture.loadFromFile("white_knight.png")) { std::cout << "Error!"; }
	if (!white_bishopTexture.loadFromFile("white_bishop.png")) { std::cout << "Error!"; }
	if (!white_queenTexture.loadFromFile("white_queen.png")) { std::cout << "Error!"; }
	if (!white_kingTexture.loadFromFile("white_king.png")) { std::cout << "Error!"; }

	if (!circle_texture.loadFromFile("circle.png")) { std::cout << "Error!"; }
	if (!dot_texture.loadFromFile("dot.png")) { std::cout << "Error!"; }
}

void GameBoard::setPlayer(RandomEngine* player, char color)
{
	if (color == 'w') {
		white_player = player;
	}
	else {
		black_player = player;
	}
}

void GameBoard::drawBoard(sf::RenderWindow* w) {

	sf::RectangleShape square(sf::Vector2f(Ui::squareLength, Ui::squareLength));
	// Draw squares first
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if ((i + j) % 2 == 0) {
				square.setFillColor(sf::Color(240, 218, 181));
			}
			else {
				square.setFillColor(sf::Color(181, 135, 99));
			}

			square.setPosition(Ui::getTopLeftCorner(w, sf::Vector2i(i, j)));
			w->draw(square);
		}
	}

	drawPlayerTurn(w);
	drawPotenialMoves(w);

	// Draw pieces next
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j]) {
				board[i][j]->drawPiece(w, getTexture(board[i][j]));
			}
		}
	}

	// Draw holding piece last so it is on top
	if (holdingPiece) {
		sf::Vector2f adjustecdCoords = sf::Vector2f(mouseCoords.x - Ui::squareLength / 2, mouseCoords.y - Ui::squareLength / 2);
		holdingPiece->drawPiece(w, getTexture(holdingPiece), adjustecdCoords);
	}

}

void GameBoard::resetBoard() {
	// Free the memory of every object still on the board
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; ++j) {
			if (board[i][j]) {
				delete board[i][j];
				board[i][j] = nullptr;
				placeStartingPieces();
			}
		}
	}

	whiteTurn = true;
	whiteVictory = false;
	blackVictory = false;
	draw = false;

	holdingPiece = nullptr;
}

void GameBoard::hold(sf::RenderWindow* w, sf::Vector2f p) {
	holdingPiece = getPieceAt(w, p);
	if (!holdingPiece) { return; }
	if (holdingPiece) { holdingPiece->visible = false; }
}

void GameBoard::drop(sf::RenderWindow* w, sf::Vector2f p) {
	if (!holdingPiece) { return; }
	holdingPiece->visible = true;

	sf::Vector2i newSquare = getSquareAt(w, mouseCoords);
	sf::Vector2i oldSquare = holdingPiece->getSquare();

	// If placed on invalid square, return
	if (newSquare.x == -1) {
		holdingPiece = nullptr;
		return;
	}

	makeMove(holdingPiece, newSquare);

	// Reset holding piece
	holdingPiece = nullptr;
}

void GameBoard::hover(sf::Vector2f p) {
	mouseCoords = p;
}

GameBoard::~GameBoard()
{
}

void GameBoard::getWhiteMove()
{
	if (!white_player) return;
	makeMove(white_player->makeMove());
}

void GameBoard::getBlackMove()
{
}

sf::Texture* GameBoard::getTexture(Piece* p) {
	char piece = p->getId();
	char color = p->getColor();

	switch (piece) {
	case 'p':
		if (color == 'w') return &white_pawnTexture;
		else			  return &black_pawnTexture;
		break;
	case 'r':
		if (color == 'w') return &white_rookTexture;
		else			  return &black_rookTexture;
		break;
	case 'n':
		if (color == 'w') return &white_knightTexture;
		else			  return &black_knightTexture;
		break;
	case 'b':
		if (color == 'w') return &white_bishopTexture;
		else			  return &black_bishopTexture;
		break;
	case 'q':
		if (color == 'w') return &white_queenTexture;
		else			  return &black_queenTexture;
		break;
	case 'k':
		if (color == 'w') return &white_kingTexture;
		else			  return &black_kingTexture;
		break;
	default:
		std::cout << "Error! Invalid piece id: " << piece << std::endl;
		return nullptr;
		break;
	}
}

void GameBoard::drawPlayerTurn(sf::RenderWindow* w) {
	sf::Sprite king;
	if (whiteTurn) { king.setTexture(white_kingTexture); }
	else { king.setTexture(black_kingTexture); }
	king.setScale(4, 4);

	sf::Text text;
	text.setFont(font);
	if (whiteTurn) { text.setString(" White's Turn"); }
	else { text.setString(" Black's Turn"); }
	if (whiteVictory) { text.setString(" White Wins!!!"); }
	if (blackVictory) { text.setString(" Black Wins!!!"); }
	if (draw) { text.setString(" Draw"); }
	text.setCharacterSize(48);
	text.setStyle(sf::Text::Bold);
	text.setFillColor(sf::Color::White);
	float textWidth = text.getLocalBounds().width;
	float kingWidth = king.getGlobalBounds().width;
	float overallWidth = textWidth + kingWidth;
	float centerOfWindow = w->getSize().x / 2;

	king.setPosition(sf::Vector2f(centerOfWindow - overallWidth / 2, 20));
	w->draw(king);
	text.setPosition(sf::Vector2f(centerOfWindow - overallWidth / 2 + kingWidth, 20));
	w->draw(text);
}

void GameBoard::drawPotenialMoves(sf::RenderWindow* w) {
	if (!holdingPiece) { return; }

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (!validMove(holdingPiece, sf::Vector2i(i, j))) { continue; }

			sf::Sprite s;
			if (board[i][j]) { s.setTexture(circle_texture); }
			else { s.setTexture(dot_texture); }
			s.setPosition(Ui::getTopLeftCorner(w, sf::Vector2i(i, j)));
			w->draw(s);
		}
	}
}

Piece* GameBoard::getPieceAt(sf::RenderWindow* w, sf::Vector2f p) {
	// Cycle through board
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (!board[i][j]) { continue; }
			if (board[i][j]->getBoundingBox(w).contains(p)) {
				return board[i][j];
			}
		}
	}

	return nullptr;
}

sf::Vector2i GameBoard::getSquareAt(sf::RenderWindow* w, sf::Vector2f p) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			sf::FloatRect square(Ui::getTopLeftCorner(w, sf::Vector2i(i, j)), sf::Vector2f(Ui::squareLength, Ui::squareLength));
			if (square.contains(p)) {
				return sf::Vector2i(i, j);
			}
		}
	}

	return sf::Vector2i(-1, -1);
}
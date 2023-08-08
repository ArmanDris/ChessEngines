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

void GameBoard::setPlayer(Engine* player, Color color)
{
	if (color == Color::White) white_player = player;
	if (color == Color::Black) black_player = player;
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
	// !!! drawPotenialMoves(w);

	// Draw pieces next
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j]) {
				board[i][j].drawPiece(w, getTexture(&board[i][j]));
			}
		}
	}

	// Draw holding piece last so it is on top
	if (holdingPiece) {
		sf::Vector2f adjustecdCoords = sf::Vector2f(mouseCoords.x - Ui::squareLength / 2, mouseCoords.y - Ui::squareLength / 2);
		holdingPiece->drawPiece(w, getTexture(holdingPiece), adjustecdCoords);
	}

}

void GameBoard::handleCPUMoves()
{
	double elapsed_time_ms = c.getElapsedTime().asMilliseconds();

	if (checkGameOver() && elapsed_time_ms > move_delay_ms + win_delay_ms) resetBoard();
	if (checkGameOver()) return;

	if (elapsed_time_ms < move_delay_ms) return;

	if      (white_player &&  whiteTurn) { makeWhiteMove(); }
	else if (black_player && !whiteTurn) { makeBlackMove(); }

	c.restart();
}

void GameBoard::resetBoard() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; ++j) {
			if (board[i][j]) {
				board[i][j] = Piece();
			}
		}
	}

	placeStartingPieces();

	whiteTurn = true;
	whiteVictory = false;
	blackVictory = false;
	draw = false;

	holdingPiece = nullptr;
}

void GameBoard::hold(sf::RenderWindow* w, sf::Vector2f p) {
	holdingPiece = getPieceAt(w, p);
	if (!holdingPiece) { return; }

	holdingPiece->visible = false;
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

void GameBoard::makeWhiteMove()
{
	auto move = white_player->returnMove(board, 'w');
	makeMove(move.first, move.second);
}

void GameBoard::makeBlackMove()
{
	auto move = black_player->returnMove(board, 'b');
	std::cout << move.first.x << "," << move.first.y << " " << move.second.x << "," << move.second.y << std::endl;
	makeMove(move.first, move.second);
}

sf::Texture* GameBoard::getTexture(Piece* p) {
	Type piece = p->getId();
	Color color = p->getColor();

	switch (piece) {
	case Type::Pawn:
		if (color == Color::White) return &white_pawnTexture;
		if (color == Color::Black) return &black_pawnTexture;
		break;
	case Type::Rook:
		if (color == Color::White) return &white_rookTexture;
		if (color == Color::Black) return &black_rookTexture;
		break;
	case Type::Knight:
		if (color == Color::White) return &white_knightTexture;
		if (color == Color::Black) return &black_knightTexture;
		break;
	case Type::Bishop:
		if (color == Color::White) return &white_bishopTexture;
		if (color == Color::Black) return &black_bishopTexture;
		break;
	case Type::Queen:
		if (color == Color::White) return &white_queenTexture;
		if (color == Color::Black) return &black_queenTexture;
		break;
	case Type::King:
		if (color == Color::White) return &white_kingTexture;
		if (color == Color::Black) return &black_kingTexture;
		break;
	default:
		std::cout << "Error! Invalid piece id: " << typeToString(piece) << std::endl;
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
			if (board[i][j] && board[i][j].getBoundingBox(w).contains(p)) {
				return &board[i][j];
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
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

GameBoard::GameBoard(Engine* white, Engine* black) : GameBoard()
{
	white_player = white;
	black_player = black;
}

void GameBoard::setPlayer(Engine* player, Color color)
{
	if (color == Color::White) white_player = player;
	if (color == Color::Black) black_player = player;
}

void GameBoard::drawBoard(sf::RenderWindow* w) const {

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
			sf::Vector2i sq(i, j);
			if (board[i][j] && sq != holdingPiece_original_square) {
				board[i][j].drawPiece(w, Ui::getTopLeftCorner(w, sq), getTexture(sf::Vector2i(i, j)));
			}
		}
	}

	// Draw holding piece last so it is on top
	if (holdingPiece) {
		sf::Vector2f adjustecdCoords = sf::Vector2f(mouseCoords.x - Ui::squareLength / 2, mouseCoords.y - Ui::squareLength / 2);
		holdingPiece.drawPiece(w, adjustecdCoords, getTexture(holdingPiece_original_square));
	}

}

void GameBoard::preformCPUMoves(int move_delay_ms)
{

	double elapsed_time_ms = c.getElapsedTime().asMilliseconds();

	if (checkGameOver()) return;

	if (elapsed_time_ms < move_delay_ms) return;

	if      (white_player &&  whiteTurn) makeWhiteMove();
	else if (black_player && !whiteTurn) makeBlackMove();

	c.restart();
}

void GameBoard::triggerMove()
{
	if (checkGameOver()) return;

	if      (white_player &&  whiteTurn)  { makeWhiteMove(); }
	else if (black_player && !whiteTurn) { makeBlackMove(); }
}

void GameBoard::start_tournement(int num_games)
{
	int white_wins = 0;
	int black_wins = 0;
	int draws = 0;

	GameBoard tnmnt_board(white_player, black_player);

	for (int i = 0; i < num_games; i++) {
		tnmnt_board.resetBoard();
		while (!tnmnt_board.checkGameOver()) {
			if (tnmnt_board.whiteTurn) tnmnt_board.makeWhiteMove();
			else		               tnmnt_board.makeBlackMove();
		}
		std::cout << "Game " << i + 1 << " complete!\n";
		if (tnmnt_board.whiteVictory) white_wins++;
		if (tnmnt_board.blackVictory) black_wins++;
		if (tnmnt_board.draw) draws++;
	}

	std::cout << "White wins: " << white_wins << "\n";
	std::cout << "Black wins: " << black_wins << "\n";
	std::cout << "Draws: " << draws << "\n";
}

void GameBoard::resetBoard() {
	*this = GameBoard(white_player, black_player);
}

void GameBoard::hold(sf::RenderWindow* w, sf::Vector2f p) {
	sf::Vector2i clicked_square = getSquareAt(w, p);
	if (clicked_square.x == -1) { return; }

	holdingPiece = Piece(board[clicked_square.x][clicked_square.y]);
	holdingPiece_original_square = clicked_square;
}

void GameBoard::drop(sf::RenderWindow* w, sf::Vector2f p) {
	if (!holdingPiece) { return; }

	sf::Vector2i newSquare = getSquareAt(w, mouseCoords);
	sf::Vector2i oldSquare = holdingPiece_original_square;

	// If placed on valid square, make move
	if (newSquare.x != -1) makeMove(oldSquare, newSquare);

	// Reset holding piece
	holdingPiece = Piece();
	holdingPiece_original_square = sf::Vector2i(-1, -1);
}

void GameBoard::hover(sf::Vector2f p) {
	mouseCoords = p;
}

GameBoard::~GameBoard()
{
}

void GameBoard::makeWhiteMove()
{
	auto move = white_player->returnMove(*this);
	makeMove(move.first, move.second);
}

void GameBoard::makeBlackMove()
{
	auto move = black_player->returnMove(*this);
	makeMove(move.first, move.second);
}

const sf::Texture* GameBoard::getTexture(sf::Vector2i sq) const {
	const Type piece = board[sq.x][sq.y].getType();
	const Color color = board[sq.x][sq.y].getColor();

	const sf::Texture* texture = nullptr;

	switch (piece) {
	case Type::Pawn:
		if (color == Color::White) texture = &white_pawnTexture;
		if (color == Color::Black) texture = &black_pawnTexture;
		break;
	case Type::Rook:
		if (color == Color::White) texture = &white_rookTexture;
		if (color == Color::Black) texture = &black_rookTexture;
		break;
	case Type::Knight:
		if (color == Color::White) texture = &white_knightTexture;
		if (color == Color::Black) texture = &black_knightTexture;
		break;
	case Type::Bishop:
		if (color == Color::White) texture = &white_bishopTexture;
		if (color == Color::Black) texture = &black_bishopTexture;
		break;
	case Type::Queen:
		if (color == Color::White) texture = &white_queenTexture;
		if (color == Color::Black) texture = &black_queenTexture;
		break;
	case Type::King:
		if (color == Color::White) texture = &white_kingTexture;
		if (color == Color::Black) texture = &black_kingTexture;
		break;
	default:
		std::cout << "Error! Invalid piece id: " << typeToString(piece) << std::endl;
		texture = nullptr;
		break;
	}

	return texture;
}

void GameBoard::drawPlayerTurn(sf::RenderWindow* w) const  {
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

void GameBoard::drawPotenialMoves(sf::RenderWindow* w) const
{
	if (!holdingPiece) { return; }

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (!legal_move(holdingPiece_original_square, sf::Vector2i(i, j))) { continue; }

			sf::Sprite s;
			if (board[i][j]) { s.setTexture(circle_texture); }
			else { s.setTexture(dot_texture); }
			s.setPosition(Ui::getTopLeftCorner(w, sf::Vector2i(i, j)));
			w->draw(s);
		}
	}
}

sf::Vector2i GameBoard::getSquareAt(sf::RenderWindow* w, sf::Vector2f p) const  {
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
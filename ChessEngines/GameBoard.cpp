#include "GameBoard.h"

GameBoard::GameBoard() {
	if (!font.loadFromFile("Nunito-VariableFont_wght.ttf")) { std::cout << "Error!"; }

	black_pawnTexture.loadFromFile("black_pawn.png");
	black_rookTexture.loadFromFile("black_rook.png");
	black_knightTexture.loadFromFile("black_knight.png");
	black_bishopTexture.loadFromFile("black_bishop.png");
	black_queenTexture.loadFromFile("black_queen.png");
	black_kingTexture.loadFromFile("black_king.png");

	white_pawnTexture.loadFromFile("white_pawn.png");
	white_rookTexture.loadFromFile("white_rook.png");
	white_knightTexture.loadFromFile("white_knight.png");
	white_bishopTexture.loadFromFile("white_bishop.png");
	white_queenTexture.loadFromFile("white_queen.png");
	white_kingTexture.loadFromFile("white_king.png");

	circle_texture.loadFromFile("circle.png");
	dot_texture.loadFromFile("dot.png");
}

GameBoard::GameBoard(Engine* white, Engine* black) : GameBoard()
{
	white_player = white;
	black_player = black;
}

void GameBoard::importFEN(std::string fen)
{
	b.importFEN(fen);
}

void GameBoard::setPlayer(Engine* player, Color color)
{
	if (color == Color::White) white_player = player;
	if (color == Color::Black) black_player = player;
}

void GameBoard::drawBoard(sf::RenderWindow& w) 
{
	sf::RectangleShape square(sf::Vector2f(square_length, square_length));
	// Draw squares first
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if ((i + j) % 2 == 0) {
				square.setFillColor(sf::Color(240, 218, 181));
			}
			else {
				square.setFillColor(sf::Color(181, 135, 99));
			}
			square.setPosition(getTopLeftCorner(w, sf::Vector2i(i, j)));
			w.draw(square);

			// If the square is the last move, highlight it
			auto last_move = b.getLastMove();
			if (last_move.first.x == -1) continue;
			if (last_move.first == sf::Vector2i(i, j) || last_move.second == sf::Vector2i(i, j)) {
				square.setFillColor(sf::Color(255, 255, 0, 50));
				w.draw(square);
			}
		}
	}

	drawPlayerTurn(w);
	drawPotenialMoves(w);

	// Draw pieces next
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			sf::Vector2i sq(i, j);
			if (b.getPiece(sf::Vector2i(i, j)) && sq != holdingPiece_original_square) {
				drawPiece(w, getTopLeftCorner(w, sq), getTexture(sf::Vector2i(i, j)));
			}
		}
	}

	// Draw holding piece last so it is on top
	if (holdingPiece) {
		sf::Vector2f adjustecdCoords = sf::Vector2f(mouseCoords.x - square_length / 2, mouseCoords.y - square_length / 2);
		drawPiece(w, adjustecdCoords, getTexture(holdingPiece_original_square));
	}

}

void GameBoard::preformCPUMoves(int move_delay_ms)
{
	double elapsed_time_ms = c.getElapsedTime().asMilliseconds();

	// Uncomment if you want the game to restart 10 seconds after game is finished
	//if (b.isGameOver() && elapsed_time_ms > move_delay_ms + 5000) { resetBoard(); }
	if (b.isGameOver()) return;

	if (elapsed_time_ms < move_delay_ms) return;

	if      (white_player &&  b.isWhiteTurn()) makeWhiteMove();
	else if (black_player && !b.isWhiteTurn()) makeBlackMove();

	c.restart();
}

void GameBoard::triggerMove()
{
	//if (checkGameOver()) return;

	if		(white_player &&  b.isWhiteTurn()) { makeWhiteMove(); }
	else if (black_player && !b.isWhiteTurn()) { makeBlackMove(); }
}

void GameBoard::start_tournement(int num_games)
{
	int white_wins = 0;
	int black_wins = 0;
	int draws = 0;

	GameBoard tnmnt_board(white_player, black_player);

	for (int i = 0; i < num_games; i++) {
		tnmnt_board.resetBoard();
		while (!tnmnt_board.b.isGameOver()) {
			if (tnmnt_board.b.isWhiteTurn()) tnmnt_board.makeWhiteMove();
			else		                     tnmnt_board.makeBlackMove();
		}
		std::cout << "Game " << i + 1 << " complete!\n";
		if (tnmnt_board.b.isWhiteVictory()) white_wins++;
		if (tnmnt_board.b.isBlackVictory()) black_wins++;
		if (tnmnt_board.b.isDraw()) draws++;
	}

	std::cout << "White wins: " << white_wins << "\n";
	std::cout << "Black wins: " << black_wins << "\n";
	std::cout << "Draws: " << draws << "\n";
}

void GameBoard::resetBoard() 
{
	*this = GameBoard(white_player, black_player);
}

void GameBoard::drawPiece(sf::RenderWindow& w, sf::Vector2f coords, const sf::Texture* t) const 
{
	sf::Sprite s;
	s.setTexture(*t);
	float scale_factor = static_cast<float>(square_length) / std::max(s.getLocalBounds().width, s.getLocalBounds().height);
	s.setScale(scale_factor, scale_factor);
	s.setPosition(coords);
	w.draw(s);
}

void GameBoard::hold(sf::RenderWindow& w, sf::Vector2f p) {
	sf::Vector2i clicked_square = getSquareAt(w, p);
	if (clicked_square.x == -1) { return; }

	holdingPiece = Piece(b.getPiece(sf::Vector2i(clicked_square.x, clicked_square.y)));
	holdingPiece_original_square = clicked_square;
}

void GameBoard::drop(sf::RenderWindow& w, sf::Vector2f p) {
	if (!holdingPiece) { return; }

	sf::Vector2i newSquare = getSquareAt(w, mouseCoords);
	sf::Vector2i oldSquare = holdingPiece_original_square;

	// If placed on valid square, make move
	if (newSquare.x != -1) b.makeMove(oldSquare, newSquare);

	// Reset holding piece
	holdingPiece = Piece();
	holdingPiece_original_square = sf::Vector2i(-1, -1);
}

void GameBoard::hover(sf::Vector2f p) 
{
	mouseCoords = p;
}

void GameBoard::undoMove()
{
	b.undoMove();
}

void GameBoard::makeWhiteMove()
{
	auto move = white_player->returnMove(b);
	b.makeMove(move.first, move.second);
}

void GameBoard::makeBlackMove()
{
	auto move = black_player->returnMove(b);
	b.makeMove(move.first, move.second);
}

const sf::Texture* GameBoard::getTexture(sf::Vector2i sq) const {
	const Type  piece = b.getPiece(sq).getType();
	const Color color = b.getPiece(sq).getColor();

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
		std::cout << "Error! Invalid piece id, returning null" << std::endl;
		texture = nullptr;
		break;
	}

	return texture;
}

void GameBoard::drawPlayerTurn(sf::RenderWindow& w) const {
	sf::Sprite king;
	if (b.isWhiteTurn()) { king.setTexture(white_kingTexture); }
	else { king.setTexture(black_kingTexture); }
	king.setScale(4, 4);

	sf::Text text;
	text.setFont(font);
	if (b.isWhiteTurn()) { text.setString(" White's Turn"); }
	else { text.setString(" Black's Turn"); }
	if (b.isWhiteVictory()) { text.setString(" White Wins!!!"); }
	if (b.isBlackVictory()) { text.setString(" Black Wins!!!"); }
	if (b.isDraw()) { text.setString(" Draw"); }
	text.setCharacterSize(48);
	text.setStyle(sf::Text::Bold);
	text.setFillColor(sf::Color::White);
	float textWidth = text.getLocalBounds().width;
	float kingWidth = king.getGlobalBounds().width;
	float overallWidth = textWidth + kingWidth;
	float centerOfWindow = w.getSize().x / 2;

	king.setPosition(sf::Vector2f(centerOfWindow - overallWidth / 2, 20));
	w.draw(king);
	text.setPosition(sf::Vector2f(centerOfWindow - overallWidth / 2 + kingWidth, 20));
	w.draw(text);
}

void GameBoard::drawPotenialMoves(sf::RenderWindow& w)
{
	if (!holdingPiece) return;

	std::vector <std::pair<sf::Vector2i, sf::Vector2i>> moves = b.getMoves();

	for (auto move : moves) {
		if (holdingPiece_original_square == move.first) {
			sf::Sprite s;
			if (b.getPiece(move.second)) { s.setTexture(circle_texture); }
			else { s.setTexture(dot_texture); }
			s.setPosition(getTopLeftCorner(w, move.second));
			w.draw(s);
		}
	}
}

sf::Vector2i GameBoard::getSquareAt(sf::RenderWindow& w, sf::Vector2f p) const {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			sf::FloatRect square(getTopLeftCorner(w, sf::Vector2i(i, j)), sf::Vector2f(square_length, square_length));
			if (square.contains(p)) {
				return sf::Vector2i(i, j);
			}
		}
	}

	return sf::Vector2i(-1, -1);
}

sf::Vector2f GameBoard::getTopLeftCorner(sf::RenderWindow& w, sf::Vector2i square) const
{
	sf::Vector2f centerOfWindow(w.getSize().x/2, w.getSize().y / 2);
	sf::Vector2f topRightOfWindow(w.getSize().x, 0);
	int boardLength = square_length * 8;
	sf::Vector2f topLeftBoardCorner(centerOfWindow.x - boardLength / 2, centerOfWindow.y - boardLength / 2);
	return sf::Vector2f(topLeftBoardCorner.x + square_length * square.x, topLeftBoardCorner.y + square_length * square.y);
}
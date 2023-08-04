#include "Board.h"
#include "Piece.h"

Board::Board(sf::RenderWindow* w) :window(w) {
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

	placeStartingPieces();
	drawBoard();
}

void Board::drawBoard() {

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

			square.setPosition(Ui::getTopLeftCorner(sf::Vector2i(i, j)));
			window->draw(square);
		}
	}

	drawPlayerTurn();
	drawPotenialMoves();

	// Draw pieces next
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j]) {
				board[i][j]->drawPiece();
			}
		}
	}

	// Draw holding piece last so it is on top
	if (holdingPiece) {
		sf::Vector2f adjustecdCoords = sf::Vector2f(mouseCoords.x - Ui::squareLength / 2, mouseCoords.y - Ui::squareLength / 2);
		holdingPiece->drawPiece(adjustecdCoords);
	}

}

bool Board::makeMove(sf::Vector2i oldSquare, sf::Vector2i newSquare) {
	return makeMove(board[oldSquare.x][oldSquare.y], newSquare);
}

void Board::hold(sf::Vector2f p) {
	holdingPiece = getPieceAt(p);
	if (!holdingPiece) { return; }
	if (holdingPiece) { holdingPiece->visible = false; }
}

void Board::drop(sf::Vector2f p) {
	if (!holdingPiece) { return; }
	holdingPiece->visible = true;

	sf::Vector2i newSquare = getSquareAt(mouseCoords);
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

void Board::hover(sf::Vector2f p) {
	mouseCoords = p;
}

Board::~Board() {
	saveLog();

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; ++j) {
			if (board[i][j]) {
				delete board[i][j];
			}
		}
	}
}

void Board::drawPlayerTurn() {
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
	float centerOfWindow = window->getSize().x / 2;

	king.setPosition(sf::Vector2f(centerOfWindow - overallWidth / 2, 20));
	window->draw(king);
	text.setPosition(sf::Vector2f(centerOfWindow - overallWidth / 2 + kingWidth, 20));
	window->draw(text);
}

void Board::drawPotenialMoves() {
	if (!holdingPiece) { return; }

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (!validMove(holdingPiece, sf::Vector2i(i, j))) { continue; }

			sf::Sprite s;
			if (board[i][j]) { s.setTexture(circle_texture); }
			else { s.setTexture(dot_texture); }
			s.setPosition(Ui::getTopLeftCorner(sf::Vector2i(i, j)));
			window->draw(s);
		}
	}
}

void Board::placeStartingPieces() {
	placePiece('r', 'b', sf::Vector2i(0, 0));
	placePiece('n', 'b', sf::Vector2i(1, 0));
	placePiece('b', 'b', sf::Vector2i(2, 0));
	placePiece('q', 'b', sf::Vector2i(3, 0));
	placePiece('k', 'b', sf::Vector2i(4, 0));
	placePiece('b', 'b', sf::Vector2i(5, 0));
	placePiece('n', 'b', sf::Vector2i(6, 0));
	placePiece('r', 'b', sf::Vector2i(7, 0));
	placePiece('p', 'b', sf::Vector2i(0, 1));
	placePiece('p', 'b', sf::Vector2i(1, 1));
	placePiece('p', 'b', sf::Vector2i(2, 1));
	placePiece('p', 'b', sf::Vector2i(3, 1));
	placePiece('p', 'b', sf::Vector2i(4, 1));
	placePiece('p', 'b', sf::Vector2i(5, 1));
	placePiece('p', 'b', sf::Vector2i(6, 1));
	placePiece('p', 'b', sf::Vector2i(7, 1));
	placePiece('r', 'w', sf::Vector2i(0, 7));
	placePiece('n', 'w', sf::Vector2i(1, 7));
	placePiece('b', 'w', sf::Vector2i(2, 7));
	placePiece('q', 'w', sf::Vector2i(3, 7));
	placePiece('k', 'w', sf::Vector2i(4, 7));
	placePiece('b', 'w', sf::Vector2i(5, 7));
	placePiece('n', 'w', sf::Vector2i(6, 7));
	placePiece('r', 'w', sf::Vector2i(7, 7));
	placePiece('p', 'w', sf::Vector2i(0, 6));
	placePiece('p', 'w', sf::Vector2i(1, 6));
	placePiece('p', 'w', sf::Vector2i(2, 6));
	placePiece('p', 'w', sf::Vector2i(3, 6));
	placePiece('p', 'w', sf::Vector2i(4, 6));
	placePiece('p', 'w', sf::Vector2i(5, 6));
	placePiece('p', 'w', sf::Vector2i(6, 6));
	placePiece('p', 'w', sf::Vector2i(7, 6));
}

void Board::placePiece(char id, char color, sf::Vector2i square) {
	int x = square.x;
	int y = square.y;

	switch (id) {
	case 'p':
		if (color == 'w')
			board[x][y] = new Piece('p', 'w', &white_pawnTexture, window, square);
		else
			board[x][y] = new Piece('p', 'b', &black_pawnTexture, window, square);
		break;
	case 'r':
		if (color == 'w')
			board[x][y] = new Piece('r', 'w', &white_rookTexture, window, square);
		else
			board[x][y] = new Piece('r', 'b', &black_rookTexture, window, square);
		break;
	case 'n':
		if (color == 'w')
			board[x][y] = new Piece('n', 'w', &white_knightTexture, window, square);
		else
			board[x][y] = new Piece('n', 'b', &black_knightTexture, window, square);
		break;
	case 'b':
		if (color == 'w')
			board[x][y] = new Piece('b', 'w', &white_bishopTexture, window, square);
		else
			board[x][y] = new Piece('b', 'b', &black_bishopTexture, window, square);
		break;
	case 'q':
		if (color == 'w')
			board[x][y] = new Piece('q', 'w', &white_queenTexture, window, square);
		else
			board[x][y] = new Piece('q', 'b', &black_queenTexture, window, square);
		break;
	case 'k':
		if (color == 'w')
			board[x][y] = new Piece('k', 'w', &white_kingTexture, window, square);
		else
			board[x][y] = new Piece('k', 'b', &black_kingTexture, window, square);
		break;
	default:
		std::cout << "Invalid piece id: " << id << std::endl;
		break;
	}
}

// Returns true if a move is excecuted
bool Board::makeMove(Piece* piece, sf::Vector2i newSquare) {
	if (isGameOver()) { return false; }
	if (!validMove(piece, newSquare)) { return false; }

	sf::Vector2i oldSquare = piece->getSquare();

	// Special Pawn Logic (moving up two & en passant)
	piece->moved = true;
	removeAllMovedUpTwo();
	if (abs(oldSquare.y - newSquare.y) > 1) { piece->justMovedUpTwo = true; }

	// Specil King Logic (castling)
	if (piece->getId() == 'k' && abs(oldSquare.x - newSquare.x) > 1) {
		castle(piece, newSquare);
	}
	else {
		movePiece(piece, newSquare);
	}

	promotePawns();
	logMove(oldSquare, newSquare);
	changeTurn();
	return true;
}

// Castling invlolves moving multiple pieces soo all turn logic must be done in makeMove
void Board::movePiece(Piece* piece, sf::Vector2i newSquare) {
	sf::Vector2i oldSquare = piece->getSquare();

	if (newSquare == oldSquare) return;

	// Delete square about to be moved to
	delete board[newSquare.x][newSquare.y];
	// Set old square to nullptr
	board[oldSquare.x][oldSquare.y] = nullptr;
	// Move piece to new square
	board[newSquare.x][newSquare.y] = piece;
	// Update piece
	piece->setSquare(newSquare);
}

void Board::takePiece(sf::Vector2i square) {
	// Delete square about to be moved to
	delete board[square.x][square.y];
	// Set old square to nullptr
	board[square.x][square.y] = nullptr;
}

void Board::removeAllMovedUpTwo() {
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++)
			// if piece is a pawn and just moved up two
			if (board[x][y] != nullptr && board[x][y]->getId() == 'p' && board[x][y]->justMovedUpTwo)
				// set justMovedUpTwo to false
				board[x][y]->justMovedUpTwo = false;
	}
}

void Board::castle(Piece* piece, sf::Vector2i newSquare) {
	sf::Vector2i oldSquare = piece->getSquare();
	// If the king is trying to castle left
	if (newSquare.x < piece->getSquare().x) {
		Piece* rook = board[0][oldSquare.y];
		movePiece(rook, sf::Vector2i(3, oldSquare.y));
		movePiece(piece, sf::Vector2i(2, oldSquare.y));
	}

	// If the king is trying to castle right
	if (newSquare.x > piece->getSquare().x) {
		Piece* rook = board[7][oldSquare.y];
		movePiece(rook, sf::Vector2i(5, oldSquare.y));
		movePiece(piece, sf::Vector2i(6, oldSquare.y));
	}
}

// Sets whiteVictory or blackVictory to true if the player has no valid moves
bool Board::isGameOver() {
	int numMoves = getPossibleMoves().size();

	if (whiteTurn && numMoves == 0 && isKingInCheck('w')) { blackVictory = true; return true; }
	if (!whiteTurn && numMoves == 0 && isKingInCheck('b')) { whiteVictory = true; return true; }

	if (whiteTurn && numMoves == 0 && !isKingInCheck('w')) { draw = true; return true; }
	if (!whiteTurn && numMoves == 0 && !isKingInCheck('b')) { draw = true; return true; }

	// loop through board
	int numPieces = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j]) { numPieces++; }
		}
	}
	if (numPieces == 2) { draw = true; return true; }

	return false;
}

void Board::promotePawns() {
	for (int x = 0; x < 8; x++) {
		if (board[x][0] && board[x][0]->getId() == 'p') {
			delete board[x][0];
			board[x][0] = new Piece('q', 'w', &white_queenTexture, window, sf::Vector2i(x, 0));
		}
		if (board[x][7] && board[x][7]->getId() == 'p') {
			delete board[x][7];
			board[x][7] = new Piece('q', 'b', &black_queenTexture, window, sf::Vector2i(x, 7));
		}
	}
}

// True if moving 'piece' to 'newSquare' is a legal move
bool Board::validMove(Piece* piece, sf::Vector2i newSquare) {
	if (piece == nullptr) { return false; }
	if (whiteTurn && piece->getColor() == 'b') { return false; }
	if (!whiteTurn && piece->getColor() == 'w') { return false; }

	return pieceCanMoveTo(piece, newSquare) && !willMoveCauseCheckForColor(piece, newSquare);
}

// True if moving 'piece' to 'newSquare' is a legal move (does not check for check)
bool Board::pieceCanMoveTo(Piece* piece, sf::Vector2i newSquare) {

	if (board[newSquare.x][newSquare.y] && piece->getColor() == board[newSquare.x][newSquare.y]->getColor())
		return false;

	switch (piece->getId()) {
	case 'p':
		return validMovePawn(piece, newSquare);
		break;
	case 'r':
		return validMoveRook(piece, newSquare);
		break;
	case 'n':
		return validMoveKnight(piece, newSquare);
		break;
	case 'b':
		return validMoveBishop(piece, newSquare);
		break;
	case 'q':
		return validMoveQueen(piece, newSquare);
		break;
	case 'k':
		return validMoveKing(piece, newSquare);
		break;
	default:
		std::cout << "Error! Invalid piece id!";
		break;
	}
	return false;
}

bool Board::validMovePawn(Piece* piece, sf::Vector2i newSquare) {
	sf::Vector2i oldSquare = piece->getSquare();
	char color = piece->getColor();

	if (color == 'w' && oldSquare.y <= newSquare.y) { return false; }
	if (color == 'b' && oldSquare.y >= newSquare.y) { return false; }

	// If pawn is trying to move to square in front of it
	if (newSquare.x == oldSquare.x && abs(newSquare.y - oldSquare.y) == 1) {
		// If there is a piece in front of it return false
		if (board[newSquare.x][newSquare.y]) { return false; }
		return true;
	}

	// If pawn is trying to move to square two in front of it
	if (newSquare.x == oldSquare.x && abs(newSquare.y - oldSquare.y) == 2) {
		// If pawn is not in starting position return false
		if (color == 'w' && oldSquare.y != 6) { return false; }
		if (color == 'b' && oldSquare.y != 1) { return false; }
		// If there is a piece in front of it return false
		if (board[int(newSquare.x)][int(newSquare.y)]) { return false; }
		return true;
	}

	// If pawn is trying to move diagonally
	if (abs(newSquare.x - oldSquare.x) == 1 && abs(oldSquare.y - newSquare.y) == 1) {
		// If there is a pawn that just moved up two squares behind newSquare (eb passant)
		if (board[newSquare.x][oldSquare.y]) {
			if (board[newSquare.x][oldSquare.y]->getId() == 'p' && board[newSquare.x][oldSquare.y]->justMovedUpTwo) {
				takePiece(sf::Vector2i(newSquare.x, oldSquare.y));
				return true;
			}
		}
		// If there is no piece in front of it return false
		if (!board[newSquare.x][newSquare.y]) { return false; }
		return true;
	}

	return false;
}

bool Board::validMoveRook(Piece* piece, sf::Vector2i newSquare) {
	sf::Vector2i oldSquare = piece->getSquare();

	if (newSquare.x != oldSquare.x && newSquare.y != oldSquare.y) { return false; }
	// Check if there are any pieces in the way
	if (newSquare.x == oldSquare.x) {
		// Moving up
		if (newSquare.y < oldSquare.y) {
			for (int i = int(oldSquare.y) - 1; i > int(newSquare.y); i--) {
				if (board[int(newSquare.x)][i]) { return false; }
			}
		}
		// Moving down
		else {
			for (int i = int(oldSquare.y) + 1; i < int(newSquare.y); i++) {
				if (board[int(newSquare.x)][i]) { return false; }
			}
		}
	}
	else {
		// Moving left
		if (newSquare.x < oldSquare.x) {
			for (int i = int(oldSquare.x) - 1; i > int(newSquare.x); i--) {
				if (board[i][int(newSquare.y)]) { return false; }
			}
		}
		// Moving right
		else {
			for (int i = int(oldSquare.x) + 1; i < int(newSquare.x); i++) {
				if (board[i][int(newSquare.y)]) { return false; }
			}
		}
	}

	return true;
}

bool Board::validMoveKnight(Piece* piece, sf::Vector2i newSquare) {
	sf::Vector2i oldSquare = piece->getSquare();

	if (abs(newSquare.x - oldSquare.x) == 1 && abs(newSquare.y - oldSquare.y) == 2) { return true; }
	if (abs(newSquare.x - oldSquare.x) == 2 && abs(newSquare.y - oldSquare.y) == 1) { return true; }
	return false;
}

bool Board::validMoveBishop(Piece* piece, sf::Vector2i newSquare) {
	sf::Vector2i oldSquare = piece->getSquare();

	if (abs(newSquare.x - oldSquare.x) != abs(newSquare.y - oldSquare.y)) { return false; }
	// Check if there are any pieces in the way
	if (newSquare.x < oldSquare.x) {
		// Moving up left
		if (newSquare.y < oldSquare.y) {
			for (int i = 1; i < int(oldSquare.x) - int(newSquare.x); i++) {
				if (board[int(oldSquare.x) - i][int(oldSquare.y) - i]) { return false; }
			}
		}
		// Moving down left
		else {
			for (int i = 1; i < int(oldSquare.x) - int(newSquare.x); i++) {
				if (board[int(oldSquare.x) - i][int(oldSquare.y) + i]) { return false; }
			}
		}
	}
	else {
		// Moving up right
		if (newSquare.y < oldSquare.y) {
			for (int i = 1; i < int(newSquare.x) - int(oldSquare.x); i++) {
				if (board[int(oldSquare.x) + i][int(oldSquare.y) - i]) { return false; }
			}
		}
		// Moving down right
		else {
			for (int i = 1; i < int(newSquare.x) - int(oldSquare.x); i++) {
				if (board[int(oldSquare.x) + i][int(oldSquare.y) + i]) { return false; }
			}
		}
	}
	return true;
}

bool Board::validMoveQueen(Piece* piece, sf::Vector2i newSquare) {
	sf::Vector2i oldSquare = piece->getSquare();

	if (validMoveRook(piece, newSquare) || validMoveBishop(piece, newSquare)) { return true; }
	return false;
}

bool Board::validMoveKing(Piece* piece, sf::Vector2i newSquare) {
	sf::Vector2i oldSquare = piece->getSquare();

	// Check special case where king is trying to castle
	if (abs(newSquare.x - oldSquare.x) > 1 && newSquare.y == oldSquare.y) {
		// If the king is trying to castle
		if (piece->moved) { return false; }
		// If king is in check or is trying to move into check
		if (isSquareInCheck(sf::Vector2i(oldSquare.x, oldSquare.y), piece->getColor())) { return false; }
		if (isSquareInCheck(sf::Vector2i(newSquare.x, newSquare.y), piece->getColor())) { return false; }

		// If the king is trying to castle left
		if (newSquare.x < oldSquare.x) {
			// If the rook has moved
			if (hasRookMoved(sf::Vector2i(0, 0)) || hasRookMoved(sf::Vector2i(0, 7))) { return false; }
			// If there are pieces in the way
			for (int i = 1; i < int(oldSquare.x); i++) {
				if (board[i][int(oldSquare.y)]) { return false; }
			}
			return true;
		}

		// If the king is trying to castle right
		if (newSquare.x > oldSquare.x) {
			// If the rook has moved
			if (hasRookMoved(sf::Vector2i(7, 7)) || hasRookMoved(sf::Vector2i(7, 0))) { return false; }
			// If there are pieces in the way
			for (int i = oldSquare.x + 1; i < 6; i++) {
				if (board[i][int(oldSquare.y)]) { return false; }
			}
			return true;
		}

		return false;
	}

	// Prevents the king from moving more than one square
	if (abs(newSquare.x - oldSquare.x) > 1 || abs(newSquare.y - oldSquare.y) > 1) { return false; }

	return true;
}

// If square is being attacked by a piece of the opposite color
bool Board::isSquareInCheck(sf::Vector2i sq, char color) {
	// If any of the enemy pieces on the board can make a valid move into p then p is in check
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; ++j) {
			if (board[i][j] && board[i][j]->getColor() != color) {
				if (pieceCanMoveTo(board[i][j], sq)) { return true; }
			}
		}
	}
	return false;
}

bool Board::isKingInCheck(char color) {
	Piece* king = nullptr;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j] && board[i][j]->getId() == 'k' && board[i][j]->getColor() == color)
				king = board[i][j];
		}
	}

	if (!king) { return false; }

	if (color == 'w') return isSquareInCheck(king->getSquare(), 'w');
	else			  return isSquareInCheck(king->getSquare(), 'b');
}

// True if after moving p to newSquare, the king of the same color as p will be in check
bool Board::willMoveCauseCheckForColor(Piece* p, sf::Vector2i newSquare) {
	Piece* boardBackup[8][8];
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			boardBackup[i][j] = board[i][j];
		}
	}

	// Create a copy of the board
	Piece* boardCopy[8][8];
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j]) { boardCopy[i][j] = new Piece(*board[i][j]); }
			else { boardCopy[i][j] = nullptr; }
		}
	}

	// Swap the board for its copy
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			board[i][j] = boardCopy[i][j];
		}
	}

	// Move the piece on the copied board
	sf::Vector2i sq = p->getSquare();
	movePiece(board[sq.x][sq.y], newSquare);

	// Check if that move will cause check
	bool ret = isKingInCheck(p->getColor());

	// Delete all the memory from the copied board
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; ++j) {
			delete board[i][j];
		}
	}

	// Swap the board back to the original
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			board[i][j] = boardBackup[i][j];
		}
	}

	// Return the result
	return ret;
}

bool Board::hasRookMoved(sf::Vector2i startingSquare) {
	// loop through log
	for (int i = 0; i < log.size(); i++) {
		// If the log contains starting square then return true
		if (log[i].first == startingSquare || log[i].second == startingSquare) { return true; }
	}

	return false;
}

Piece* Board::getPieceAt(sf::Vector2f p) {
	// Cycle through board
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (!board[i][j]) { continue; }
			if (board[i][j]->getBoundingBox().contains(p)) {
				return board[i][j];
			}
		}
	}

	return nullptr;
}

sf::Vector2i Board::getSquareAt(sf::Vector2f p) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			sf::FloatRect square(Ui::getTopLeftCorner(sf::Vector2i(i, j)), sf::Vector2f(Ui::squareLength, Ui::squareLength));
			if (square.contains(p)) {
				return sf::Vector2i(i, j);
			}
		}
	}

	return sf::Vector2i(-1, -1);
}

void Board::changeTurn() {
	if (whiteTurn) { whiteTurn = false; }
	else { whiteTurn = true; }
}

void Board::resetBoard() {
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

std::vector<std::pair<sf::Vector2i, sf::Vector2i>> Board::getPossibleMoves() {
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> moves;

	for (int l = 0; l < 8; l++) {
		for (int m = 0; m < 8; m++) {
			Piece* currentPiece = board[l][m];
			if (!currentPiece) continue;
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					sf::Vector2i newSquare(i, j);
					if (validMove(currentPiece, newSquare)) {
						moves.push_back({ currentPiece->getSquare(), newSquare });
					}
				}
			}
		}
	}

	return moves;
}

void Board::logMove(sf::Vector2i oldSquare, sf::Vector2i newSquare) {
	log.push_back({ oldSquare, newSquare });
}

void Board::saveLog(std::string saveLog) {
	std::ofstream file(saveLog);
	for (auto move : log) {
		file << move.first.x << " " << move.first.y << " " << move.second.x << " " << move.second.y << std::endl;
	}
	file.close();
}

bool Board::makeRandomBlackMove() {
	if (whiteTurn) return false;
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> moves = getPossibleMoves();
	if (moves.size() == 0) return false;
	// Random number generation setup
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<std::size_t> dis(0, moves.size() - 1);
	std::pair<sf::Vector2i, sf::Vector2i> move = moves[dis(gen)];
	makeMove(move.first, move.second);
	return true;
}

bool Board::makeRandomWhiteMove() {
	if (!whiteTurn) return false;
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> moves = getPossibleMoves();
	if (moves.size() == 0) return false;

	// Random number generation setup
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<std::size_t> dis(0, moves.size() - 1);

	std::pair<sf::Vector2i, sf::Vector2i> move = moves[dis(gen)];
	makeMove(move.first, move.second);
	return true;
}
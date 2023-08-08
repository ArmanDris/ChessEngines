#include "Board.h"

Board::Board() {
	placeStartingPieces();
}

bool Board::makeMove(sf::Vector2i oldSquare, sf::Vector2i newSquare) {
	return makeMove(&board[oldSquare.x][oldSquare.y], newSquare);
}

Board::~Board() {
	saveLog();
}

void Board::placeStartingPieces() {
	placePiece(Type::Rook, Color::Black, sf::Vector2i(0, 0));
	placePiece(Type::Knight, Color::Black, sf::Vector2i(1, 0));
	placePiece(Type::Bishop, Color::Black, sf::Vector2i(2, 0));
	placePiece(Type::Queen, Color::Black, sf::Vector2i(3, 0));
	placePiece(Type::King, Color::Black, sf::Vector2i(4, 0));
	placePiece(Type::Bishop, Color::Black, sf::Vector2i(5, 0));
	placePiece(Type::Knight, Color::Black, sf::Vector2i(6, 0));
	placePiece(Type::Rook, Color::Black, sf::Vector2i(7, 0));
	placePiece(Type::Pawn, Color::Black, sf::Vector2i(0, 1));
	placePiece(Type::Pawn, Color::Black, sf::Vector2i(1, 1));
	placePiece(Type::Pawn, Color::Black, sf::Vector2i(2, 1));
	placePiece(Type::Pawn, Color::Black, sf::Vector2i(3, 1));
	placePiece(Type::Pawn, Color::Black, sf::Vector2i(4, 1));
	placePiece(Type::Pawn, Color::Black, sf::Vector2i(5, 1));
	placePiece(Type::Pawn, Color::Black, sf::Vector2i(6, 1));
	placePiece(Type::Pawn, Color::Black, sf::Vector2i(7, 1));

	placePiece(Type::Rook, Color::White, sf::Vector2i(0, 7));
	placePiece(Type::Knight, Color::White, sf::Vector2i(1, 7));
	placePiece(Type::Bishop, Color::White, sf::Vector2i(2, 7));
	placePiece(Type::Queen, Color::White, sf::Vector2i(3, 7));
	placePiece(Type::King, Color::White, sf::Vector2i(4, 7));
	placePiece(Type::Bishop, Color::White, sf::Vector2i(5, 7));
	placePiece(Type::Knight, Color::White, sf::Vector2i(6, 7));
	placePiece(Type::Rook, Color::White, sf::Vector2i(7, 7));
	placePiece(Type::Pawn, Color::White, sf::Vector2i(0, 6));
	placePiece(Type::Pawn, Color::White, sf::Vector2i(1, 6));
	placePiece(Type::Pawn, Color::White, sf::Vector2i(2, 6));
	placePiece(Type::Pawn, Color::White, sf::Vector2i(3, 6));
	placePiece(Type::Pawn, Color::White, sf::Vector2i(4, 6));
	placePiece(Type::Pawn, Color::White, sf::Vector2i(5, 6));
	placePiece(Type::Pawn, Color::White, sf::Vector2i(6, 6));
	placePiece(Type::Pawn, Color::White, sf::Vector2i(7, 6));
}

void Board::placePiece(Type id, Color color, sf::Vector2i square) {
	board[square.x][square.y] = Piece(id, color, square);
}

// Returns true if a move is excecuted
bool Board::makeMove(Piece& piece, sf::Vector2i newSquare) {
	if ( checkGameOver())             return false;
	if (!validMove(piece, newSquare)) return false;

	sf::Vector2i oldSquare = piece->getSquare();

	piece->moved = true;

	// Special Pawn Logic (moving up two & en passant)
	removeAllMovedUpTwo();
	if (abs(oldSquare.y - newSquare.y) > 1) piece->justMovedUpTwo = true;

	// Preform move
	if (moveIsCastling(piece, newSquare)) castle(piece, newSquare);
	else                                  movePiece(piece, newSquare);

	promotePawns();
	logMove(oldSquare, newSquare);
	changeTurn();
	checkGameOver();
	return true;
}

// Castling invlolves moving multiple pieces soo all turn logic must be done in makeMove
void Board::movePiece(Piece* piece, sf::Vector2i newSquare) {
	sf::Vector2i oldSquare = piece->getSquare();

	// Move piece to new square
	board[newSquare.x][newSquare.y] = *piece;
	piece->setSquare(newSquare);

	// Remove piece from old square
	board[oldSquare.x][oldSquare.y] = Piece();
}

void Board::removeAllMovedUpTwo() {
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++)
			// if piece is a pawn and just moved up two
			if (board[x][y] && board[x][y].getId() == Type::Pawn && board[x][y].justMovedUpTwo)
				// set justMovedUpTwo to false
				board[x][y].justMovedUpTwo = false;
	}
}

void Board::castle(Piece* piece, sf::Vector2i newSquare) {
	sf::Vector2i oldSquare = piece->getSquare();
	// If the king is trying to castle left
	if (newSquare.x < piece->getSquare().x) {
		Piece* rook = &board[0][oldSquare.y];
		movePiece(rook, sf::Vector2i(3, oldSquare.y));
		movePiece(piece, sf::Vector2i(2, oldSquare.y));
	}

	// If the king is trying to castle right
	if (newSquare.x > piece->getSquare().x) {
		Piece* rook = &board[7][oldSquare.y];
		movePiece(rook, sf::Vector2i(5, oldSquare.y));
		movePiece(piece, sf::Vector2i(6, oldSquare.y));
	}
}

// Sets whiteVictory or blackVictory to true if the player has no valid moves
bool Board::checkGameOver() {
	int numMoves = getPossibleMoves().size();

	if (whiteTurn && numMoves == 0 && isKingInCheck(Color::White)) { blackVictory = true; return true; }
	if (!whiteTurn && numMoves == 0 && isKingInCheck(Color::Black)) { whiteVictory = true; return true; }

	if (whiteTurn && numMoves == 0 && !isKingInCheck(Color::White)) { draw = true; return true; }
	if (!whiteTurn && numMoves == 0 && !isKingInCheck(Color::Black)) { draw = true; return true; }

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
		if (board[x][0] && board[x][0].getId() == Type::Pawn) {
			placePiece(Type::Queen, Color::White, sf::Vector2i(x, 0));
		}
		if (board[x][7] && board[x][7].getId() == Type::Pawn) {
			placePiece(Type::Queen, Color::Black, sf::Vector2i(x, 7));
		}
	}
}

// True if moving 'piece' to 'newSquare' is a legal move
bool Board::validMove(Piece* piece, sf::Vector2i newSquare) {
	if (!piece)                                          return false;
	if ( whiteTurn && piece->getColor() == Color::Black) return false;
	if (!whiteTurn && piece->getColor() == Color::White) return false;

	return pieceCanMoveTo(piece, newSquare) && !willMoveCauseCheckForColor(piece, newSquare);
}

// True if moving 'piece' to 'newSquare' is a legal move (does not check for check)
bool Board::pieceCanMoveTo(Piece* piece, sf::Vector2i newSquare) {

	if (board[newSquare.x][newSquare.y] && piece->getColor() == board[newSquare.x][newSquare.y].getColor())
		return false;


	switch (piece->getId()) {
	case Type::Pawn:
		return validMovePawn(piece, newSquare);
		break;
	case Type::Rook:
		return validMoveRook(piece, newSquare);
		break;
	case Type::Knight:
		return validMoveKnight(piece, newSquare);
		break;
	case Type::Bishop:
		return validMoveBishop(piece, newSquare);
		break;
	case Type::Queen:
		return validMoveQueen(piece, newSquare);
		break;
	case Type::King:
		return validMoveKing(piece, newSquare);
		break;
	default:
		std::cout << "Error! Invalid piece id: " << typeToString(piece->getId()) << std::endl;
		break;
	}
	return false;
}

bool Board::validMovePawn(Piece* piece, sf::Vector2i newSquare) {
	sf::Vector2i oldSquare = piece->getSquare();
	Color color = piece->getColor();

	if (color == Color::White && oldSquare.y <= newSquare.y) { return false; }
	if (color == Color::Black && oldSquare.y >= newSquare.y) { return false; }

	// If pawn is trying to move to square in front of it
	if (newSquare.x == oldSquare.x && abs(newSquare.y - oldSquare.y) == 1) {
		// If there is a piece in front of it return false
		if (board[newSquare.x][newSquare.y]) { return false; }
		return true;
	}

	// If pawn is trying to move to square two in front of it
	if (newSquare.x == oldSquare.x && abs(newSquare.y - oldSquare.y) == 2) {
		// If pawn is not in starting position return false
		if (color == Color::White && oldSquare.y != 6) { return false; }
		if (color == Color::White && oldSquare.y != 1) { return false; }
		// If there is a piece in front of it return false
		if (board[int(newSquare.x)][int(newSquare.y)]) { return false; }
		return true;
	}

	// If pawn is trying to move diagonally
	if (abs(newSquare.x - oldSquare.x) == 1 && abs(oldSquare.y - newSquare.y) == 1) {
		// If there is a pawn that just moved up two squares behind newSquare (eb passant)
		if (board[newSquare.x][oldSquare.y]) {
			if (board[newSquare.x][oldSquare.y].getId() == Type::Pawn && board[newSquare.x][oldSquare.y].justMovedUpTwo) {
				board[newSquare.x][oldSquare.y] = Piece();
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
bool Board::isSquareInCheck(sf::Vector2i sq, Color color) {
	// If any of the enemy pieces on the board can make a valid move into p then p is in check
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; ++j) {
			if (board[i][j] && board[i][j].getColor() != color) {
				if (pieceCanMoveTo(&board[i][j], sq)) { return true; }
			}
		}
	}
	return false;
}

bool Board::isKingInCheck(Color color) {
	Piece* king = nullptr;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j] && board[i][j].getId() == Type::King && board[i][j].getColor() == color)
				king = &board[i][j];
		}
	}

	if (!king) { return false; }

	if (color == Color::White) return isSquareInCheck(king->getSquare(), Color::White);
	if (color == Color::Black) return isSquareInCheck(king->getSquare(), Color::Black);
}

// True if after moving p to newSquare, the king of the same color as p will be in check
bool Board::willMoveCauseCheckForColor(Piece* p, sf::Vector2i newSquare) {
	return false;

	//Piece* boardBackup[8][8];
	//for (int i = 0; i < 8; i++) {
	//	for (int j = 0; j < 8; j++) {
	//		boardBackup[i][j] = board[i][j];
	//	}
	//}

	//// Create a copy of the board
	//Piece* boardCopy[8][8];
	//for (int i = 0; i < 8; i++) {
	//	for (int j = 0; j < 8; j++) {
	//		if (board[i][j]) { boardCopy[i][j] = new Piece(*board[i][j]); }
	//		else { boardCopy[i][j] = nullptr; }
	//	}
	//}

	//// Swap the board for its copy
	//for (int i = 0; i < 8; i++) {
	//	for (int j = 0; j < 8; j++) {
	//		board[i][j] = boardCopy[i][j];
	//	}
	//}

	//// Move the piece on the copied board
	//sf::Vector2i sq = p->getSquare();
	//movePiece(board[sq.x][sq.y], newSquare);

	//// Check if that move will cause check
	//bool ret = isKingInCheck(p->getColor());

	//// Delete all the memory from the copied board
	//for (int i = 0; i < 8; i++) {
	//	for (int j = 0; j < 8; ++j) {
	//		delete board[i][j];
	//	}
	//}

	//// Swap the board back to the original
	//for (int i = 0; i < 8; i++) {
	//	for (int j = 0; j < 8; j++) {
	//		board[i][j] = boardBackup[i][j];
	//	}
	//}

	//// Return the result
	//return ret;
}

bool Board::hasRookMoved(sf::Vector2i startingSquare) {
	// loop through log
	for (int i = 0; i < log.size(); i++) {
		// If the log contains starting square then return true
		if (log[i].first == startingSquare || log[i].second == startingSquare) { return true; }
	}

	return false;
}

bool Board::moveIsCastling(Piece* piece, sf::Vector2i newSquare)
{
	sf::Vector2i oldSquare = piece->getSquare();
	return (piece->getId() == Type::King) && (abs(oldSquare.x - newSquare.x) > 1);
}

void Board::changeTurn() {
	if (whiteTurn) { whiteTurn = false; }
	else           { whiteTurn = true; }
}

void Board::setTurn(char turn)
{
	if (turn == 'w') { whiteTurn = true; }
	else			 { whiteTurn = false; }
}

std::vector<std::pair<sf::Vector2i, sf::Vector2i>> Board::getPossibleMoves() {
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> moves;

	for (int l = 0; l < 8; l++) {
		for (int m = 0; m < 8; m++) {
			Piece* currentPiece = &board[l][m];
			if (!*currentPiece) continue;
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
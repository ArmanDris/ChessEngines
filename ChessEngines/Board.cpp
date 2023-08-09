#include "Board.h"

Board::Board() {
	placeStartingPieces();
}

const Piece& Board::operator[](sf::Vector2i square) const
{
	return board[square.x][square.y];
}

Piece& Board::operator[](sf::Vector2i square)
{
	return board[square.x][square.y];
}

void Board::placeStartingPieces() {
	board[0][0] = Piece(Type::Rook, Color::Black);
	board[1][0] = Piece(Type::Knight, Color::Black);
	board[2][0] = Piece(Type::Bishop, Color::Black);
	board[3][0] = Piece(Type::Queen, Color::Black);
	board[4][0] = Piece(Type::King, Color::Black);
	board[5][0] = Piece(Type::Bishop, Color::Black);
	board[6][0] = Piece(Type::Knight, Color::Black);
	board[7][0] = Piece(Type::Rook, Color::Black);
	board[0][1] = Piece(Type::Pawn, Color::Black);
	board[1][1] = Piece(Type::Pawn, Color::Black);
	board[2][1] = Piece(Type::Pawn, Color::Black);
	board[3][1] = Piece(Type::Pawn, Color::Black);
	board[4][1] = Piece(Type::Pawn, Color::Black);
	board[5][1] = Piece(Type::Pawn, Color::Black);
	board[6][1] = Piece(Type::Pawn, Color::Black);
	board[7][1] = Piece(Type::Pawn, Color::Black);

	board[0][7] = Piece(Type::Rook, Color::White);
	board[1][7] = Piece(Type::Knight, Color::White);
	board[2][7] = Piece(Type::Bishop, Color::White);
	board[3][7] = Piece(Type::Queen, Color::White);
	board[4][7] = Piece(Type::King, Color::White);
	board[5][7] = Piece(Type::Bishop, Color::White);
	board[6][7] = Piece(Type::Knight, Color::White);
	board[7][7] = Piece(Type::Rook, Color::White);
	board[0][6] = Piece(Type::Pawn, Color::White);
	board[1][6] = Piece(Type::Pawn, Color::White);
	board[2][6] = Piece(Type::Pawn, Color::White);
	board[3][6] = Piece(Type::Pawn, Color::White);
	board[4][6] = Piece(Type::Pawn, Color::White);
	board[5][6] = Piece(Type::Pawn, Color::White);
	board[6][6] = Piece(Type::Pawn, Color::White);
	board[7][6] = Piece(Type::Pawn, Color::White);
}

// Returns true if a move is excecuted
bool Board::makeMove(sf::Vector2i oldSquare, sf::Vector2i newSquare) {
	if (whiteVictory || blackVictory || draw) return false;
	if (!legal_move(oldSquare, newSquare)) return false;

	logMove(oldSquare, newSquare);
	movePiece(oldSquare, newSquare);

	changeTurn();
	checkGameOver();
	return true;
}

// DO NOT CHANGE ORDER OF CONDITIONS, will break logic.
void Board::movePiece(sf::Vector2i oldSquare, sf::Vector2i newSquare)
{
	if (newSquare == oldSquare) return;

	// En passant
	if (moveIsEnPassent(oldSquare, newSquare))
		board[newSquare.x][oldSquare.y] = Piece();

	board[newSquare.x][newSquare.y] = board[oldSquare.x][oldSquare.y];
	board[oldSquare.x][oldSquare.y] = Piece();

	// Castling
	if (board[newSquare.x][newSquare.y].getType() == Type::King && abs(oldSquare.x - newSquare.x) > 1)
		castle(oldSquare, newSquare);

	// Pawn promotion
	if (board[newSquare.x][newSquare.y].getType() == Type::Pawn && (newSquare.y == 0 || newSquare.y == 7))
		board[newSquare.x][newSquare.y] = Piece(Type::Queen, board[newSquare.x][newSquare.y].getColor());
}

void Board::castle(sf::Vector2i oldSquare, sf::Vector2i newSquare) {
	// If the king is trying to castle left
	if (newSquare.x < oldSquare.x) {
		sf::Vector2i rook_square = sf::Vector2i(0, oldSquare.y);
		movePiece(rook_square, sf::Vector2i(3, oldSquare.y));
		movePiece(newSquare, sf::Vector2i(2, oldSquare.y));
	}

	// If the king is trying to castle right
	if (newSquare.x > oldSquare.x) {
		sf::Vector2i rook_square(7, oldSquare.y);
		movePiece(rook_square, sf::Vector2i(5, oldSquare.y));
		movePiece(newSquare, sf::Vector2i(6, oldSquare.y));
	}
}

// Sets whiteVictory or blackVictory to true if the player has no valid moves
bool Board::checkGameOver() {
	int numMoves = get_squares_pices_are_attacking().size();

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

bool Board::legal_move(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	if ( whiteTurn && board[oldSquare.x][oldSquare.y].getColor() == Color::Black) return false;
	if (!whiteTurn && board[oldSquare.x][oldSquare.y].getColor() == Color::White) return false;
	if (willMoveCauseCheckForColor(oldSquare, newSquare)) return false;

	return piece_is_attacking_square(oldSquare, newSquare);
}

// True if a piece is attacking newSquare
bool Board::piece_is_attacking_square(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	if (!board[oldSquare.x][oldSquare.y]) return false;
	if (board[oldSquare.x][oldSquare.y].getColor() == board[newSquare.x][newSquare.y].getColor()) return false;

	switch (board[oldSquare.x][oldSquare.y].getType()) {
	case Type::Pawn:   return pawn_is_attacking_square(oldSquare, newSquare);
	case Type::Rook:   return rook_is_attacking_square(oldSquare, newSquare);
	case Type::Knight: return knight_is_attacking_square(oldSquare, newSquare);
	case Type::Bishop: return bishop_is_attacking_square(oldSquare, newSquare);
	case Type::Queen:  return queen_is_attacking_square(oldSquare, newSquare);
	case Type::King:   return king_is_attacking_square(oldSquare, newSquare);
	default: std::cout << "Error! Invalid piece id: " << typeToString(board[oldSquare.x][oldSquare.y].getType()) << std::endl;
	}

	return false;
}

bool Board::pawn_is_attacking_square(sf::Vector2i oldSquare, sf::Vector2i newSquare) const {
	Color color = board[oldSquare.x][oldSquare.y].getColor();

	if (color == Color::White && oldSquare.y <= newSquare.y) return false;
	if (color == Color::Black && oldSquare.y >= newSquare.y) return false;

	// If pawn is trying to move to square in front of it
	if (newSquare.x == oldSquare.x && abs(newSquare.y - oldSquare.y) == 1) {
		// If there is a piece in front of it return false
		if (board[newSquare.x][newSquare.y]) return false;
		return true;
	}

	// If pawn is trying to move to square two in front of it
	if (newSquare.x == oldSquare.x && abs(newSquare.y - oldSquare.y) == 2) {
		// If pawn is not in starting position return false
		if (color == Color::White && oldSquare.y != 6) { return false; }
		if (color == Color::Black && oldSquare.y != 1) { return false; }
		// If there is a piece in front of it return false
		if (board[newSquare.x][newSquare.y]) { return false; }
		return true;
	}

	// If pawn is trying to move diagonally
	if (abs(newSquare.x - oldSquare.x) == 1 && abs(oldSquare.y - newSquare.y) == 1) {
		// If there is a pawn that just moved up two squares behind newSquare (en passant)
		if (hasPieceJustMovedUpTwo(sf::Vector2i(newSquare.x, oldSquare.y))) return true;
		// If there is no piece in front of it return false
		if (!board[newSquare.x][newSquare.y]) { return false; }
		return true;
	}

	return false;
}

bool Board::rook_is_attacking_square(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
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

bool Board::knight_is_attacking_square(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	if (abs(newSquare.x - oldSquare.x) == 1 && abs(newSquare.y - oldSquare.y) == 2) { return true; }
	if (abs(newSquare.x - oldSquare.x) == 2 && abs(newSquare.y - oldSquare.y) == 1) { return true; }
	return false;
}

bool Board::bishop_is_attacking_square(sf::Vector2i oldSquare, sf::Vector2i newSquare) const {

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

bool Board::queen_is_attacking_square(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	if (rook_is_attacking_square(oldSquare, newSquare) || bishop_is_attacking_square(oldSquare, newSquare)) { return true; }
	return false;
}

bool Board::king_is_attacking_square(sf::Vector2i oldSquare, sf::Vector2i newSquare) const {

	//// Check special case where king is trying to castle
	//if (abs(newSquare.x - oldSquare.x) > 1 && newSquare.y == oldSquare.y) {
	//	// If the king is trying to castle
	//	sf::Vector2i startingSquare;
	//	if (board[oldSquare.x][oldSquare.y].getColor() == Color::White) startingSquare = sf::Vector2i(4, 7);
	//	else                                                            startingSquare = sf::Vector2i(4, 0);
	//	if (hasPieceMoved(startingSquare)) return false;
	//	// If king is in check or is trying to move into check
	//	if (isSquareInCheck(sf::Vector2i(oldSquare.x, oldSquare.y), board[oldSquare.x][oldSquare.y].getColor())) { return false; }
	//	if (isSquareInCheck(sf::Vector2i(newSquare.x, newSquare.y), board[oldSquare.x][oldSquare.y].getColor())) { return false; }

	//	// If the king is trying to castle left
	//	if (newSquare.x < oldSquare.x) {
	//		// If the rook has moved
	//		if (hasPieceMoved(sf::Vector2i(0, 0)) || hasPieceMoved(sf::Vector2i(0, 7))) { return false; }
	//		// If there are pieces in the way
	//		for (int i = 1; i < int(oldSquare.x); i++) {
	//			if (board[i][int(oldSquare.y)]) { return false; }
	//		}
	//		return true;
	//	}

	//	// If the king is trying to castle right
	//	if (newSquare.x > oldSquare.x) {
	//		// If the rook has moved
	//		if (hasPieceMoved(sf::Vector2i(7, 7)) || hasPieceMoved(sf::Vector2i(7, 0))) { return false; }
	//		// If there are pieces in the way
	//		for (int i = oldSquare.x + 1; i < 6; i++) {
	//			if (board[i][int(oldSquare.y)]) { return false; }
	//		}
	//		return true;
	//	}

	//	return false;
	//}
	throw std::logic_error("I should move the castle logic somewhere else and keep this just for attacking squares.");

	// Prevents the king from moving more than one square
	if (abs(newSquare.x - oldSquare.x) > 1 || abs(newSquare.y - oldSquare.y) > 1) { return false; }

	return true;
}

// If square is being attacked by a piece of the opposite color
bool Board::isSquareInCheck(sf::Vector2i sq, Color color) const {
	// If any of the enemy pieces on the board can make a valid move into p then p is in check
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; ++j) {
			if (board[i][j] && board[i][j].getColor() != color) {
				if (piece_is_attacking_square(sf::Vector2i(i, j), sq)) { return true; }
			}
		}
	}
	return false;
}

bool Board::isKingInCheck(Color color) const {

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j].getType() == Type::King && board[i][j].getColor() == color)
				return isSquareInCheck(sf::Vector2i(i, j), color);
		}
	}

	return false;
}

// If a pawn just moved up two AND into sq
bool Board::hasPieceJustMovedUpTwo(sf::Vector2i sq) const
{
	if (log.size() == 0) return false;

	std::tuple<Color, Type, sf::Vector2i, Color, Type, sf::Vector2i> last_move = log.back();
	if (std::get<1>(last_move) == Type::Pawn &&  // If last entry is a pawn
		abs(std::get<2>(last_move).y - std::get<5>(last_move).y) == 2 && // If it moved up two
		std::get<5>(last_move) == sq) { // If it moved into sq
		return true; // Then return true
	}

	return false;
}

bool Board::moveIsEnPassent(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	// Piece must be a pawn
	if (board[oldSquare.x][oldSquare.y].getType() != Type::Pawn) return false;

	// Piece must not move onto a piece AND be moving diagonally
	if (!board[newSquare.x][newSquare.y] && newSquare.x != oldSquare.x) return true;

	return false;
}

// True if after moving p to newSquare, the king of the same color as p will be in check
bool Board::willMoveCauseCheckForColor(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	Board clone = *this;
	clone.movePiece(oldSquare, newSquare);
	return clone.isKingInCheck(board[oldSquare.x][oldSquare.y].getColor());
}

bool Board::hasPieceMoved(sf::Vector2i startingSquare) const {
	// loop through log
	for (int i = 0; i < log.size(); i++) {
		// If the log contains starting square then return true
		if (std::get<2>(log[i]) == startingSquare || std::get<5>(log[i]) == startingSquare) { return true; }
	}

	return false;
}

void Board::changeTurn() {
	if (whiteTurn) { whiteTurn = false; }
	else { whiteTurn = true; }
}

void Board::setTurn(char turn)
{
	if (turn == 'w') { whiteTurn = true; }
	else { whiteTurn = false; }
}

// This is not called getMoves because of two reasons:
// 1. It returns a list of moves for both black and white
// 2. Some moves may put the king in check, so they are not valid
std::vector<std::pair<sf::Vector2i, sf::Vector2i>> Board::get_squares_pices_are_attacking() {
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> moves;

	for (int l = 0; l < 8; l++) {
		for (int m = 0; m < 8; m++) {
			sf::Vector2i currentSquare(l, m);
			if (!board[currentSquare.x][currentSquare.y]) continue;
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					sf::Vector2i newSquare(i, j);
					if (piece_is_attacking_square(currentSquare, newSquare)) {
						moves.push_back({ currentSquare, newSquare });
					}
				}
			}
		}
	}

	return moves;
}

void Board::logMove(sf::Vector2i oldSquare, sf::Vector2i newSquare) {
	log.push_back({ board[oldSquare.x][oldSquare.y].getColor(), board[oldSquare.x][oldSquare.y].getType(), oldSquare,
					board[newSquare.x][newSquare.y].getColor(), board[newSquare.x][newSquare.y].getType(), newSquare });
}

void Board::saveLog(std::string saveLog) {
	std::ofstream file(saveLog);
	for (auto move : log) {
		file << colorToString(std::get<0>(move)) << " "
			<< typeToString(std::get<1>(move)) << " "
			<< std::get<2>(move).x << "," << std::get<2>(move).y << " "

			<< colorToString(std::get<3>(move)) << " "
			<< typeToString(std::get<4>(move)) << " "
			<< std::get<5>(move).x << "," << std::get<5>(move).y << std::endl;
	}
	file.close();
}
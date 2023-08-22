#include "Board.h"


Board::Board() {
	placeStartingPieces();
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

void Board::undoMove()
{
	// !!! NEED TO ADD SUPPORT FOR CASTLING AND EN PASSENT !!!
	if (log.size() == 0) return;
	auto last_log = log.back();

	Piece old_piece = std::get<0>(last_log);
	sf::Vector2i old_square = std::get<1>(last_log);

	Piece new_piece = std::get<2>(last_log);
	sf::Vector2i new_square = std::get<3>(last_log);

	if (old_piece.getType() == Type::King && abs(old_square.x - new_square.x) > 1)
		std::cout << "undo castle" << std::endl;

	if (old_piece.getType() == Type::Pawn && new_piece.getType() == Type::None && old_square.y != new_square.y) {
		// Should place a pawn behind the new square
		if (old_piece.getColor() == Color::White)
			board[new_square.x][new_square.y + 1] = Piece(Type::Pawn, Color::Black);
		else
			board[new_square.x][new_square.y - 1] = Piece(Type::Pawn, Color::White);
	}

	board[old_square.x][old_square.y] = old_piece;
	board[new_square.x][new_square.y] = new_piece;

	log.pop_back();
	changeTurn();
}

// Will not check if move is legal
void Board::movePiece(sf::Vector2i oldSquare, sf::Vector2i newSquare)
{
	if (newSquare == oldSquare) return;

	// En passant
	if (moveIsEnPassent(oldSquare, newSquare))
		board[newSquare.x][oldSquare.y] = Piece();
	
	// Castling
	if (moveIsCastle(oldSquare, newSquare))
		castle(oldSquare, newSquare);

	// Pawn promotion
	if (board[oldSquare.x][oldSquare.y].getType() == Type::Pawn && (newSquare.y == 0 || newSquare.y == 7))
		board[oldSquare.x][oldSquare.y] = Piece(Type::Queen, board[oldSquare.x][oldSquare.y].getColor());

	board[newSquare.x][newSquare.y] = board[oldSquare.x][oldSquare.y];
	board[oldSquare.x][oldSquare.y] = Piece();
}

void Board::castle(sf::Vector2i oldSquare, sf::Vector2i newSquare) 
{
	// The king move will have already been excecuted so just need to move rook
	
	// If the king is trying to castle left
	if (oldSquare.x > newSquare.x) {
		board[3][oldSquare.y] = board[0][oldSquare.y];
		board[0][oldSquare.y] = Piece();
	}

	// If the king is trying to castle right
	if (oldSquare.x < newSquare.x) {
		board[5][oldSquare.y] = board[7][oldSquare.y];
		board[7][oldSquare.y] = Piece();
	}
}

// Sets whiteVictory or blackVictory to true if the player has no valid moves
bool Board::checkGameOver() {
	int numMoves = get_moves().size();

	if (whiteTurn && numMoves == 0 && isKingInCheck(Color::White)) { blackVictory = true; return true; }
	if (!whiteTurn && numMoves == 0 && isKingInCheck(Color::Black)) { whiteVictory = true; return true; }

	if (whiteTurn && numMoves == 0 && !isKingInCheck(Color::White)) { draw = true; return true; }
	if (!whiteTurn && numMoves == 0 && !isKingInCheck(Color::Black)) { draw = true; return true; }

	if (fiftyMoveRule()) { draw = true; return true; }

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

// If no pawn has moved or no piece has been taken in the last 50 moves then return true
bool Board::fiftyMoveRule() const

{
	if (log.size() < 50) return false;

	// Loop through last 50 log entries
	for (int i = log.size() - 50; i < log.size(); i++) {
		// If a pawn has moved return false
		if (std::get<0>(log[i]).getType() == Type::Pawn) return false;
		// If a piece has been taken return false
		if (std::get<2>(log[i]).getType() != Type::None) return false;
	}
	return true;
}

bool Board::legal_move(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	if ( whiteTurn && board[oldSquare.x][oldSquare.y].getColor() == Color::Black) return false;
	if (!whiteTurn && board[oldSquare.x][oldSquare.y].getColor() == Color::White) return false;
	if (board[oldSquare.x][oldSquare.y].getColor() == board[newSquare.x][newSquare.y].getColor()) return false;
	if (willMoveCauseCheckForColor(oldSquare, newSquare)) return false;
	if (moveIsCastle(oldSquare, newSquare)) return true;

	return piece_is_attacking_square(oldSquare, newSquare);
}

// True if a piece is attacking newSquare
bool Board::piece_is_attacking_square(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	if (!board[oldSquare.x][oldSquare.y]) return false;

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
		if (color == Color::White && oldSquare.y != 6) return false;
		if (color == Color::Black && oldSquare.y != 1) return false;
		// If there is a piece in front of it return false
		if (color == Color::White && board[oldSquare.x][oldSquare.y - 1]) return false;
		if (color == Color::Black && board[oldSquare.x][oldSquare.y + 1]) return false;
		// If there is a piece two in front of it return false
		if (board[newSquare.x][newSquare.y]) return false;

		return true;
	}

	// If pawn is trying to move diagonally
	if (abs(newSquare.x - oldSquare.x) == 1 && abs(oldSquare.y - newSquare.y) == 1) {
		// If there is a pawn that just moved up two squares behind newSquare (en passant)
		if (hasPawnJustMovedUpTwo(sf::Vector2i(newSquare.x, oldSquare.y))) return true;
		// If there is no piece in front of it return false
		if (!board[newSquare.x][newSquare.y]) return false;
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

bool Board::king_is_attacking_square(sf::Vector2i oldSquare, sf::Vector2i newSquare) const 
{
	// Prevents the king from moving more than one square
	if (abs(newSquare.x - oldSquare.x) > 1 || abs(newSquare.y - oldSquare.y) > 1) { return false; }

	return true;
}

bool Board::square_is_attacked_by(sf::Vector2i sq, Color color) const {
	// If any of the enemy pieces on the board can make a valid move into p then p is in check
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; ++j) {
			if (board[i][j] && board[i][j].getColor() == color && sf::Vector2i(i, j) != sq) {
				if (piece_is_attacking_square(sf::Vector2i(i, j), sq)) { return true; }
			}
		}
	}
	return false;
}

bool Board::isKingInCheck(Color color) const {
	Color enemy_color = (color == Color::White) ? Color::Black : Color::White;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j].getType() == Type::King && board[i][j].getColor() == color)
				return square_is_attacked_by(sf::Vector2i(i, j), enemy_color);
		}
	}

	return false;
}

// If a pawn just moved up two AND into sq
bool Board::hasPawnJustMovedUpTwo(sf::Vector2i sq) const
{
	if (log.size() == 0) return false;

	auto last_move = log.back();
	if (std::get<0>(last_move).getType() == Type::Pawn &&  // If last entry is a pawn
		abs(std::get<1>(last_move).y - std::get<3>(last_move).y) == 2 && // If it moved up two
		std::get<3>(last_move) == sq) { // If it moved into sq
		return true; // Then return true
	}

	return false;
}

bool Board::moveIsCastle(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	Color king_color = board[oldSquare.x][oldSquare.y].getColor();
	Color enemy_color = (king_color == Color::White) ? Color::Black : Color::White;
	if (board[oldSquare.x][oldSquare.y].getType() != Type::King) return false; // Make sure piece is king
	if (board[newSquare.x][newSquare.y]) return false;                         // Make sure new square is empty
	if (oldSquare.y != newSquare.y) return false;                              // Make sure king is nor moving up/down
	if (king_is_attacking_square(oldSquare, newSquare)) return false;          // Make sure king is moving more than one square
	if (abs(oldSquare.x - newSquare.x) > 2) return false;                      // Make sure king is mot moving three squares left on queen side
	if (hasPieceMoved(oldSquare)) return false;  						       // Make sure king has not moved
	if (square_is_attacked_by(oldSquare, enemy_color)) return false;           // Make sure king is not in check

	// Castling right
	if (oldSquare.x < newSquare.x) {
		// If pieces between king and rook
		for (int i = oldSquare.x + 1; i < 7; i++)
			if (board[i][oldSquare.y]) return false;
		// If rook has moved return false
		if (king_color == Color::White && hasPieceMoved(sf::Vector2i(7, 7))) return false;
		if (king_color == Color::Black && hasPieceMoved(sf::Vector2i(7, 0))) return false;
		// If king will be in check return false
		if (square_is_attacked_by(sf::Vector2i(6, oldSquare.y), enemy_color)) return false;
		// If king will move over checked square return false
		if (square_is_attacked_by(sf::Vector2i(5, oldSquare.y), enemy_color)) return false;

		// Otherwise return true
		return true;
	}

	// Castling left
	if (oldSquare.x > newSquare.x) {
		// If pieces between king and rook
		for (int i = 1; i < oldSquare.x; i++)
			if (board[i][oldSquare.y]) return false;
		// If rook has moved return false
		if (king_color == Color::White && hasPieceMoved(sf::Vector2i(0, 7))) return false;
		if (king_color == Color::Black && hasPieceMoved(sf::Vector2i(0, 0))) return false;
		// If king will be in check return false
		if (square_is_attacked_by(sf::Vector2i(2, oldSquare.y), enemy_color)) return false;
		// If king will move over checked square return false
		if (square_is_attacked_by(sf::Vector2i(3, oldSquare.y), enemy_color)) return false;

		// Otherwise return true
		return true;
	}
	return false;
}

bool Board::moveIsEnPassent(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	// Make sure is legal move
	if (pawn_is_attacking_square(oldSquare, newSquare)) return false;
	// Piece must be a pawn
	if (board[oldSquare.x][oldSquare.y].getType() != Type::Pawn) return false;

	// Piece must not move onto a piece AND be moving diagonally
	if (!board[newSquare.x][newSquare.y] && newSquare.x != oldSquare.x) return true;

	return false;
}

// True if after moving oldSquare to newSquare, the king of the same color as oldSquare will be in check
bool Board::willMoveCauseCheckForColor(sf::Vector2i oldSquare, sf::Vector2i newSquare) const
{
	Color color = board[oldSquare.x][oldSquare.y].getColor();
	Board clone = *this;
	clone.movePiece(oldSquare, newSquare);
	return clone.isKingInCheck(color);
}

// If square has moved return true
bool Board::hasPieceMoved(sf::Vector2i startingSquare) const {
	// do not look at move that is being made (most recent log entry)
	for (int i = 0; i < log.size() - 1; i++) {
		// If the log contains starting square then return true
		if (std::get<1>(log[i]) == startingSquare || std::get<3>(log[i]) == startingSquare) return true;
	}

	return false;
}

void Board::changeTurn() {
	if (whiteTurn) whiteTurn = false;
	else           whiteTurn = true;
}

std::vector<std::pair<sf::Vector2i, sf::Vector2i>> Board::get_moves() {
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> moves;

	for (int l = 0; l < 8; l++) {
		for (int m = 0; m < 8; m++) {
			sf::Vector2i currentSquare(l, m);
			if (!board[currentSquare.x][currentSquare.y]) continue;
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					sf::Vector2i newSquare(i, j);
					if (legal_move(currentSquare, newSquare)) {
						moves.push_back({ currentSquare, newSquare });
					}
				}
			}
		}
	}

	return moves;
}

void Board::logMove(sf::Vector2i oldSquare, sf::Vector2i newSquare) {
	log.push_back({ board[oldSquare.x][oldSquare.y], oldSquare,
					board[newSquare.x][newSquare.y], newSquare });
}

void Board::saveLog(std::string saveLog) {
	std::ofstream file(saveLog);

	// Get the current system time
	auto now = std::chrono::system_clock::now();
	std::time_t time_now = std::chrono::system_clock::to_time_t(now);
	std::tm tm_now;

	// Use localtime_s() for safer handling
	if (localtime_s(&tm_now, &time_now) == 0) {
		char time_buffer[80];
		std::strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", &tm_now);

		// Print the formatted time at the top of the file
		file << "Log saved on: " << time_buffer << std::endl;
	}
	else {
		// Handle error
		std::cerr << "Error getting local time." << std::endl;
	}


	file << "Piece being moved : Location piece is moving to" << std::endl << std::endl;
	for (auto move : log) {
		file << colorToString(std::get<0>(move).getColor()) << " "
			<< typeToString(std::get<0>(move).getType()) << " "
			<< std::get<1>(move).x << "," << std::get<1>(move).y << " : "

			<< colorToString(std::get<2>(move).getColor()) << " "
			<< typeToString(std::get<2>(move).getType()) << " "
			<< std::get<3>(move).x << "," << std::get<3>(move).y << std::endl;
	}
	file.close();
}
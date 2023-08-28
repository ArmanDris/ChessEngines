#include "Board.h"


Board::Board() {
	placeStartingPieces();
}

void Board::placeStartingPieces() {
	board[0][0] = Piece(PieceType::Rook, PieceColor::Black);
	board[1][0] = Piece(PieceType::Knight, PieceColor::Black);
	board[2][0] = Piece(PieceType::Bishop, PieceColor::Black);
	board[3][0] = Piece(PieceType::Queen, PieceColor::Black);
	board[4][0] = Piece(PieceType::King, PieceColor::Black);
	board[5][0] = Piece(PieceType::Bishop, PieceColor::Black);
	board[6][0] = Piece(PieceType::Knight, PieceColor::Black);
	board[7][0] = Piece(PieceType::Rook, PieceColor::Black);
	board[0][1] = Piece(PieceType::Pawn, PieceColor::Black);
	board[1][1] = Piece(PieceType::Pawn, PieceColor::Black);
	board[2][1] = Piece(PieceType::Pawn, PieceColor::Black);
	board[3][1] = Piece(PieceType::Pawn, PieceColor::Black);
	board[4][1] = Piece(PieceType::Pawn, PieceColor::Black);
	board[5][1] = Piece(PieceType::Pawn, PieceColor::Black);
	board[6][1] = Piece(PieceType::Pawn, PieceColor::Black);
	board[7][1] = Piece(PieceType::Pawn, PieceColor::Black);

	board[0][7] = Piece(PieceType::Rook, PieceColor::White);
	board[1][7] = Piece(PieceType::Knight, PieceColor::White);
	board[2][7] = Piece(PieceType::Bishop, PieceColor::White);
	board[3][7] = Piece(PieceType::Queen, PieceColor::White);
	board[4][7] = Piece(PieceType::King, PieceColor::White);
	board[5][7] = Piece(PieceType::Bishop, PieceColor::White);
	board[6][7] = Piece(PieceType::Knight, PieceColor::White);
	board[7][7] = Piece(PieceType::Rook, PieceColor::White);
	board[0][6] = Piece(PieceType::Pawn, PieceColor::White);
	board[1][6] = Piece(PieceType::Pawn, PieceColor::White);
	board[2][6] = Piece(PieceType::Pawn, PieceColor::White);
	board[3][6] = Piece(PieceType::Pawn, PieceColor::White);
	board[4][6] = Piece(PieceType::Pawn, PieceColor::White);
	board[5][6] = Piece(PieceType::Pawn, PieceColor::White);
	board[6][6] = Piece(PieceType::Pawn, PieceColor::White);
	board[7][6] = Piece(PieceType::Pawn, PieceColor::White);
}

// Returns true if a move is excecuted
bool Board::makeMove(const sf::Vector2i old_square, const sf::Vector2i new_square
) {
	movePiece(old_square, new_square);
	changeTurn();
	return true;
}

void Board::undoMove()
{
	if (log.size() == 0) return;
	auto last_log = log.back();

	Piece old_piece = std::get<0>(last_log);
	sf::Vector2i old_square = std::get<1>(last_log);

	Piece new_piece = std::get<2>(last_log);
	sf::Vector2i new_square = std::get<3>(last_log);

	// Undo castle
	if (old_piece.getType() == PieceType::King && abs(old_square.x - new_square.x) > 1) {
		// If undoing left castle
		if (old_square.x > new_square.x) {
			board[0][old_square.y] = Piece(PieceType::Rook, old_piece.getColor());
			board[3][old_square.y] = Piece();
		}
		// If undoing right castle
		else {
			board[7][old_square.y] = Piece(PieceType::Rook, old_piece.getColor());
			board[5][old_square.y] = Piece();
		}
	}

	// Undo en passent
	if (old_piece.getType() == PieceType::Pawn && new_piece.getType() == PieceType::None && old_square.x != new_square.x) {
		// Should place a pawn behind the new square
		if (old_piece.getColor() == PieceColor::White)
			board[new_square.x][new_square.y + 1] = Piece(PieceType::Pawn, PieceColor::Black);
		else
			board[new_square.x][new_square.y - 1] = Piece(PieceType::Pawn, PieceColor::White);
	}

	board[old_square.x][old_square.y] = old_piece;
	board[new_square.x][new_square.y] = new_piece;

	log.pop_back();
	// Reset game over in case move caused draw or such
	whiteVictory = false; blackVictory = false; draw = false;
	changeTurn();
}

void Board::importFEN(std::string FEN)
{
	size_t pos = FEN.find(' ');
	if (pos == std::string::npos) { std::cout << "Invalid FEN \n"; return; }
	std::string piece_placement = FEN.substr(0, pos);
	FEN = FEN.substr(pos + 1);

	pos = FEN.find(' ');
	if (pos == std::string::npos) { std::cout << "Invalid FEN \n"; return; }
	std::string active_color = FEN.substr(0, pos);
	FEN = FEN.substr(pos + 1);

	pos = FEN.find(' ');
	if (pos == std::string::npos) { std::cout << "Invalid FEN \n"; return; }
	std::string castling_rights = FEN.substr(0, pos);
	FEN = FEN.substr(pos + 1);

	pos = FEN.find(' ');
	if (pos == std::string::npos) { std::cout << "Invalid FEN \n"; return; }
	std::string en_passent_target_square = FEN.substr(0, pos);
	FEN = FEN.substr(pos + 1);

	pos = FEN.find(' ');
	if (pos == std::string::npos) { std::cout << "Invalid FEN \n"; return; }
	std::string half_move_clock = FEN.substr(0, pos);
	FEN = FEN.substr(pos + 1);

	std::string full_move_number = FEN;
}

// Will not check if move is legal
void Board::movePiece(const sf::Vector2i& old_square, const sf::Vector2i& new_square)
{
	logMove(old_square, new_square);
	
	if (new_square == old_square) return;

	// En passant
	if (moveIsEnPassent(old_square, new_square))
		board[new_square.x][old_square.y] = Piece();

	// Castling
	if (moveIsCastle(old_square, new_square))
		castle(old_square, new_square);

	// Pawn promotion
	if (board[old_square.x][old_square.y].getType() == PieceType::Pawn && (new_square.y == 0 || new_square.y == 7))
		board[old_square.x][old_square.y] = Piece(PieceType::Queen, board[old_square.x][old_square.y].getColor());

	board[new_square.x][new_square.y] = board[old_square.x][old_square.y];
	board[old_square.x][old_square.y] = Piece();
}

bool Board::moveIsCastle(const sf::Vector2i& old_square, const sf::Vector2i& new_square)
{
	return false;
}

bool Board::moveIsEnPassent(const sf::Vector2i& old_square, const sf::Vector2i& new_square)
{
	return false;
}

void Board::castle(const sf::Vector2i& old_square, const sf::Vector2i& new_square)
{
	// The king move will have already been excecuted so just need to move rook

	// If the king is trying to castle left
	if (old_square.x > new_square.x) {
		board[3][old_square.y] = board[0][old_square.y];
		board[0][old_square.y] = Piece();
	}

	// If the king is trying to castle right
	if (old_square.x < new_square.x) {
		board[5][old_square.y] = board[7][old_square.y];
		board[7][old_square.y] = Piece();
	}
}


// If no pawn has moved or no piece has been taken in the last 50 moves then return true
bool Board::fiftyMoveRule() const
{
	if (log.size() < 100) return false;

	// Loop through last 100 log entries
	for (int i = log.size() - 100; i < log.size(); i++) {
		// If a pawn has moved return false
		if (std::get<0>(log[i]).getType() == PieceType::Pawn) return false;
		// If a piece has been taken return false
		if (std::get<2>(log[i]).getType() != PieceType::None) return false;
	}
	return true;
}

void Board::changeTurn() {
	if (whiteTurn) whiteTurn = false;
	else           whiteTurn = true;
}

void Board::logMove(const sf::Vector2i old_square, const sf::Vector2i new_square) {
	log.push_back({ board[old_square.x][old_square.y], old_square,
					board[new_square.x][new_square.y], new_square });
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
		file << std::get<0>(move).colorToString() << " "
			<< std::get<0>(move).typeToString() << " "
			<< std::get<1>(move).x << "," << std::get<1>(move).y << " : "

			<< std::get<2>(move).colorToString() << " "
			<< std::get<2>(move).typeToString() << " "
			<< std::get<3>(move).x << "," << std::get<3>(move).y << std::endl;
	}
	file.close();
}
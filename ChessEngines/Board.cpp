#include "Board.h"


Board::Board() {
	importFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

// Returns true if a move is excecuted
void Board::makeMove(const sf::Vector2i old_square, const sf::Vector2i new_square) {
	move m = { old_square, new_square };
	auto it = std::find(legal_moves.begin(), legal_moves.end(), m);
	if (it == legal_moves.end()) return;

	movePiece(old_square, new_square);
	changeTurn();
}

void Board::undoMove()
{
	if (log.size() == 0) return;
	softUndoMove();
	changeTurn();
}

void Board::importFEN(std::string FEN)
{
	log.clear();
	whiteVictory = false; blackVictory = false; draw = false;
	legal_moves.clear();

	for (int x = 0; x < 8; x++) for (int y = 0; y < 8; y++) board[x][y] = Piece();

	if (FEN.back() != ' ') FEN += ' ';

	std::string FEN_parts[6];
	size_t pos = FEN.find(' ');
	int i = 0;
	while (pos != std::string::npos) {
		FEN_parts[i] = FEN.substr(0, pos);

		FEN = FEN.substr(pos + 1);
		i++;
		pos = FEN.find(' ');
	}

	// Place Pieces
	FEN_parts[0] += '/';
	std::string pieces[8];
	pos = FEN_parts[0].find('/');
	i = 0;
	while (pos != std::string::npos) {
		pieces[i] = FEN_parts[0].substr(0, pos);

		FEN_parts[0] = FEN_parts[0].substr(pos + 1);
		i++;
		pos = FEN_parts[0].find('/');
	}
	
	int x = 0, y = 0;
	for (std::string s : pieces) {
		for (char c : s) {
			if (c >= '0' && c <= '9')
        		x += c - '0';
			else {
				board[x][y] = charToPiece(c);
				x++;
			}
		}
		x = 0;
		y++;
	}

	// Set turn
	if (FEN_parts[1] == "w" && !whiteTurn) changeTurn();
	if (FEN_parts[1] == "b" &&  whiteTurn) changeTurn();

	// Castling Rights
	// FEN_parts[2]; Not implemented

	// En Passent target square
	// FEN_parts[3]; Not implemented

	// Half move clock
	// FEN_parts[4]; Not implemented

	// Full move number
	// FEN_parts[5]; Not implemented

	generateLegalMoves();
}

Piece Board::charToPiece(char c)
{
	switch(c) {
		case 'P': return Piece(Type::Pawn,   Color::White);
		case 'R': return Piece(Type::Rook,   Color::White);
		case 'N': return Piece(Type::Knight, Color::White);
		case 'B': return Piece(Type::Bishop, Color::White);
		case 'Q': return Piece(Type::Queen,  Color::White);
		case 'K': return Piece(Type::King,   Color::White);

		case 'p': return Piece(Type::Pawn,   Color::Black);
		case 'r': return Piece(Type::Rook,   Color::Black);
		case 'n': return Piece(Type::Knight, Color::Black);
		case 'b': return Piece(Type::Bishop, Color::Black);
		case 'q': return Piece(Type::Queen,  Color::Black);
		case 'k': return Piece(Type::King,  Color::Black);
		default: std::cout << "Invalid char"; return Piece();
	}
	return Piece();
}

// Possible optimization: Dont use moveIsCastle and moveIsEnPassent helpers
// Will not check if move is legal
void Board::movePiece(const sf::Vector2i& old_square, const sf::Vector2i& new_square)
{
	logMove(old_square, new_square);

	if (new_square == old_square) return;

	// En passant
	if (board[old_square.x][old_square.y].getType() == Type::Pawn && !board[new_square.x][new_square.y] && new_square.x != old_square.x)
		board[new_square.x][old_square.y] = Piece();

	// Castling
	if (board[old_square.x][old_square.y].getType() == Type::King && abs(old_square.x - new_square.x) > 1)
		castle(old_square, new_square);

	// Pawn promotion
	if (board[old_square.x][old_square.y].getType() == Type::Pawn && (new_square.y == 0 || new_square.y == 7))
		board[old_square.x][old_square.y] = Piece(Type::Queen, board[old_square.x][old_square.y].getColor());

	board[new_square.x][new_square.y] = board[old_square.x][old_square.y];
	board[old_square.x][old_square.y] = Piece();
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

void Board::changeTurn() {
	if (whiteTurn) whiteTurn = false;
	else           whiteTurn = true;

	generateLegalMoves();
}

void Board::logMove(const sf::Vector2i& old_square, const sf::Vector2i& new_square) {
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

// !!! Expensive function because it loops through all the ally and enemy moves
void Board::generateLegalMoves()
{
	Color ally_color = whiteTurn ? Color::White : Color::Black;
	Color enemy_color = whiteTurn ? Color::Black : Color::White;

	std::vector<move> psudo_legal_moves;
	generatePsudoLegalMoves(ally_color, psudo_legal_moves);

	// Now we have to make every legal move and see if the enemy can capture the king
	auto it = psudo_legal_moves.begin();

	while (it != psudo_legal_moves.end()) {
		move m = *it;
		movePiece(m.first, m.second);

		std::vector<move> enemy_psudo_legal_moves;
		generatePsudoLegalMoves(enemy_color, enemy_psudo_legal_moves);

		bool move_exposes_king = false;

		for (move enemy_moves : enemy_psudo_legal_moves) {
			if (board[enemy_moves.second.x][enemy_moves.second.y].getType() == Type::King) {
				move_exposes_king = true;
				break; // No need to check the rest of the enemy moves
			}

			// Prevent moving over check
			if (board[m.second.x][m.second.y].getType() == Type::King && abs(m.first.x - m.second.x) > 1) {
				if (m.second.x == 2 && enemy_moves.second.x == 3 && enemy_moves.second.y == m.first.y) {
					move_exposes_king = true;
					break;
				}
				else if (m.second.x == 6 && enemy_moves.second.x == 5 && enemy_moves.second.y == m.first.y) {
					move_exposes_king = true;
					break;
				}
			}
		}

		softUndoMove();

		if (move_exposes_king) {
			it = psudo_legal_moves.erase(it);
		}
		else {
			it++;
		}
	}

	legal_moves = psudo_legal_moves;
}

// Possible optimization: Generate the moves for white and black so you only have to call this function once
// !!! Expensive function because it loops through the whole board
void Board::generatePsudoLegalMoves(Color& c, std::vector<move>& vec_to_append_moves_to)
{
	for (int x = 0; x < 8; x++) for (int y = 0; y < 8; y++) {
		if (!board[x][y]) continue;
		if (board[x][y].getColor() != c) continue;
		sf::Vector2i square(x, y);
		Type type = board[x][y].getType();
		Color color = board[x][y].getColor();

		switch (type) {
		case Type::Pawn:   appendPsudoLegalPawnMoves(square, color, vec_to_append_moves_to); break;
		case Type::Rook:   appendPsudoLegalRookMoves(square, color, vec_to_append_moves_to); break;
		case Type::Knight: appendPsudoLegalKnightMoves(square, color, vec_to_append_moves_to); break;
		case Type::Bishop: appendPsudoLegalBishopMoves(square, color, vec_to_append_moves_to); break;
		case Type::King:   appendPsudoLegalKingMoves(square, color, vec_to_append_moves_to); break;
		case Type::Queen:  appendPsudoLegalQueenMoves(square, color, vec_to_append_moves_to); break;
		}
	}
}

void Board::appendPsudoLegalPawnMoves(const sf::Vector2i& sq, const Color& c, std::vector<move>& moves)
{
	if (c == Color::White) {
		if (!board[sq.x][sq.y - 1]) {
			moves.push_back({ sq, sf::Vector2i(sq.x, sq.y - 1) });
			if (sq.y == 6 && !board[sq.x][4])
				moves.push_back({sq, sf::Vector2i(sq.x, 4)});
		}
		// Capture up and to the right
		if (sq.x != 7 && board[sq.x+1][sq.y-1] && board[sq.x+1][sq.y-1].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x+1, sq.y-1)});
		// Capture up and to the left
		if (sq.x != 0 && board[sq.x-1][sq.y-1] && board[sq.x-1][sq.y-1].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x-1, sq.y-1)});
	}
	else {
		if (!board[sq.x][sq.y + 1]) {
			moves.push_back({ sq, sf::Vector2i(sq.x, sq.y + 1) });
			if (sq.y == 1 && !board[sq.x][3])
				moves.push_back({ sq, sf::Vector2i(sq.x, 3) });
		}
		// Capture down and to the right
		if (sq.x != 7 && board[sq.x+1][sq.y+1] && board[sq.x+1][sq.y+1].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x+1, sq.y+1)});
		// Capture down and to the left
		if (sq.x != 0 && board[sq.x-1][sq.y+1] && board[sq.x-1][sq.y+1].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x-1, sq.y+1)});
	}

	// En Passent logic:
	if (log.size() > 0 && std::get<0>(log.back()).getType() == Type::Pawn) {
		auto last_move = log.back();

		sf::Vector2i last_old_square = std::get<1>(last_move);
		sf::Vector2i last_new_square = std::get<3>(last_move);

		// If last move was an enemy double pawn push next to an ally pawn
		// AND on the same rank as sq AND next to sq
		if (abs(last_old_square.y - last_new_square.y) >= 2
			&& std::get<0>(last_move).getColor() != c
			&& sq.y == last_new_square.y
			&& (sq.x == last_new_square.x - 1 || sq.x == last_new_square.x + 1)
			) {
			if (c == Color::White) 
				moves.push_back({ sq, sf::Vector2i(last_new_square.x, last_new_square.y - 1)});
			else
				moves.push_back({ sq, sf::Vector2i(last_new_square.x, last_new_square.y + 1) });
		}
	}
}

void Board::appendPsudoLegalRookMoves(const sf::Vector2i& sq, const Color& c, std::vector<move>& moves)
{
	// Look for moves upward
	for (int y = sq.y - 1; y >= 0; y--) {
		if (!board[sq.x][y]) {
			moves.push_back({ sq, sf::Vector2i(sq.x, y) });
		}
		else {
			if (board[sq.x][y].getColor() == c)
				break;
			if (board[sq.x][y].getColor() != c) {
				moves.push_back({ sq, sf::Vector2i(sq.x, y) });
				break;
			}
		}
	}

	// Look for moves downward
	for (int y = sq.y + 1; y <= 7; y++) {
		if (!board[sq.x][y]) {
			moves.push_back({ sq, sf::Vector2i(sq.x, y) });
		}
		else {
			if (board[sq.x][y].getColor() == c)
				break;
			if (board[sq.x][y].getColor() != c) {
				moves.push_back({ sq, sf::Vector2i(sq.x, y) });
				break;
			}
		}
	}

	// Look for moves left
	for (int x = sq.x - 1; x >= 0; x--) {
		if (!board[x][sq.y])
			moves.push_back({ sq, sf::Vector2i(x, sq.y) });
		else {
			if (board[x][sq.y].getColor() == c)
				break;
			else {
				moves.push_back({ sq, sf::Vector2i(x, sq.y) });
				break;
			}
		}
	}

	// Look for moves right
	for (int x = sq.x + 1; x <= 7; x++) {
		if (!board[x][sq.y])
			moves.push_back({ sq, sf::Vector2i(x, sq.y) });
		else {
			if (board[x][sq.y].getColor() == c)
				break;
			else {
				moves.push_back({ sq, sf::Vector2i(x, sq.y) });
				break;
			}
		}
	}
}

void Board::appendPsudoLegalKnightMoves(const sf::Vector2i& sq, const Color& c, std::vector<move>& moves)
{
	if (sq.x >= 1 && sq.y >= 2) {
		if (!board[sq.x-1][sq.y-2] || board[sq.x-1][sq.y-2].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x - 1, sq.y - 2)}); // up left
	}

	if (sq.x <= 6 && sq.y >= 2) {
		if (!board[sq.x+1][sq.y-2] || board[sq.x+1][sq.y-2].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x + 1, sq.y - 2)}); // up right
	}

	if (sq.x <= 5 && sq.y > 0) {
		if (!board[sq.x+2][sq.y-1] || board[sq.x+2][sq.y-1].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x + 2, sq.y - 1)}); // right up
	}

	if (sq.x <= 5 && sq.y <= 6) {
		if (!board[sq.x+2][sq.y+1] || board[sq.x+2][sq.y+1].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x + 2, sq.y + 1)}); // right down
	}

	if (sq.x <= 6 && sq.y <= 5) {
		if (!board[sq.x+1][sq.y+2] || board[sq.x+1][sq.y+2].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x + 1, sq.y + 2)}); // down right
	}

	if (sq.x >= 1 && sq.y <= 5) {
		if (!board[sq.x-1][sq.y+2] || board[sq.x-1][sq.y+2].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x - 1, sq.y + 2)}); // down left
	}

	if (sq.x >= 2 && sq.y <= 6) {
		if (!board[sq.x-2][sq.y+1] || board[sq.x-2][sq.y+1].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x - 2, sq.y + 1)}); // left down
	}

	if (sq.x >= 2 && sq.y >= 1) {
		if (!board[sq.x-2][sq.y-1] || board[sq.x-2][sq.y-1].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x - 2, sq.y - 1)}); // left up
	}
}

void Board::appendPsudoLegalBishopMoves(const sf::Vector2i& sq, const Color& c, std::vector<move>& moves)
{
	// Generating up left
	int i = 0;
	sf::Vector2i tgt(sq.x - 1, sq.y - 1);
	while (tgt.x >= 0 && tgt.y >= 0) {
		if (!board[tgt.x][tgt.y])
			moves.push_back({ sq, tgt });
		else {
			if (board[tgt.x][tgt.y].getColor() == c)
				break;
			else {
				moves.push_back({ sq, tgt });
				break;
			}
		}
		tgt.x -= 1;
		tgt.y -= 1;
	}

	// Generate up right
	i = 0;
	tgt = sf::Vector2i(sq.x + 1, sq.y - 1);
	while (tgt.x <= 7 && tgt.y >= 0) {
		if (!board[tgt.x][tgt.y])
			moves.push_back({ sq, tgt });
		else {
			if (board[tgt.x][tgt.y].getColor() == c)
				break;
			else {
				moves.push_back({ sq, tgt });
				break;
			}
		}
		tgt.x += 1;
		tgt.y -= 1;
	}

	// Generate down left
	i = 0;
	tgt = sf::Vector2i(sq.x - 1, sq.y + 1);
	while (tgt.x >= 0 && tgt.y <= 7) {
		if (!board[tgt.x][tgt.y])
			moves.push_back({ sq, tgt });
		else {
			if (board[tgt.x][tgt.y].getColor() == c)
				break;
			else {
				moves.push_back({ sq, tgt });
				break;
			}
		}
		tgt.x -= 1;
		tgt.y += 1;
	}

	// Generate down right
	i = 0;
	tgt = sf::Vector2i(sq.x + 1, sq.y + 1);
	while (tgt.x <= 7 && tgt.y <= 7) {
		if (!board[tgt.x][tgt.y])
			moves.push_back({ sq, tgt });
		else {
			if (board[tgt.x][tgt.y].getColor() == c)
				break;
			else {
				moves.push_back({ sq, tgt });
				break;
			}
		}
		tgt.x += 1;
		tgt.y += 1;
	}
}

void Board::appendPsudoLegalKingMoves(const sf::Vector2i& sq, const Color& c, std::vector<move>& moves)
{
	if (sq.x != 0 && sq.y != 0) {
		if (!board[sq.x-1][sq.y-1] || board[sq.x-1][sq.y-1].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x - 1, sq.y - 1)}); // top left
	}

	if (sq.y != 0) {
		if (!board[sq.x][sq.y-1] || board[sq.x][sq.y-1].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x    , sq.y - 1)}); // top
	}

	if (sq.x != 7 && sq.y != 0) {
		if (!board[sq.x+1][sq.y-1] || board[sq.x+1][sq.y-1].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x + 1, sq.y - 1)}); // top right
	}

	if (sq.x != 7) {
		if (!board[sq.x+1][sq.y] || board[sq.x+1][sq.y].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x + 1, sq.y    )}); // right
	}

	if (sq.x != 7 && sq.y != 7) {
		if (!board[sq.x+1][sq.y+1] || board[sq.x+1][sq.y+1].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x + 1, sq.y + 1)}); // bottom right
	}

	if (sq.y != 7) {
		if (!board[sq.x][sq.y+1] || board[sq.x][sq.y+1].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x    , sq.y + 1)}); // bottom
	}

	if (sq.x != 0 && sq.y != 7) {
		if (!board[sq.x-1][sq.y+1] || board[sq.x-1][sq.y+1].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x - 1, sq.y + 1)}); // bottom left
	}

	if (sq.x != 0) {
		if (!board[sq.x-1][sq.y] || board[sq.x-1][sq.y].getColor() != c)
			moves.push_back({sq, sf::Vector2i(sq.x - 1, sq.y)});     // left
	}

	// Castling logic
	if (c == Color::White) {
		// White king side castle
		if (!hasPieceMoved(sf::Vector2i(4, 7)) && !hasPieceMoved(sf::Vector2i(7, 7)) && !board[5][7] && !board[6][7])
			moves.push_back({sq, sf::Vector2i(6, 7)});
		// White queen side castle
		if (!hasPieceMoved(sf::Vector2i(4, 7)) && !hasPieceMoved(sf::Vector2i(0, 7)) && !board[1][7] && !board[2][7] && !board[3][7])
			moves.push_back({sq, sf::Vector2i(2, 7)});
	}
	else {
		// Black king side castle
		if (!hasPieceMoved(sf::Vector2i(4, 0)) && !hasPieceMoved(sf::Vector2i(7, 0)) && !board[5][0] && !board[6][0])
			moves.push_back({sq, sf::Vector2i(6, 0)});
		// Black queen side castle
		if (!hasPieceMoved(sf::Vector2i(4, 0)) && !hasPieceMoved(sf::Vector2i(0, 0)) && !board[1][0] && !board[2][0] && !board[3][0])
			moves.push_back({sq, sf::Vector2i(2, 0)});
	}
}

void Board::appendPsudoLegalQueenMoves(const sf::Vector2i& sq, const Color& c, std::vector<move>& moves)
{
	appendPsudoLegalRookMoves(sq, c, moves);
	appendPsudoLegalBishopMoves(sq, c, moves);
}

// Will not change turn or generate moves after undo
void Board::softUndoMove()
{
	if (log.size() == 0) return;
	auto last_log = log.back();

	Piece old_piece = std::get<0>(last_log);
	sf::Vector2i old_square = std::get<1>(last_log);

	Piece new_piece = std::get<2>(last_log);
	sf::Vector2i new_square = std::get<3>(last_log);

	// Undo castle
	if (old_piece.getType() == Type::King && abs(old_square.x - new_square.x) > 1) {
		// If undoing left castle
		if (old_square.x > new_square.x) {
			board[0][old_square.y] = Piece(Type::Rook, old_piece.getColor());
			board[3][old_square.y] = Piece();
		}
		// If undoing right castle
		else {
			board[7][old_square.y] = Piece(Type::Rook, old_piece.getColor());
			board[5][old_square.y] = Piece();
		}
	}

	// Undo en passent
	if (old_piece.getType() == Type::Pawn && new_piece.getType() == Type::None && old_square.x != new_square.x) {
		// Should place a pawn behind the new square
		if (old_piece.getColor() == Color::White)
			board[new_square.x][new_square.y + 1] = Piece(Type::Pawn, Color::Black);
		else
			board[new_square.x][new_square.y - 1] = Piece(Type::Pawn, Color::White);
	}

	board[old_square.x][old_square.y] = old_piece;
	board[new_square.x][new_square.y] = new_piece;

	log.pop_back();
	// Reset game over in case move caused draw or such
	whiteVictory = false; blackVictory = false; draw = false;
}

// !!! Potentially expensive; will look through log
bool Board::hasPieceMoved(const sf::Vector2i& sq)
{
	for (auto move : log) {
		if (std::get<1>(move) == sq || std::get<3>(move) == sq)
			return true;
	}
	return false;
}

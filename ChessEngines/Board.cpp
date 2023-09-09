#include "Board.h"


Board::Board() {
	importFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

// Returns true if a move is excecuted
void Board::makeMove(const sf::Vector2i old_square, const sf::Vector2i new_square) {
	const std::vector<move>& legal_moves = generateLegalMoves();
	if (legal_moves.size() == 0) return;
	move m = { old_square, new_square };
	auto it = std::find(legal_moves.begin(), legal_moves.end(), m);
	if (it == legal_moves.end()) return;

	makeSafeMove(old_square, new_square);
}

void Board::makeSafeMove(const sf::Vector2i old_square, const sf::Vector2i new_square)
{
	movePiece(old_square, new_square);
	changeTurn();
	checkGameOver();
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
	if (old_piece.getType() == Type::King && abs(old_square.x - new_square.x) > 1) {
		// If undoing left castle
		if (old_square.x > new_square.x) {
			pieceAt(0, old_square.y) = Piece(Type::Rook, old_piece.getColor());
			pieceAt(3, old_square.y) = Piece();
		}
		// If undoing right castle
		else {
			pieceAt(7, old_square.y) = Piece(Type::Rook, old_piece.getColor());
			pieceAt(5, old_square.y) = Piece();
		}
	}

	// Undo en passent
	if (old_piece.getType() == Type::Pawn && new_piece.getType() == Type::None && old_square.x != new_square.x) {
		// Should place a pawn behind the new square
		if (old_piece.getColor() == Color::White)
			pieceAt(new_square.x, new_square.y + 1) = Piece(Type::Pawn, Color::Black);
		else
			pieceAt(new_square.x, new_square.y - 1) = Piece(Type::Pawn, Color::White);
	}

	pieceAt(old_square.x, old_square.y) = old_piece;
	pieceAt(new_square.x, new_square.y) = new_piece;

	log.pop_back();
	// Reset game over in case move caused draw or such
	whiteVictory = false; blackVictory = false; draw = false;
	changeTurn();
}

// !!! Expensive call
std::vector<std::pair<sf::Vector2i, sf::Vector2i>> Board::getMoves()
{
	return generateLegalMoves();
}


const std::pair<sf::Vector2i, sf::Vector2i> Board::getLastMove() const
{
	if (log.size() == 0) return { sf::Vector2i(-1, -1), sf::Vector2i(-1, -1) };
	return { std::get<1>(log.back()), std::get<3>(log.back()) };
}

bool Board::isGameOver() const
{
	return draw || whiteVictory || blackVictory;
}

void Board::importFEN(std::string FEN)
{
	log.clear();
	whiteVictory = false; blackVictory = false; draw = false;

	for (int x = 0; x < 8; x++) for (int y = 0; y < 8; y++) pieceAt(x, y) = Piece();

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
				pieceAt(x, y) = charToPiece(c);
				x++;
			}
		}
		x = 0;
		y++;
	}

	// Set turn
	if (FEN_parts[1] == "w" && !whiteTurn) changeTurn();
	if (FEN_parts[1] == "b" && whiteTurn) changeTurn();

	// Castling Rights
	if (FEN_parts[2].find('K') == std::string::npos) // Log a kingside white rook move
		log.push_back({ Piece(Type::Rook, Color::White), sf::Vector2i(7, 7), Piece(), sf::Vector2i(7, 7) });
	if (FEN_parts[2].find('Q') == std::string::npos) // Log a queenside white rook move
		log.push_back({ Piece(Type::Rook, Color::White), sf::Vector2i(0, 7), Piece(), sf::Vector2i(0, 7) });
	if (FEN_parts[2].find('k') == std::string::npos) // Log a kingside black rook move
		log.push_back({ Piece(Type::Rook, Color::Black), sf::Vector2i(7, 0), Piece(), sf::Vector2i(7, 0) });
	if (FEN_parts[2].find('q') == std::string::npos) // Log a queenside black rook move
		log.push_back({ Piece(Type::Rook, Color::Black), sf::Vector2i(0, 0), Piece(), sf::Vector2i(0, 0) });

	// En Passent target square
	if (FEN_parts[3] != "-") {
		sf::Vector2i target_square = stringToVec(FEN_parts[3]);
		if (target_square.y == 2) // Log a black double pawn push in front of the target square
			log.push_back({ Piece(Type::Pawn, Color::Black), sf::Vector2i(target_square.x, 1), Piece(), sf::Vector2i(target_square.x, 3) });
		else // Log a white double pawn push in front of the target square
			log.push_back({ Piece(Type::Pawn, Color::White), sf::Vector2i(target_square.x, 6), Piece(), sf::Vector2i(target_square.x, 4) });
	}

	// Half move clock
	// FEN_parts[4]; Not implemented

	// Full move number
	// FEN_parts[5]; This board does not store move counts
}

sf::Vector2i Board::stringToVec(std::string s)
{
	sf::Vector2i v(-1, -1);

	switch (s[0]) {
	case 'a': v.x = 0; break;
	case 'b': v.x = 1; break;
	case 'c': v.x = 2; break;
	case 'd': v.x = 3; break;
	case 'e': v.x = 4; break;
	case 'f': v.x = 5; break;
	case 'g': v.x = 6; break;
	case 'h': v.x = 7; break;
	}
	switch (s[1]) { // I am sorry for this
	case '8': v.y = 0; break;
	case '7': v.y = 1; break;
	case '6': v.y = 2; break;
	case '5': v.y = 3; break;
	case '4': v.y = 4; break;
	case '3': v.y = 5; break;
	case '2': v.y = 6; break;
	case '1': v.y = 7; break;
	}
	return v;
}

Piece Board::charToPiece(char c)
{
	switch (c) {
	case 'P': return Piece(Type::Pawn, Color::White);
	case 'R': return Piece(Type::Rook, Color::White);
	case 'N': return Piece(Type::Knight, Color::White);
	case 'B': return Piece(Type::Bishop, Color::White);
	case 'Q': return Piece(Type::Queen, Color::White);
	case 'K': return Piece(Type::King, Color::White);

	case 'p': return Piece(Type::Pawn, Color::Black);
	case 'r': return Piece(Type::Rook, Color::Black);
	case 'n': return Piece(Type::Knight, Color::Black);
	case 'b': return Piece(Type::Bishop, Color::Black);
	case 'q': return Piece(Type::Queen, Color::Black);
	case 'k': return Piece(Type::King, Color::Black);
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
	if (pieceAt(old_square).getType() == Type::Pawn && !pieceAt(new_square) && new_square.x != old_square.x)
		pieceAt(new_square.x, old_square.y) = Piece();

	// Castling
	if (pieceAt(old_square).getType() == Type::King && abs(old_square.x - new_square.x) > 1)
		castle(old_square, new_square);

	// Pawn promotion
	if (pieceAt(old_square).getType() == Type::Pawn && (new_square.y == 0 || new_square.y == 7))
		pieceAt(old_square) = Piece(Type::Queen, pieceAt(old_square).getColor());

	pieceAt(new_square) = pieceAt(old_square);
	pieceAt(old_square) = Piece();
}

void Board::castle(const sf::Vector2i& old_square, const sf::Vector2i& new_square)
{
	// The king move will have already been excecuted so just need to move rook

	// If the king is trying to castle left
	if (old_square.x > new_square.x) {
		pieceAt(3, old_square.y) = pieceAt(0, old_square.y);
		pieceAt(0, old_square.y) = Piece();
	}

	// If the king is trying to castle right
	if (old_square.x < new_square.x) {
		pieceAt(5, old_square.y) = pieceAt(7, old_square.y);
		pieceAt(7, old_square.y) = Piece();
	}
}

void Board::changeTurn() {
	if (whiteTurn) whiteTurn = false;
	else           whiteTurn = true;
}

void Board::logMove(const sf::Vector2i& old_square, const sf::Vector2i& new_square) {
	log.push_back({ pieceAt(old_square), old_square,
					pieceAt(new_square), new_square });
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
std::vector<Board::move> Board::generateLegalMoves()
{
	Color ally_color = whiteTurn ? Color::White : Color::Black;
	Color enemy_color = whiteTurn ? Color::Black : Color::White;

	sf::Vector2i ally_king = getKingSquare(ally_color);

	std::vector<move> psudo_legal_moves;
	generatePsudoLegalMoves(ally_color, psudo_legal_moves);


	// Now we have to make every legal move and see if the enemy can capture the king
	auto it = psudo_legal_moves.begin();

	while (it != psudo_legal_moves.end()) {

		move m = *it;
		movePiece(m.first, m.second);

		sf::Vector2i ally_king = getKingSquare(ally_color);

		bool move_exposes_king = false;

		for (int x = 0; x < 8; x++) for (int y = 0; y < 8; y++) {
			if (pieceAt(x, y).getColor() == ally_color) continue;
			if (isSquareAttacked(sf::Vector2i(x, y), ally_king)) {
				move_exposes_king = true;
				break;
			}
		}

		// If castline make sure the king does not move over check
		if (pieceAt(m.second).getType() == Type::King && abs(m.first.x - m.second.x) > 1) {
			for (int x = 0; x < 8; x++) for (int y = 0; y < 8; y++) {
				if (pieceAt(x, y).getColor() == ally_color) continue;
				// Left side castle
				if (m.second.x == 2 && isSquareAttacked(sf::Vector2i(x, y), sf::Vector2i(3, ally_king.y))) {
					move_exposes_king = true;
					break;
				}
				// Right side castle
				if (m.second.x == 6 && isSquareAttacked(sf::Vector2i(x, y), sf::Vector2i(5, ally_king.y))) {
					move_exposes_king = true;
					break;
				}
			}
		}

		undoMove();
		changeTurn();

		if (move_exposes_king) {
			it = psudo_legal_moves.erase(it);
		}
		else {
			it++;
		}
	}

	return psudo_legal_moves;
}

// Possible optimization: Generate the moves for white and black so you only have to call this function once
// !!! Expensive function because it loops through the whole board
void Board::generatePsudoLegalMoves(Color& c, std::vector<move>& vec_to_append_moves_to)
{
	for (int x = 0; x < 8; x++) for (int y = 0; y < 8; y++) {
		if (!pieceAt(x, y)) continue;
		if (pieceAt(x, y).getColor() != c) continue;
		sf::Vector2i square(x, y);
		Type type = pieceAt(x, y).getType();
		Color color = pieceAt(x, y).getColor();

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
		if (!pieceAt(sq.x, sq.y - 1)) {
			moves.push_back({ sq, sf::Vector2i(sq.x, sq.y - 1) });
			if (sq.y == 6 && !pieceAt(sq.x, 4))
				moves.push_back({ sq, sf::Vector2i(sq.x, 4) });
		}
		// Capture up and to the right
		if (sq.x != 7 && pieceAt(sq.x + 1, sq.y - 1) && pieceAt(sq.x + 1, sq.y - 1).getColor() != c)
			moves.push_back({ sq, sf::Vector2i(sq.x + 1, sq.y - 1) });
		// Capture up and to the left
		if (sq.x != 0 && pieceAt(sq.x - 1, sq.y - 1) && pieceAt(sq.x - 1, sq.y - 1).getColor() != c)
			moves.push_back({ sq, sf::Vector2i(sq.x - 1, sq.y - 1) });
	}
	else {
		if (!pieceAt(sq.x, sq.y + 1)) {
			moves.push_back({ sq, sf::Vector2i(sq.x, sq.y + 1) });
			if (sq.y == 1 && !pieceAt(sq.x, 3))
				moves.push_back({ sq, sf::Vector2i(sq.x, 3) });
		}
		// Capture down and to the right
		if (sq.x != 7 && pieceAt(sq.x + 1, sq.y + 1) && pieceAt(sq.x + 1, sq.y + 1).getColor() != c)
			moves.push_back({ sq, sf::Vector2i(sq.x + 1, sq.y + 1) });
		// Capture down and to the left
		if (sq.x != 0 && pieceAt(sq.x - 1, sq.y + 1) && pieceAt(sq.x - 1, sq.y + 1).getColor() != c)
			moves.push_back({ sq, sf::Vector2i(sq.x - 1, sq.y + 1) });
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
				moves.push_back({ sq, sf::Vector2i(last_new_square.x, last_new_square.y - 1) });
			else
				moves.push_back({ sq, sf::Vector2i(last_new_square.x, last_new_square.y + 1) });
		}
	}
}

void Board::appendPsudoLegalRookMoves(const sf::Vector2i& sq, const Color& c, std::vector<move>& moves)
{
	// Look for moves upward
	for (int y = sq.y - 1; y >= 0; y--) {
		if (!pieceAt(sq.x, y)) {
			moves.push_back({ sq, sf::Vector2i(sq.x, y) });
		}
		else {
			if (pieceAt(sq.x, y).getColor() == c)
				break;
			if (pieceAt(sq.x, y).getColor() != c) {
				moves.push_back({ sq, sf::Vector2i(sq.x, y) });
				break;
			}
		}
	}

	// Look for moves downward
	for (int y = sq.y + 1; y <= 7; y++) {
		if (!pieceAt(sq.x, y)) {
			moves.push_back({ sq, sf::Vector2i(sq.x, y) });
		}
		else {
			if (pieceAt(sq.x, y).getColor() == c)
				break;
			if (pieceAt(sq.x, y).getColor() != c) {
				moves.push_back({ sq, sf::Vector2i(sq.x, y) });
				break;
			}
		}
	}

	// Look for moves left
	for (int x = sq.x - 1; x >= 0; x--) {
		if (!pieceAt(x, sq.y))
			moves.push_back({ sq, sf::Vector2i(x, sq.y) });
		else {
			if (pieceAt(x, sq.y).getColor() == c)
				break;
			else {
				moves.push_back({ sq, sf::Vector2i(x, sq.y) });
				break;
			}
		}
	}

	// Look for moves right
	for (int x = sq.x + 1; x <= 7; x++) {
		if (!pieceAt(x, sq.y))
			moves.push_back({ sq, sf::Vector2i(x, sq.y) });
		else {
			if (pieceAt(x, sq.y).getColor() == c)
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
		if (!pieceAt(sq.x - 1, sq.y - 2) || pieceAt(sq.x - 1, sq.y - 2).getColor() != c)
			moves.push_back({ sq, sf::Vector2i(sq.x - 1, sq.y - 2) }); // up left
	}

	if (sq.x <= 6 && sq.y >= 2) {
		if (!pieceAt(sq.x + 1, sq.y - 2) || pieceAt(sq.x + 1, sq.y - 2).getColor() != c)
			moves.push_back({ sq, sf::Vector2i(sq.x + 1, sq.y - 2) }); // up right
	}

	if (sq.x <= 5 && sq.y > 0) {
		if (!pieceAt(sq.x + 2, sq.y - 1) || pieceAt(sq.x + 2, sq.y - 1).getColor() != c)
			moves.push_back({ sq, sf::Vector2i(sq.x + 2, sq.y - 1) }); // right up
	}

	if (sq.x <= 5 && sq.y <= 6) {
		if (!pieceAt(sq.x + 2, sq.y + 1) || pieceAt(sq.x + 2, sq.y + 1).getColor() != c)
			moves.push_back({ sq, sf::Vector2i(sq.x + 2, sq.y + 1) }); // right down
	}

	if (sq.x <= 6 && sq.y <= 5) {
		if (!pieceAt(sq.x + 1, sq.y + 2) || pieceAt(sq.x + 1, sq.y + 2).getColor() != c)
			moves.push_back({ sq, sf::Vector2i(sq.x + 1, sq.y + 2) }); // down right
	}

	if (sq.x >= 1 && sq.y <= 5) {
		if (!pieceAt(sq.x - 1, sq.y + 2) || pieceAt(sq.x - 1, sq.y + 2).getColor() != c)
			moves.push_back({ sq, sf::Vector2i(sq.x - 1, sq.y + 2) }); // down left
	}

	if (sq.x >= 2 && sq.y <= 6) {
		if (!pieceAt(sq.x - 2, sq.y + 1) || pieceAt(sq.x - 2, sq.y + 1).getColor() != c)
			moves.push_back({ sq, sf::Vector2i(sq.x - 2, sq.y + 1) }); // left down
	}

	if (sq.x >= 2 && sq.y >= 1) {
		if (!pieceAt(sq.x - 2, sq.y - 1) || pieceAt(sq.x - 2, sq.y - 1).getColor() != c)
			moves.push_back({ sq, sf::Vector2i(sq.x - 2, sq.y - 1) }); // left up
	}
}

void Board::appendPsudoLegalBishopMoves(const sf::Vector2i& sq, const Color& c, std::vector<move>& moves)
{
	// Generating up left
	sf::Vector2i tgt(sq.x - 1, sq.y - 1);
	while (tgt.x >= 0 && tgt.y >= 0) {
		if (!pieceAt(tgt.x, tgt.y))
			moves.push_back({ sq, tgt });
		else {
			if (pieceAt(tgt.x, tgt.y).getColor() == c)
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
	tgt = sf::Vector2i(sq.x + 1, sq.y - 1);
	while (tgt.x <= 7 && tgt.y >= 0) {
		if (!pieceAt(tgt.x, tgt.y))
			moves.push_back({ sq, tgt });
		else {
			if (pieceAt(tgt.x, tgt.y).getColor() == c)
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
	tgt = sf::Vector2i(sq.x - 1, sq.y + 1);
	while (tgt.x >= 0 && tgt.y <= 7) {
		if (!pieceAt(tgt.x, tgt.y))
			moves.push_back({ sq, tgt });
		else {
			if (pieceAt(tgt.x, tgt.y).getColor() == c)
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
	tgt = sf::Vector2i(sq.x + 1, sq.y + 1);
	while (tgt.x <= 7 && tgt.y <= 7) {
		if (!pieceAt(tgt.x, tgt.y))
			moves.push_back({ sq, tgt });
		else {
			if (pieceAt(tgt.x, tgt.y).getColor() == c)
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
		if (!pieceAt(sq.x - 1, sq.y - 1) || pieceAt(sq.x - 1, sq.y - 1).getColor() != c)
			moves.push_back({ sq, sf::Vector2i(sq.x - 1, sq.y - 1) }); // top left
	}

	if (sq.y != 0) {
		if (!pieceAt(sq.x, sq.y - 1) || pieceAt(sq.x, sq.y - 1).getColor() != c)
			moves.push_back({ sq, sf::Vector2i(sq.x    , sq.y - 1) }); // top
	}

	if (sq.x != 7 && sq.y != 0) {
		if (!pieceAt(sq.x + 1, sq.y - 1) || pieceAt(sq.x + 1, sq.y - 1).getColor() != c)
			moves.push_back({ sq, sf::Vector2i(sq.x + 1, sq.y - 1) }); // top right
	}

	if (sq.x != 7) {
		if (!pieceAt(sq.x + 1, sq.y) || pieceAt(sq.x + 1, sq.y).getColor() != c)
			moves.push_back({ sq, sf::Vector2i(sq.x + 1, sq.y) }); // right
	}

	if (sq.x != 7 && sq.y != 7) {
		if (!pieceAt(sq.x + 1, sq.y + 1) || pieceAt(sq.x + 1, sq.y + 1).getColor() != c)
			moves.push_back({ sq, sf::Vector2i(sq.x + 1, sq.y + 1) }); // bottom right
	}

	if (sq.y != 7) {
		if (!pieceAt(sq.x, sq.y + 1) || pieceAt(sq.x, sq.y + 1).getColor() != c)
			moves.push_back({ sq, sf::Vector2i(sq.x    , sq.y + 1) }); // bottom
	}

	if (sq.x != 0 && sq.y != 7) {
		if (!pieceAt(sq.x - 1, sq.y + 1) || pieceAt(sq.x - 1, sq.y + 1).getColor() != c)
			moves.push_back({ sq, sf::Vector2i(sq.x - 1, sq.y + 1) }); // bottom left
	}

	if (sq.x != 0) {
		if (!pieceAt(sq.x - 1, sq.y) || pieceAt(sq.x - 1, sq.y).getColor() != c)
			moves.push_back({ sq, sf::Vector2i(sq.x - 1, sq.y) });     // left
	}

	// Castling logic
	if (c == Color::White) {
		// White king side castle
		if (!hasPieceMoved(sf::Vector2i(4, 7)) && !hasPieceMoved(sf::Vector2i(7, 7)) && !pieceAt(5, 7) && !pieceAt(6, 7))
			moves.push_back({ sq, sf::Vector2i(6, 7) });
		// White queen side castle
		if (!hasPieceMoved(sf::Vector2i(4, 7)) && !hasPieceMoved(sf::Vector2i(0, 7)) && !pieceAt(1, 7) && !pieceAt(2, 7) && !pieceAt(3, 7))
			moves.push_back({ sq, sf::Vector2i(2, 7) });
	}
	else {
		// Black king side castle
		if (!hasPieceMoved(sf::Vector2i(4, 0)) && !hasPieceMoved(sf::Vector2i(7, 0)) && !pieceAt(5, 0) && !pieceAt(6, 0))
			moves.push_back({ sq, sf::Vector2i(6, 0) });
		// Black queen side castle
		if (!hasPieceMoved(sf::Vector2i(4, 0)) && !hasPieceMoved(sf::Vector2i(0, 0)) && !pieceAt(1, 0) && !pieceAt(2, 0) && !pieceAt(3, 0))
			moves.push_back({ sq, sf::Vector2i(2, 0) });
	}
}

void Board::appendPsudoLegalQueenMoves(const sf::Vector2i& sq, const Color& c, std::vector<move>& moves)
{
	appendPsudoLegalRookMoves(sq, c, moves);
	appendPsudoLegalBishopMoves(sq, c, moves);
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

void Board::checkGameOver()
{
	//std::tuple<bool, sf::Vector2i, sf::Vector2i> info = insufficientMaterial();

	//if (std::get<0>(info)) {
	//	draw = true;
	//	return;
	//}
	//
	//Color ally_color = whiteTurn ? Color::White : Color::Black;
	//Color enemy_color = whiteTurn ? Color::Black : Color::White;

	//sf::Vector2i ally_king = whiteTurn ? std::get<1>(info) : std::get<2>(info);

	//// See if ally has moves


	//for (int x = 0; x < 8; x++) for (int y = 0; y < 8; y++) {
	//	if (pieceAt(x, y).getColor() == enemy_color && isSquareAttacked(sf::Vector2i(x, y), ally_king)) {
	//		std::cout << "ally king is in check" << std::endl;
	//		return;
	//	}
	//}
}

// returns a tuple contatining: is there isnsufficent material, position of white king, position of black king.
std::tuple<bool, sf::Vector2i, sf::Vector2i> Board::insufficientMaterial() const
{
	std::tuple<bool, sf::Vector2i, sf::Vector2i> info = { true, sf::Vector2i(-1, -1), sf::Vector2i(-1, -1) };
	for (int x = 0; x < 8; x++) for (int y = 0; y < 8; y++) {
		if (pieceAt(x, y) && pieceAt(x, y).getType() == Type::King) {
			if (pieceAt(x, y).getColor() == Color::White) std::get<1>(info) = sf::Vector2i(x, y);
			else                                          std::get<2>(info) = sf::Vector2i(x, y);
		}
		else {
			std::get<0>(info) = false;
		}
	}

	return info;
}

// True if tgt is attacked by sq
bool Board::isSquareAttacked(const sf::Vector2i& sq, const sf::Vector2i& tgt)
{
	switch (pieceAt(sq).getType()) {
	case Type::Pawn:   return isPawnAttacking(sq, tgt);   break;
	case Type::Rook:   return isRookAttacking(sq, tgt);   break;
	case Type::Knight: return isKnightAttacking(sq, tgt); break;
	case Type::Bishop: return isBishopAttacking(sq, tgt); break;
	case Type::King:   return isKingAttacking(sq, tgt);   break;
	case Type::Queen:  return isQueenAttacking(sq, tgt);  break;
	}
	return false;
}

bool Board::isPawnAttacking(const sf::Vector2i& sq, const sf::Vector2i& tgt)
{
	Color c = pieceAt(sq).getColor();

	if (c == Color::White) {
		// Capture up and to the right
		if (tgt == sf::Vector2i(sq.x + 1, sq.y - 1))
			return true;
		// Capture up and to the left
		if (tgt == sf::Vector2i(sq.x - 1, sq.y - 1))
			return true;
	}
	else {
		// Capture down and to the right
		if (tgt == sf::Vector2i(sq.x + 1, sq.y + 1))
			return true;
		// Capture down and to the left
		if (tgt == sf::Vector2i(sq.x - 1, sq.y + 1))
			return true;
	}

	return false;
}

bool Board::isRookAttacking(const sf::Vector2i& sq, const sf::Vector2i& tgt)
{
	if (sq.x != tgt.x && sq.y != tgt.y) return false;

	// Look for moves upward
	for (int y = sq.y - 1; y >= 0; y--) {
		if (!pieceAt(sq.x, y)) {
			if (sf::Vector2i(sq.x, y) == tgt)
				return true;
		}
		else {
			if (sf::Vector2i(sq.x, y) == tgt)
				return true;
			break;
		}
	}

	// Look for moves downward
	for (int y = sq.y + 1; y <= 7; y++) {
		if (!pieceAt(sq.x, y)) {
			if (sf::Vector2i(sq.x, y) == tgt)
				return true;
		}
		else {
			if (sf::Vector2i(sq.x, y) == tgt)
				return true;
			break;
		}
	}

	// Look for moves left // BROKEN
	for (int x = sq.x - 1; x >= 0; x--) {
		if (!pieceAt(x, sq.y)) {
			if (sf::Vector2i(x, sq.y) == tgt)
				return true;
		}
		else {
			if (sf::Vector2i(x, sq.y) == tgt)
				return true;
			break;
		}
	}

	// Look for moves right // Also broken
	for (int x = sq.x + 1; x <= 7; x++) {
		if (!pieceAt(x, sq.y)) {
			if (sf::Vector2i(x, sq.y) == tgt)
				return true;
		}
		else {
			if (sf::Vector2i(x, sq.y) == tgt)
				return true;
			break;
		}
	}
	return false;
}

bool Board::isKnightAttacking(const sf::Vector2i& sq, const sf::Vector2i& tgt)
{
	if (abs(tgt.x - sq.x) == 1 && abs(tgt.y - sq.y) == 2) return true;
	if (abs(tgt.x - sq.x) == 2 && abs(tgt.y - sq.y) == 1) return true;
	return false;
}

bool Board::isBishopAttacking(const sf::Vector2i& sq, const sf::Vector2i& new_sq)
{
	// Generating up left
	sf::Vector2i tgt(sq.x - 1, sq.y - 1);
	while (tgt.x >= 0 && tgt.y >= 0) {
		if (tgt == new_sq)
			return true;
		if (pieceAt(tgt))
			break;

		tgt.x -= 1;
		tgt.y -= 1;
	}

	// Generate up right
	tgt = sf::Vector2i(sq.x + 1, sq.y - 1);
	while (tgt.x <= 7 && tgt.y >= 0) {
		if (tgt == new_sq)
			return true;
		if (pieceAt(tgt))
			break;

		tgt.x += 1;
		tgt.y -= 1;
	}

	// Generate down left
	tgt = sf::Vector2i(sq.x - 1, sq.y + 1);
	while (tgt.x >= 0 && tgt.y <= 7) {
		if (tgt == new_sq)
			return true;
		if (pieceAt(tgt))
			break;

		tgt.x -= 1;
		tgt.y += 1;
	}

	// Generate down right
	tgt = sf::Vector2i(sq.x + 1, sq.y + 1);
	while (tgt.x <= 7 && tgt.y <= 7) {
		if (tgt == new_sq)
			return true;
		if (pieceAt(tgt))
			break;

		tgt.x += 1;
		tgt.y += 1;
	}
	return false;
}

bool Board::isKingAttacking(const sf::Vector2i& sq, const sf::Vector2i& tgt)
{
	// Prevents the king from moving more than one square
	if (abs(tgt.x - sq.x) > 1 || abs(tgt.y - sq.y) > 1) return false;

	return true;
}

bool Board::isQueenAttacking(const sf::Vector2i& sq, const sf::Vector2i& tgt)
{
	return isRookAttacking(sq, tgt) || isBishopAttacking(sq, tgt);
}

bool Board::doesColorHaveMoves(Color c)
{
	return false;
}

sf::Vector2i Board::getKingSquare(Color c)
{
	int i = 0;
	for (const Piece& p : board) {
		if (p.getType() == Type::King && p.getColor() == c)
			return sf::Vector2i(i % 8, i / 8);
		i++;
	}

	return sf::Vector2i(-1, -1);
}

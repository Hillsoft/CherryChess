module;

#include <assert.h>

export module cherry.board;

import std;

import cherry.move;
import cherry.piece;
import cherry.squareIndex;

export namespace cherry {

	class Board {
	public:
		constexpr explicit Board(std::string_view fen) {
			size_t i;
			size_t row = 0;

			auto pullChar = [&]() {
				if (fen.size() == 0)
					throw std::runtime_error("Invalid FEN");
				char currentPiece = fen.at(0);
				fen = fen.substr(1);
				return currentPiece;
			};
			auto pushSquare = [&](Piece current) {
				if (i >= 64 || i / 8 != row)
					throw std::runtime_error("Invalid FEN");
				data_[i] = current;
				i++;
			};

			// Board state
			for (i = 0; i < 64 && fen.size() > 0;) {
				char currentPiece = pullChar();
				if ('0' <= currentPiece && currentPiece <= '9') {
					// We have a skip
					for (size_t j = 0; j < currentPiece - '0'; j++) {
						pushSquare(Piece::PieceNone);
					}
				}
				else {
					switch (currentPiece) {
					case '/':
						row++;
						break;
					case 'P':
						pushSquare(Piece::WhitePawn);
						break;
					case 'R':
						pushSquare(Piece::WhiteRook);
						break;
					case 'N':
						pushSquare(Piece::WhiteKnight);
						break;
					case 'B':
						pushSquare(Piece::WhiteBishop);
						break;
					case 'Q':
						pushSquare(Piece::WhiteQueen);
						break;
					case 'K':
						pushSquare(Piece::WhiteKing);
						break;
					case 'p':
						pushSquare(Piece::BlackPawn);
						break;
					case 'r':
						pushSquare(Piece::BlackRook);
						break;
					case 'n':
						pushSquare(Piece::BlackKnight);
						break;
					case 'b':
						pushSquare(Piece::BlackBishop);
						break;
					case 'q':
						pushSquare(Piece::BlackQueen);
						break;
					case 'k':
						pushSquare(Piece::BlackKing);
						break;
					default:
						throw std::runtime_error("Invalid FEN");
					}
				}
			}

			if (i != 64)
				throw std::runtime_error("Invalid FEN");

			if (pullChar() != ' ')
				throw std::runtime_error("Invalid FEN");

			// Turn player
			switch (pullChar()) {
			case 'w':
				whiteToPlay_ = true;
				break;
			case 'b':
				whiteToPlay_ = false;
				break;
			default:
				throw std::runtime_error("Invalid FEN");
			}

			if (pullChar() != ' ')
				throw std::runtime_error("Invalid FEN");

			// Castling rights
			whiteKingsideCastle_ = false;
			whiteQueensideCastle_ = false;
			blackKingsideCastle_ = false;
			blackQueensideCastle_ = false;
			char c;
			while ((c = pullChar()) != ' ') {
				switch (c) {
				case '-':
					break;
				case 'K':
					whiteKingsideCastle_ = true;
					break;
				case 'Q':
					whiteQueensideCastle_ = true;
					break;
				case 'k':
					blackKingsideCastle_ = true;
					break;
				case 'q':
					blackQueensideCastle_ = true;
					break;
				default:
					throw std::runtime_error("Invalid FEN");
				}
			}

			// En-passant
			// We can guarantee at least two characters as this is not the last item
			if (fen.size() < 2)
				throw std::runtime_error("Invalid FEN");
			if (fen.at(0) == '-') {
				pullChar();
			}
			else {
				std::string_view squareIndex = fen.substr(0, 2);
				if (squareIndex.at(0) < 'a' || squareIndex.at(0) > 'h' || squareIndex.at(1) < '1' || squareIndex.at(1) > '8')
					throw std::runtime_error("Invalid FEN");
				enPassantTarget_ = SquareIndex(squareIndex);
				pullChar(); pullChar();
			}

			if (pullChar() != ' ')
				throw std::runtime_error("Invalid FEN");

			// Half moves (for 50-move rule)
			halfMoveClock_ = 0;
			while ((c = pullChar()) != ' ') {
				if (c < '0' || c > '9')
					throw std::runtime_error("Invalid FEN");
				halfMoveClock_ *= 10;
				halfMoveClock_ += c - '0';
				if (halfMoveClock_ > 100)
					throw std::runtime_error("Invalid FEN");
			}

			// Move count (ignored for now)
			while (fen.size() > 0) {
				c = pullChar();
				if (c < '0' || c > '9')
					throw std::runtime_error("Invalid FEN");
			}
		}

		constexpr auto operator<=>(Board const& other) const = default;

		constexpr Piece at(SquareIndex i) const {
			return data_[i.getRawIndex()];
		}

		constexpr void makeMove(Move m) {
			// Half-move clock
			if (at(m.to_) != Piece::PieceNone || getPieceType(at(m.from_)) == PieceType::Pawn) {
				halfMoveClock_ = 0;
			}
			else {
				halfMoveClock_++;
			}

			// En passant
			if (m.to_ == enPassantTarget_ && getPieceType(at(m.from_)) == PieceType::Pawn) {
				char invStep = whiteToPlay_ == PieceColor::White ? 8 : -8;
				data_[m.to_.getRawIndex() + invStep] = Piece::PieceNone;
			}
			if (getPieceType(at(m.from_)) == PieceType::Pawn && std::abs(m.from_.getRawIndex() - m.to_.getRawIndex()) == 16) {
				enPassantTarget_ = SquareIndex((m.from_.getRawIndex() + m.to_.getRawIndex()) / 2);
			}
			else {
				enPassantTarget_ = nullSquareIndex;
			}

			// Losing castling rights
			if (m.from_ == SquareIndex("e1")) {
				whiteKingsideCastle_ = false;
				whiteQueensideCastle_ = false;
			}
			if (m.from_ == SquareIndex("e8")) {
				blackKingsideCastle_ = false;
				blackQueensideCastle_ = false;
			}
			if (m.from_ == SquareIndex("a1")) {
				whiteQueensideCastle_ = false;
			}
			if (m.from_ == SquareIndex("h1")) {
				whiteKingsideCastle_ = false;
			}
			if (m.from_ == SquareIndex("a8")) {
				blackQueensideCastle_ = false;
			}
			if (m.from_ == SquareIndex("h8")) {
				blackKingsideCastle_ = false;
			}

			// Move rook if castling
			if (getPieceType(at(m.from_)) == PieceType::King) {
				if (m.from_ == SquareIndex("e1") && m.to_ == SquareIndex("g1")) {
					data_[SquareIndex("h1").getRawIndex()] = Piece::PieceNone;
					data_[SquareIndex("f1").getRawIndex()] = Piece::WhiteRook;
				}
				if (m.from_ == SquareIndex("e1") && m.to_ == SquareIndex("c1")) {
					data_[SquareIndex("a1").getRawIndex()] = Piece::PieceNone;
					data_[SquareIndex("d1").getRawIndex()] = Piece::WhiteRook;
				}
				if (m.from_ == SquareIndex("e8") && m.to_ == SquareIndex("g8")) {
					data_[SquareIndex("h8").getRawIndex()] = Piece::PieceNone;
					data_[SquareIndex("f8").getRawIndex()] = Piece::BlackRook;
				}
				if (m.from_ == SquareIndex("e8") && m.to_ == SquareIndex("c8")) {
					data_[SquareIndex("a8").getRawIndex()] = Piece::PieceNone;
					data_[SquareIndex("d8").getRawIndex()] = Piece::BlackRook;
				}
			}

			data_[m.to_.getRawIndex()] = data_[m.from_.getRawIndex()];
			data_[m.from_.getRawIndex()] = Piece::PieceNone;

			// Promotion
			if (m.promotion_ != PieceType::TypeNone) {
				data_[m.to_.getRawIndex()] = makePiece(getPieceColor(data_[m.to_.getRawIndex()]), m.promotion_);
			}

			whiteToPlay_ = !whiteToPlay_;
		}

		std::string getFEN() const {
			std::ostringstream buffer;

			auto writeRow = [&](short row) -> void {
				short emptyRun = 0;
				for (short index = row * 8; index < (row + 1) * 8; index++) {
					assert(0 <= index && index < 64);
					if (data_[index] == Piece::PieceNone) {
						emptyRun++;
					}
					else {
						if (emptyRun > 0) {
							buffer << emptyRun;
							emptyRun = 0;
						}
						buffer << std::format("{}", data_[index]);
					}
				}
				if (emptyRun > 0) {
					buffer << emptyRun;
				}
			};

			writeRow(0);
			buffer << '/';
			writeRow(1);
			buffer << '/';
			writeRow(2);
			buffer << '/';
			writeRow(3);
			buffer << '/';
			writeRow(4);
			buffer << '/';
			writeRow(5);
			buffer << '/';
			writeRow(6);
			buffer << '/';
			writeRow(7);
			buffer << ' ';
			buffer << (whiteToPlay_ ? 'w' : 'b');
			buffer << ' ';
			if (!whiteKingsideCastle_ && !whiteQueensideCastle_ && !blackKingsideCastle_ && !blackQueensideCastle_)
				buffer << '-';
			if (whiteKingsideCastle_)
				buffer << 'K';
			if (whiteQueensideCastle_)
				buffer << 'Q';
			if (blackKingsideCastle_)
				buffer << 'k';
			if (blackQueensideCastle_)
				buffer << 'q';
			buffer << ' ';
			if (enPassantTarget_ == nullSquareIndex)
				buffer << '-';
			else
				buffer << enPassantTarget_.getCode();
			buffer << ' ';
			buffer << halfMoveClock_;
			buffer << ' ';
			buffer << 1;

			// Returning this immediately seems to confuse MSVC...
			std::string result = std::move(buffer).str();
			return result;
		}

	private:
		std::array<Piece, 64> data_;

	public:
		bool whiteToPlay_ = true;
		bool whiteKingsideCastle_ = false;
		bool whiteQueensideCastle_ = false;
		bool blackKingsideCastle_ = false;
		bool blackQueensideCastle_ = false;
		SquareIndex enPassantTarget_ = nullSquareIndex;
		short halfMoveClock_ = 0;
	};

	constexpr Board startingPosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

} // namespace cherry

export template <>
struct ::std::formatter<cherry::Board> : std::formatter<std::string> {
	auto format(cherry::Board const& b, std::format_context& ctx) {
		return std::formatter<std::string>::format(b.getFEN(), ctx);
	}
};

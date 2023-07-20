module;

#include <assert.h>

export module board;

import std;

export namespace cherry {

	class SquareIndex {
	public:
		constexpr SquareIndex(char rank, char file)
			: rawIndex_((7 - file) * 8 + rank) {
			assert(rank >= 0 && rank < 8);
			assert(file >= 0 && file < 8);
		}

		constexpr explicit SquareIndex(char rawIndex)
			: rawIndex_(rawIndex) {
			assert((rawIndex >= 0 && rawIndex < 64) || rawIndex == 127);
		}

		constexpr explicit SquareIndex(std::string_view code)
			: SquareIndex(code[0] - 'a', code[1] - '1') {
			assert(code.size() == 2);
		}

		constexpr bool operator==(const SquareIndex& other) const {
			return rawIndex_ == other.rawIndex_;
		}

		constexpr char getRank() const {
			return 7 - rawIndex_ / 8;
		}

		constexpr char getFile() const {
			return rawIndex_ % 8;
		}

		constexpr char getRawIndex() const {
			return rawIndex_;
		}

		constexpr std::string getCode() const {
			std::array<char, 3> data = { 'a' + getFile(), '1' + getRank(), 0 };
			return std::string(data.data());
		}

	private:
		char rawIndex_;
	};

	constexpr SquareIndex nullSquareIndex(127);

	enum Piece : char {
		None = 0,
		WhitePawn = 8,
		WhiteRook = 9,
		WhiteKnight = 10,
		WhiteBishop = 11,
		WhiteQueen = 12,
		WhiteKing = 13,
		BlackPawn = 16,
		BlackRook = 17,
		BlackKnight = 18,
		BlackBishop = 19,
		BlackQueen = 20,
		BlackKing = 21,
	};

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
						pushSquare(Piece::None);
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

		constexpr Piece at(SquareIndex i) const {
			return data_[i.getRawIndex()];
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

	constexpr bool isWhite(Piece p) {
		return p == WhitePawn
			|| p == WhiteRook
			|| p == WhiteKnight
			|| p == WhiteBishop
			|| p == WhiteQueen
			|| p == WhiteKing;
	}

	constexpr bool isBlack(Piece p) {
		return p == BlackPawn
			|| p == BlackRook
			|| p == BlackKnight
			|| p == BlackBishop
			|| p == BlackQueen
			|| p == BlackKing;
	}

	constexpr bool isPawn(Piece p) {
		return p == WhitePawn || p == BlackPawn;
	}

	constexpr bool isRook(Piece p) {
		return p == WhiteRook || p == BlackRook;
	}

	constexpr bool isKnight(Piece p) {
		return p == WhiteKnight || p == BlackKnight;
	}

	constexpr bool isBishop(Piece p) {
		return p == WhiteBishop || p == BlackBishop;
	}

	constexpr bool isQueen(Piece p) {
		return p == WhiteQueen || p == BlackQueen;
	}

	constexpr bool isKing(Piece p) {
		return p == WhiteKing || p == BlackKing;
	}

} // namespace cherry

export template <>
struct ::std::formatter<cherry::SquareIndex> : std::formatter<std::string> {
	auto format(cherry::SquareIndex i, std::format_context& ctx) {
		return std::formatter<std::string>::format(i.getCode(), ctx);
	}
};

export template <>
struct ::std::formatter<cherry::Piece> : std::formatter<std::string_view> {
	auto format(cherry::Piece i, std::format_context& ctx) {
		std::string_view result = "";
		switch (i) {
		case cherry::Piece::None:
			result = "-";
			break;
		case cherry::WhitePawn:
			result = "P";
			break;
		case cherry::WhiteRook:
			result = "R";
			break;
		case cherry::WhiteKnight:
			result = "N";
			break;
		case cherry::WhiteBishop:
			result = "B";
			break;
		case cherry::WhiteQueen:
			result = "Q";
			break;
		case cherry::WhiteKing:
			result = "K";
			break;
		case cherry::BlackPawn:
			result = "p";
			break;
		case cherry::BlackRook:
			result = "r";
			break;
		case cherry::BlackKnight:
			result = "n";
			break;
		case cherry::BlackBishop:
			result = "b";
			break;
		case cherry::BlackQueen:
			result = "q";
			break;
		case cherry::BlackKing:
			result = "k";
			break;
		}
		return std::formatter<std::string_view>::format(result, ctx);
	}
};

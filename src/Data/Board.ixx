module;

#include <assert.h>

export module cherry.board;

import std;

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

} // namespace cherry

module;

#include <assert.h>

export module board;

import std;

export namespace cherry {

	class SquareIndex {
	public:
		SquareIndex(char rank, char file)
			: rawIndex_(rank * 8 + file) {
			assert(rank >= 0 && rank < 8);
			assert(file >= 0 && file < 8);
		}

		explicit SquareIndex(char rawIndex)
			: rawIndex_(rawIndex) {
			assert(rawIndex >= 0 && rawIndex < 64);
		}

		explicit SquareIndex(std::string_view code)
			: SquareIndex(code[0] - 'a', code[1] - '1') {
			assert(code.size() == 2);
		}

		char getRank() const {
			return rawIndex_ / 8;
		}

		char getFile() const {
			return rawIndex_ % 8;
		}

		std::string getCode() const {
			std::array<char, 3> data = { 'a' + getRank(), '1' + getFile(), 0 };
			return std::string(data.data());
		}

	private:
		char rawIndex_;
	};

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
		// TODO: all of this

	private:
		std::array<Piece, 64> data_;
	};

	bool isWhite(Piece p) {
		return p == WhitePawn
			|| p == WhiteRook
			|| p == WhiteKnight
			|| p == WhiteBishop
			|| p == WhiteQueen
			|| p == WhiteKing;
	}

	bool isBlack(Piece p) {
		return p == BlackPawn
			|| p == BlackRook
			|| p == BlackKnight
			|| p == BlackBishop
			|| p == BlackQueen
			|| p == BlackKing;
	}

	bool isPawn(Piece p) {
		return p == WhitePawn || p == BlackPawn;
	}

	bool isRook(Piece p) {
		return p == WhiteRook || p == BlackRook;
	}

	bool isKnight(Piece p) {
		return p == WhiteKnight || p == BlackKnight;
	}

	bool isBishop(Piece p) {
		return p == WhiteBishop || p == BlackBishop;
	}

	bool isQueen(Piece p) {
		return p == WhiteQueen || p == BlackQueen;
	}

	bool isKing(Piece p) {
		return p == WhiteKing || p == BlackKing;
	}

} // namespace cherry

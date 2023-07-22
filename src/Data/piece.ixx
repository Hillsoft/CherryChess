module;

#include <assert.h>

export module cherry.piece;

import std;

export namespace cherry {

	enum PieceColor : char {
		ColorNone = 0,
		White = 1,
		Black = 2
	};

	enum PieceType : char {
		TypeNone = 0,
		Pawn = 1,
		Rook = 2,
		Knight = 3,
		Bishop = 4,
		Queen = 5,
		King = 6
	};

	enum Piece : char {
		PieceNone = 0,
		WhitePawn = 9,
		WhiteRook = 10,
		WhiteKnight = 11,
		WhiteBishop = 12,
		WhiteQueen = 13,
		WhiteKing = 14,
		BlackPawn = 17,
		BlackRook = 18,
		BlackKnight = 19,
		BlackBishop = 20,
		BlackQueen = 21,
		BlackKing = 22,
	};

	constexpr PieceColor getPieceColor(Piece p) {
		return (PieceColor)((char)p >> 3);

		// Verbose implementation, if you don't trust the bit-hacking
		if (p == WhitePawn
			|| p == WhiteRook
			|| p == WhiteKnight
			|| p == WhiteBishop
			|| p == WhiteQueen
			|| p == WhiteKing) {
			return PieceColor::White;
		}
		if (p == BlackPawn
			|| p == BlackRook
			|| p == BlackKnight
			|| p == BlackBishop
			|| p == BlackQueen
			|| p == BlackKing) {
			return PieceColor::Black;
		}
		return PieceColor::ColorNone;
	}

	constexpr PieceType getPieceType(Piece p) {
		return (PieceType)((char)p & 7);

		// Verbose implementation, if you don't trust the bit-hacking
		if (p == WhitePawn || p == BlackPawn) {
			return PieceType::Pawn;
		}
		if (p == WhiteRook || p == BlackRook) {
			return PieceType::Rook;
		}
		if (p == WhiteKnight || p == BlackKnight) {
			return PieceType::Knight;
		}
		if (p == WhiteBishop || p == BlackBishop) {
			return PieceType::Bishop;
		}
		if (p == WhiteQueen || p == BlackQueen) {
			return PieceType::Queen;
		}
		if (p == WhiteKing || p == BlackKing) {
			return PieceType::King;
		}
		return PieceType::TypeNone;
	}

	constexpr std::string getPieceCode(PieceType t) {
		switch (t) {
		case PieceType::Pawn:
			return "p";
		case PieceType::Rook:
			return "r";
		case PieceType::Knight:
			return "n";
		case PieceType::Bishop:
			return "b";
		case PieceType::Queen:
			return "q";
		case PieceType::King:
			return "k";
		case PieceType::TypeNone:
			return "";
		}
		return "";
	}

	constexpr PieceType fromPieceCode(std::string_view c) {
		assert(c.size() == 0 || c.size() == 1);
		if (c.size() == 0) {
			return PieceType::TypeNone;
		}
		switch (c.at(0)) {
		case 'p':
			return PieceType::Pawn;
		case 'r':
			return PieceType::Rook;
		case 'n':
			return PieceType::Knight;
		case 'b':
			return PieceType::Bishop;
		case 'q':
			return PieceType::Queen;
		case 'k':
			return PieceType::King;
		}
		return PieceType::TypeNone;
	}

} // namespace cherry

export template <>
struct ::std::formatter<cherry::Piece> : std::formatter<std::string_view> {
	auto format(cherry::Piece i, std::format_context& ctx) {
		std::string_view result = "";
		switch (i) {
		case cherry::Piece::PieceNone:
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

export template <>
struct ::std::formatter<cherry::PieceType> : std::formatter<std::string_view> {
	auto format(cherry::PieceType i, std::format_context& ctx) {
		std::string_view result = "";
		switch (i) {
		case cherry::PieceType::TypeNone:
			result = "-";
			break;
		case cherry::Pawn:
			result = "Pawn";
			break;
		case cherry::Rook:
			result = "Rook";
			break;
		case cherry::Knight:
			result = "Knight";
			break;
		case cherry::Bishop:
			result = "Bishop";
			break;
		case cherry::Queen:
			result = "Queen";
			break;
		case cherry::King:
			result = "King";
			break;
		}
		return std::formatter<std::string_view>::format(result, ctx);
	}
};

export template <>
struct ::std::formatter<cherry::PieceColor> : std::formatter<std::string_view> {
	auto format(cherry::PieceColor i, std::format_context& ctx) {
		std::string_view result = "";
		switch (i) {
		case cherry::PieceColor::ColorNone:
			result = "-";
			break;
		case cherry::PieceColor::White:
			result = "White";
			break;
		case cherry::PieceColor::Black:
			result = "Black";
			break;
		}
		return std::formatter<std::string_view>::format(result, ctx);
	}
};

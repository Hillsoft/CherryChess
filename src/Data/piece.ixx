export module cherry.piece;

import std;

export namespace cherry {

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

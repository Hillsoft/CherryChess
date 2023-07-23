module;

#include <assert.h>

export module cherry.move;

import cherry.piece;
import cherry.squareIndex;

export namespace cherry {
	
	class Move {
	public:
		constexpr Move()
			: from_(nullSquareIndex), to_(nullSquareIndex), promotion_(PieceType::TypeNone) {}

		constexpr Move(SquareIndex from, SquareIndex to, PieceType promotion = PieceType::TypeNone)
			: from_(from), to_(to), promotion_(promotion) {}

		constexpr explicit Move(std::string_view code)
			: from_(code.substr(0, 2)), to_(code.substr(2, 2)), promotion_(fromPieceCode(code.substr(4))) {
			assert(code.size() == 4 || code.size() == 5);
		}

		std::string getCode() const {
			if (from_ == nullSquareIndex || to_ == nullSquareIndex) {
				return "0000";
			}
			return std::format("{}{}{}", from_.getCode(), to_.getCode(), getPieceCode(promotion_));
		}

		constexpr auto operator<=>(Move const& other) const = default;

		SquareIndex from_;
		SquareIndex to_;
		PieceType promotion_;
	};

} // namsepace cherry

export template <>
struct ::std::formatter<cherry::Move> : std::formatter<std::string> {
	auto format(cherry::Move m, std::format_context& ctx) {
		return std::formatter<std::string>::format(m.getCode(), ctx);
	}
};

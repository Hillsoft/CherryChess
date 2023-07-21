export module cherry.move;

import cherry.piece;
import cherry.squareIndex;

export namespace cherry {
	
	class Move {
	public:
		constexpr Move(SquareIndex from, SquareIndex to, PieceType promotion = PieceType::TypeNone)
			: from_(from), to_(to), promotion_(promotion) {}

		std::string getCode() const {
			return std::format("{}{}{}", from_.getCode(), to_.getCode(), getPieceCode(promotion_));
		}

		constexpr auto operator<=>(Move const& other) const = default;

	private:
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

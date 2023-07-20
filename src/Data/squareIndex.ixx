module;

#include <assert.h>

export module cherry.squareIndex;

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

} // namespace cherry

export template <>
struct ::std::formatter<cherry::SquareIndex> : std::formatter<std::string> {
	auto format(cherry::SquareIndex i, std::format_context& ctx) {
		return std::formatter<std::string>::format(i.getCode(), ctx);
	}
};

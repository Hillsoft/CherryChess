module;

#include <cassert>

export module cherry.transpositionTable;

import std;

import cherry.board;
import cherry.evaluation;
import cherry.searchResult;

constexpr size_t kTableSize = 4194304;

namespace cherry {

	export class TranspositionTable {
	public:
		std::optional<SearchResult> lookup(Board const& board) {
			size_t index = board.hash() % kTableSize;
			if (table_[index].first.transpositionEq(board)) {
				return table_[index].second;
			}
			return std::nullopt;
		}

		SearchResult insert(Board const& board, SearchResult result) {
			size_t index = board.hash() % kTableSize;
			if (table_[index].first.transpositionEq(board)) {
				if (result.depth_ > table_[index].second.depth_) {
					table_[index].second = result;
				}
				else if (result.depth_ == table_[index].second.depth_) {
					table_[index].second.eval_.join(result.eval_);
				}
				else {
					return result;
				}
			}
			else {
				table_[index] = { board, result };
			}
			return table_[index].second;
		}

	private:
		std::array<std::pair<Board, SearchResult>, kTableSize> table_;
	};

	TranspositionTable globalTable;

	export TranspositionTable& getGlobalTranspositionTable() {
		return globalTable;
	}

} // namespace

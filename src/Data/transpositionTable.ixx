module;

#include <cassert>

export module cherry.transpositionTable;

import std;

import cherry.board;
import cherry.evaluation;
import cherry.searchResult;

constexpr size_t kTableSize = 1048576;
constexpr size_t kTableCellSize = 4;

namespace cherry {

	class TranspositionTableEntry {
	public:
		constexpr TranspositionTableEntry() {}
		constexpr TranspositionTableEntry(Board const& board, size_t hash, SearchResult result)
			: board_(board), hash_(hash), result_(result) {
		}

		constexpr TranspositionTableEntry(TranspositionTableEntry const& other) = default;
		constexpr TranspositionTableEntry(TranspositionTableEntry&& other) = default;

		constexpr TranspositionTableEntry& operator=(TranspositionTableEntry const& other) = default;
		constexpr TranspositionTableEntry& operator=(TranspositionTableEntry&& other) = default;

		constexpr bool isHit(Board const& board, size_t hash) {
			return hash_ == hash && board_.transpositionEq(board);
		}

		Board board_;
		size_t hash_;
		SearchResult result_;
	};

	class TranspositionTableCell {
	public:
		constexpr TranspositionTableCell() {
			for (size_t i = 0; i < kTableCellSize; i++) {
				lruCache_[i] = i;
			}
		}

		constexpr std::optional<SearchResult> find(Board const& board, size_t hash) {
			for (size_t i = 0; i < kTableCellSize; i++) {
				if (entries_[i].isHit(board, hash)) {
					promoteHashValue(i);
					return entries_[i].result_;
				}
			}
			return std::nullopt;
		}

		constexpr std::optional<SearchResult*> findPtr(Board const& board, size_t hash) {
			for (size_t i = 0; i < kTableCellSize; i++) {
				if (entries_[i].isHit(board, hash)) {
					promoteHashValue(i);
					return &entries_[i].result_;
				}
			}
			return std::nullopt;
		}

		constexpr void write(Board const& board, size_t hash, SearchResult result) {
			size_t index = lruCache_[kTableCellSize - 1];
			entries_[index] = TranspositionTableEntry{ board, hash, result };
			promoteHashIndex(kTableCellSize - 1);
		}

		constexpr void promoteHashIndex(size_t index) {
			size_t value = lruCache_[index];
			for (size_t i = index; i > 0; i--) {
				lruCache_[i] = lruCache_[i - 1];
			}
			lruCache_[0] = value;
		}

		constexpr void promoteHashValue(size_t value) {
			size_t index = kTableCellSize - 1;
			while (lruCache_[index] != value) {
				index--;
			}
			while (index > 0) {
				lruCache_[index] = lruCache_[index - 1];
				index--;
			}
			lruCache_[0] = value;
		}

	private:
		std::array<size_t, kTableCellSize> lruCache_;
		std::array<TranspositionTableEntry, kTableCellSize> entries_;
	};

	export class TranspositionTable {
	public:
		std::optional<SearchResult> lookup(Board const& board) {
			size_t hash = board.hash();
			size_t index = hash % kTableSize;
			return table_[index].find(board, hash);
		}

		SearchResult insert(Board const& board, SearchResult result) {
			size_t hash = board.hash();
			size_t index = hash % kTableSize;
			std::optional<SearchResult*> storedResult = table_[index].findPtr(board, hash);
			if (storedResult.has_value()) {
				if (result.depth_ > (*storedResult)->depth_) {
					**storedResult = result;
					return result;
				}
				else if (result.depth_ == (*storedResult)->depth_) {
					(*storedResult)->eval_.join(result.eval_);
					return **storedResult;
				}
				else {
					return result;
				}
			}
			else {
				table_[index].write(board, hash, result);
				return result;
			}
		}

	private:
		std::array<TranspositionTableCell, kTableSize> table_;
	};

	TranspositionTable globalTable;

	export TranspositionTable& getGlobalTranspositionTable() {
		return globalTable;
	}

} // namespace

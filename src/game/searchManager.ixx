export module cherry.searchManager;

import std;

import cherry.board;
import cherry.move;
import cherry.moveEnumeration;

export namespace cherry {

	class SearchManager {
	public:
		SearchManager()
			: currentPosition_(startingPosition) {}

		void setPosition(Board position) {
			currentPosition_ = std::move(position);
		}

		Move stopSearch() {
			std::vector<Move> possibleMoves = availableMoves(currentPosition_);
			return possibleMoves[std::rand() % possibleMoves.size()];
		}

	private:
		Board currentPosition_;
	};

	SearchManager& getGlobalSearchManager() {
		static SearchManager search = SearchManager();
		return search;
	}

} // namespace cherry

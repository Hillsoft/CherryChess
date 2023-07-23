export module cherry.searchManager;

import std;

import cherry.board;
import cherry.move;
import cherry.moveEnumeration;
import cherry.positionEval;

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
			Move bestMove;
			int bestEval = -1000000;
			for (auto const& move : possibleMoves) {
				Board resultingPosition = currentPosition_;
				resultingPosition.makeMove(move);
				int curEval = evaluatePosition(resultingPosition) * (currentPosition_.whiteToPlay_ ? 1 : -1);
				if (curEval > bestEval) {
					bestEval = curEval;
					bestMove = move;
				}
			}
			return bestMove;
		}

	private:
		Board currentPosition_;
	};

	SearchManager& getGlobalSearchManager() {
		static SearchManager search = SearchManager();
		return search;
	}

} // namespace cherry

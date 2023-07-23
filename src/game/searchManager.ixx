export module cherry.searchManager;

import std;

import cherry.board;
import cherry.move;
import cherry.moveEnumeration;
import cherry.positionEval;
import cherry.uci.commandEmitter;
import cherry.uci.cmd.info;

export namespace cherry {

	std::pair<int, Move> recursiveSearch(Board const& rootPosition, int depth) {
		if (depth <= 0) {
			return std::pair(evaluatePosition(rootPosition) * (rootPosition.whiteToPlay_ ? 1 : -1), Move());
		}

		std::vector<Move> possibleMoves = availableMoves(rootPosition);
		std::pair<int, Move> bestResult(-100000, Move());

		for (auto const& move : possibleMoves) {
			Board resultingPosition = rootPosition;
			resultingPosition.makeMove(move);
			int currentEval = recursiveSearch(resultingPosition, depth - 1).first * -1;
			if (currentEval > bestResult.first) {
				bestResult = std::pair(currentEval, move);
			}
		}

		return bestResult;
	}

	class SearchManager {
	public:
		SearchManager()
			: currentPosition_(startingPosition) {}

		void setPosition(Board position) {
			currentPosition_ = std::move(position);
		}

		Move stopSearch(uci::CommandEmitter* emitter) {
			auto [eval, move] = recursiveSearch(currentPosition_, 5);
			if (emitter != nullptr) {
				emitter->emitCommand(uci::command::UCIInfo(eval));
			}
			return move;
		}

	private:
		Board currentPosition_;
	};

	SearchManager& getGlobalSearchManager() {
		static SearchManager search = SearchManager();
		return search;
	}

} // namespace cherry

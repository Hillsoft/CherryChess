export module cherry.searchManager;

import std;

import cherry.board;
import cherry.evaluation;
import cherry.move;
import cherry.moveEnumeration;
import cherry.positionEval;
import cherry.uci.commandEmitter;
import cherry.uci.cmd.info;

export namespace cherry {

	std::pair<Evaluation, Move> recursiveSearch(Board const& rootPosition, Evaluation alpha, Evaluation beta, int depth) {
		if (depth <= 0) {
			return std::pair(evaluatePosition(rootPosition), Move());
		}

		std::vector<Move> possibleMoves = availableMoves(rootPosition);

		std::pair<Evaluation, Move> bestResult(worstEval, Move());
		bool hasLegalMove = false;

		for (auto const& move : possibleMoves) {
			Board resultingPosition = rootPosition;
			resultingPosition.makeMove(move);
			Evaluation currentEval = recursiveSearch(resultingPosition, unstep(beta), unstep(alpha), depth - 1).first;
			currentEval.step();

			if (currentEval > beta && !isIllegalDueToCheck(resultingPosition)) {
				hasLegalMove = true;
				bestResult = std::pair(currentEval, move);
				break;
			}

			if (currentEval > bestResult.first && !isIllegalDueToCheck(resultingPosition)) {
				hasLegalMove = true;
				bestResult = std::pair(currentEval, move);
				if (currentEval > alpha) {
					alpha = currentEval;
				}
			}
		}

		if (!hasLegalMove) {
			return std::pair(terminalEval(rootPosition), Move());
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
			auto [eval, move] = recursiveSearch(currentPosition_, worstEval, bestEval, 6);
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

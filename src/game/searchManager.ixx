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

	std::pair<Evaluation , Move > recursiveSearch(Board const& rootPosition, int depth) {
		if (depth <= 0) {
			return std::pair(evaluatePosition(rootPosition), Move());
		}

		std::vector<Move> possibleMoves = availableMoves(rootPosition);

		std::pair<Evaluation, Move> bestResult(Evaluation(Evaluation::CPTag(), 0), Move());
		bool hasLegalMove = false;

		auto cursor = possibleMoves.begin();
		for (; cursor != possibleMoves.end() && !hasLegalMove; cursor++) {
			Move const& move = *cursor;
			Board resultingPosition = rootPosition;
			resultingPosition.makeMove(move);
			if (!isIllegalDueToCheck(resultingPosition)) {
				Evaluation currentEval = recursiveSearch(resultingPosition, depth - 1).first;
				currentEval.step();
				bestResult = std::pair(currentEval, move);
				hasLegalMove = true;
			}
		}

		if (!hasLegalMove) {
			return std::pair(terminalEval(rootPosition), Move());
		}

		for (; cursor != possibleMoves.end(); cursor++) {
			Move const& move = *cursor;
			Board resultingPosition = rootPosition;
			resultingPosition.makeMove(move);
			Evaluation currentEval = recursiveSearch(resultingPosition, depth - 1).first;
			currentEval.step();
			if (currentEval > bestResult.first && !isIllegalDueToCheck(resultingPosition)) {
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
			auto [eval, move] = recursiveSearch(currentPosition_, 6);
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

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

	std::tuple<Evaluation, Move, int> recursiveSearch(Board const& rootPosition, Evaluation alpha, Evaluation beta, int maxDepth, int maxExtensionDepth) {
		int nodes = 1;
		if (maxExtensionDepth <= 0) {
			return std::tuple(evaluatePosition(rootPosition), Move(), nodes);
		}

		MoveEnumerationResult possibleMoves = availableMoves(rootPosition);

		std::pair<Evaluation, Move> bestResult(worstEval, Move());
		bool hasLegalMove = false;

		if (maxDepth <= 0) {
			// trim out non-extending moves
			possibleMoves.others = {};
			hasLegalMove = true;
			bestResult = std::pair(evaluatePosition(rootPosition), Move());
			if (bestResult.first > alpha) {
				alpha = bestResult.first;
			}
		}

		auto result = [&]() {
			if (!hasLegalMove) {
				return std::tuple(terminalEval(rootPosition), Move(), nodes);
			}

			return std::tuple(bestResult.first, bestResult.second, nodes);
		};

		auto searchMove = [&](Move move) -> bool {
			Board resultingPosition = rootPosition;
			resultingPosition.makeMove(move);
			auto [currentEval, _, nodesInc] = recursiveSearch(resultingPosition, unstep(beta), unstep(alpha), maxDepth - 1, maxExtensionDepth - 1);
			nodes += nodesInc;
			currentEval.step();

			if (currentEval > beta) {
				hasLegalMove = true;
				bestResult = std::pair(currentEval, move);
				return false;
			}

			if (currentEval > bestResult.first) {
				hasLegalMove = true;
				bestResult = std::pair(currentEval, move);
				if (currentEval > alpha) {
					alpha = currentEval;
				}
			}
			return true;
		};

		for (auto const& move : possibleMoves.checksAndCaptures) {
			bool shouldContinue = searchMove(move);
			if (!shouldContinue) {
				return result();
			}
		}
		for (auto const& move : possibleMoves.checks) {
			bool shouldContinue = searchMove(move);
			if (!shouldContinue) {
				return result();
			}
		}
		for (auto const& move : possibleMoves.captures) {
			bool shouldContinue = searchMove(move);
			if (!shouldContinue) {
				return result();
			}
		}
		for (auto const& move : possibleMoves.others) {
			bool shouldContinue = searchMove(move);
			if (!shouldContinue) {
				return result();
			}
		}

		return result();
	}

	class SearchManager {
	public:
		SearchManager()
			: currentPosition_(startingPosition) {}

		void setPosition(Board position) {
			currentPosition_ = std::move(position);
		}

		Move stopSearch(uci::CommandEmitter* emitter) {
			const auto start = std::chrono::steady_clock::now();
			auto [eval, move, nodes] = recursiveSearch(currentPosition_, worstEval, bestEval, 4, 12);
			if (emitter != nullptr) {
				const auto elapsed = std::chrono::steady_clock::now() - start;
				emitter->emitCommand(uci::command::UCIInfo({
					.timeMs = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count(),
					.nodes = nodes,
					.score = eval }));
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

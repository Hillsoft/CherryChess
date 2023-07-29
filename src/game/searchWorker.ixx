export module cherry.searchWorker;

import std;

import cherry.board;
import cherry.evaluation;
import cherry.move;
import cherry.moveEnumeration;
import cherry.positionEval;

export namespace cherry {

	class SearchWorker {
	public:
		std::tuple<Evaluation, Move> recursiveSearch(Board const& rootPosition, Evaluation alpha, Evaluation beta, int maxDepth, int maxExtensionDepth) {
			nodesVisited_.store(nodesVisited_.load(std::memory_order_relaxed) + 1, std::memory_order_relaxed);
			if (maxExtensionDepth <= 0) {
				return std::tuple(evaluatePosition(rootPosition), Move());
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
					return std::tuple(terminalEval(rootPosition), Move());
				}

				return std::tuple(bestResult.first, bestResult.second);
			};

			auto searchMove = [&](Move move) -> bool {
				Board resultingPosition = rootPosition;
				resultingPosition.makeMove(move);
				auto [currentEval, _] = recursiveSearch(resultingPosition, unstep(beta), unstep(alpha), maxDepth - 1, maxExtensionDepth - 1);
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

		std::atomic<int> nodesVisited_;
	};

} // namsepace cherry

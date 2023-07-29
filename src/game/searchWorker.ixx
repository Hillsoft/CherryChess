export module cherry.searchWorker;

import std;

import cherry.board;
import cherry.evaluation;
import cherry.inlineStack;
import cherry.move;
import cherry.moveEnumeration;
import cherry.positionEval;

export namespace cherry {

	class SearchWorker {
	public:
		void reset() {
			bestMove_ = Move();
			eval_ = Evaluation(Evaluation::CPTag(), 0);
			nodesVisited_ = 0;
			depth_ = 0;
			complete_ = false;
			shouldStop_ = false;
		}

		template <size_t historySize>
		void iterativeDeepening(Board const& rootPosition, InlineStack<Board, historySize>& history) {
			int maxDepth = 0;
			while (!shouldStop_.load(std::memory_order_relaxed) && maxDepth < 50 && !eval_.load(std::memory_order_relaxed).isMate_) {
				maxDepth += 1;
				auto [eval, move] = recursiveSearch(rootPosition, history, worstEval, bestEval, maxDepth, maxDepth + 2, false);
				// We don't trust partial results...
				if (!shouldStop_.load(std::memory_order_relaxed)) {
					eval_.store(eval, std::memory_order_relaxed);
					bestMove_.store(move, std::memory_order_relaxed);
					depth_.store(maxDepth, std::memory_order_relaxed);
				}
			}
			complete_.store(true, std::memory_order_release);
		}

		template <size_t historySize>
		std::tuple<Evaluation, Move> recursiveSearch(Board const& rootPosition, InlineStack<Board, historySize>& history, Evaluation alpha, Evaluation beta, int maxDepth, int maxExtensionDepth, bool topLevel = true) {
			if (shouldStop_.load(std::memory_order_relaxed)) {
				return std::tuple(worstEval, Move());
			}

			nodesVisited_.store(nodesVisited_.load(std::memory_order_relaxed) + 1, std::memory_order_relaxed);

			// Check for three-fold repetition
			char repetitionCount = 0;
			for (auto& position : history) {
				if (position.data_ == rootPosition.data_) {
					repetitionCount++;
				}
			}
			if (repetitionCount >= 2) {
				return std::tuple(Evaluation(Evaluation::CPTag(), 0), Move());
			}

			InlineStackWriter(history, rootPosition);

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
				auto [currentEval, _] = recursiveSearch(resultingPosition, history, unstep(beta), unstep(alpha), maxDepth - 1, maxExtensionDepth - 1, false);
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
					if (topLevel) {
						eval_.store(currentEval, std::memory_order_relaxed);
						bestMove_.store(move, std::memory_order_relaxed);
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

			if (topLevel) {
				complete_.store(true, std::memory_order_release);
			}

			return result();
		}
		
		std::atomic<Move> bestMove_;
		std::atomic<Evaluation> eval_;
		std::atomic<int> nodesVisited_;
		std::atomic<int> depth_;
		std::atomic<bool> complete_;
		std::atomic<bool> shouldStop_;
	};

} // namsepace cherry

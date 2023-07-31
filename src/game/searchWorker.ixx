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
		void iterativeDeepening(Board const& rootPosition, InlineStack<Board, historySize>& history, Evaluation baseAlpha = worstEval, Evaluation baseBeta = bestEval, int maxDepthLimit = 50) {
			InlineStackWriter(history, rootPosition);
			std::vector<std::pair<Move, Evaluation>> topLevelMoves;

			{
				MoveEnumerationResult enumeration = availableMoves(rootPosition);
				topLevelMoves.reserve(enumeration.others.size());

				for (auto const& move : enumeration.others) {
					topLevelMoves.emplace_back(move, Evaluation());
				}
			}

			int maxDepth = 0;
			while (!shouldStop_.load(std::memory_order_relaxed) && maxDepth < maxDepthLimit && !eval_.load(std::memory_order_relaxed).isMate_) {
				maxDepth += 1;

				Evaluation alpha = baseAlpha;

				for (auto& [move, eval] : topLevelMoves) {
					Board newPosition = rootPosition;
					newPosition.makeMove(move);
					eval = step(std::get<0>(recursiveSearch(newPosition, history, unstep(baseBeta), unstep(alpha), maxDepth - 1, maxDepth + 1 - 1, false)));
					if (eval > alpha) {
						alpha = eval;
					}
				}
				std::sort(topLevelMoves.begin(), topLevelMoves.end(), [](auto& a, auto& b) { return a.second > b.second; });
				// We don't trust partial results...
				if (!shouldStop_.load(std::memory_order_relaxed)) {
					eval_.store(topLevelMoves[0].second, std::memory_order_relaxed);
					bestMove_.store(topLevelMoves[0].first, std::memory_order_relaxed);
					depth_.store(maxDepth, std::memory_order_relaxed);
				}
			}
			complete_.store(true, std::memory_order_release);
		}

		template <size_t historySize>
		std::pair<Evaluation, Move> recursiveSearch(Board const& rootPosition, InlineStack<Board, historySize>& history, Evaluation baseAlpha, Evaluation baseBeta, int maxDepth, int maxExtensionDepth, bool topLevel = true) {
			if (shouldStop_.load(std::memory_order_relaxed)) {
				return std::pair(worstEval, Move());
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
				return std::pair(Evaluation(Evaluation::CPTag(), 0), Move());
			}

			InlineStackWriter(history, rootPosition);

			if (maxExtensionDepth <= 0) {
				return std::pair(evaluatePosition(rootPosition), Move());
			}


			std::vector<std::pair<Evaluation, Move>> possibleMoves;
			{
				MoveEnumerationResult enumeration = availableMoves(rootPosition);

				if (maxDepth <= 0) {
					Evaluation currentEval = evaluatePosition(rootPosition);
					if (currentEval > baseAlpha) {
						baseAlpha = currentEval;
					}
					if (currentEval > baseBeta) {
						return std::pair(evaluatePosition(rootPosition), Move());
					}
				}

				possibleMoves.reserve(enumeration.others.size());

				for (auto const& move : enumeration.others) {
					possibleMoves.emplace_back(Evaluation(), move);
				}
			}

			if (possibleMoves.size() == 0) {
				if (maxDepth <= 0) {
					return std::pair(evaluatePosition(rootPosition), Move());
				}
				return std::tuple(terminalEval(rootPosition), Move());
			}

			for (int subDepth = 0; subDepth <= maxDepth - 1; subDepth++) {
				Evaluation alpha = baseAlpha;
				for (auto & [eval, move] : possibleMoves) {
					Board resultingPosition = rootPosition;
					resultingPosition.makeMove(move);
					auto [currentEval, _] = recursiveSearch(resultingPosition, history, unstep(baseBeta), unstep(alpha), std::min(subDepth, maxDepth - 1), subDepth, false);
					currentEval.step();
					eval = currentEval;

					if (currentEval > baseBeta) {
						break;
					}
					if (currentEval > alpha) {
						alpha = currentEval;
					}
				}
				std::sort(possibleMoves.begin(), possibleMoves.end(), [](auto& a, auto& b) { return a.first > b.first; });
				if (subDepth == 0 && maxDepth <= 0) {
					// We are in search extension, only keep the most interesting moves
					possibleMoves.resize(std::min<size_t>(5, possibleMoves.size()));
				}
			}
			return possibleMoves[0];
		}
		
		std::atomic<Move> bestMove_;
		std::atomic<Evaluation> eval_;
		std::atomic<int> nodesVisited_;
		std::atomic<int> depth_;
		std::atomic<bool> complete_;
		std::atomic<bool> shouldStop_;
	};

} // namsepace cherry

export module cherry.searchWorker;

import std;

import cherry.board;
import cherry.evaluation;
import cherry.evaluationRange;
import cherry.inlineStack;
import cherry.move;
import cherry.moveEnumeration;
import cherry.positionEval;
import cherry.searchResult;
import cherry.transpositionTable;

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
			while (!shouldStop_.load(std::memory_order_seq_cst) && maxDepth < maxDepthLimit && !eval_.load(std::memory_order_relaxed).isMate_) {
				maxDepth += 1;

				Evaluation alpha = baseAlpha;

				for (auto& [move, eval] : topLevelMoves) {
					Board newPosition = rootPosition;
					newPosition.makeMove(move);

					SearchResult result = recursiveSearch(newPosition, history, unstep(baseBeta), unstep(alpha), maxDepth - 1, maxDepth + 2 - 1, false);
					eval = step(result.eval_.low_);
					if (eval > alpha) {
						alpha = eval;
					}
				}
				std::sort(topLevelMoves.begin(), topLevelMoves.end(), [](auto& a, auto& b) { return a.second > b.second; });
				// We don't trust partial results...
				if (!shouldStop_.load(std::memory_order_seq_cst)) {
					eval_.store(topLevelMoves[0].second, std::memory_order_relaxed);
					bestMove_.store(topLevelMoves[0].first, std::memory_order_relaxed);
					depth_.store(maxDepth, std::memory_order_relaxed);
				}
			}
			complete_.store(true, std::memory_order_release);
		}

		template <size_t historySize>
		SearchResult recursiveSearch(Board const& rootPosition, InlineStack<Board, historySize>& history, Evaluation baseAlpha, Evaluation baseBeta, int maxDepth, int maxExtensionDepth, bool topLevel = true) {
			if (shouldStop_.load(std::memory_order_seq_cst)) {
				return SearchResult(0, EvaluationRange(worstEval), Move());
			}

			nodesVisited_.store(nodesVisited_.load(std::memory_order_relaxed) + 1, std::memory_order_relaxed);

			std::optional<SearchResult> m_result = getGlobalTranspositionTable().lookup(rootPosition);
			if (m_result.has_value() && m_result->depth_ >= maxExtensionDepth) {
				if (m_result->eval_.low_ >= baseBeta || m_result->eval_.low_ == m_result->eval_.high_) {
					return *m_result;
				}
			}

			// Check for three-fold repetition
			char repetitionCount = 0;
			for (auto& position : history) {
				if (position.data_ == rootPosition.data_) {
					repetitionCount++;
				}
			}
			if (repetitionCount >= 1) {
				return SearchResult(0, EvaluationRange(Evaluation(Evaluation::CPTag(), 0)), Move());
			}

			InlineStackWriter(history, rootPosition);

			if (maxExtensionDepth <= 0) {
				SearchResult result(0, EvaluationRange(evaluatePosition(rootPosition)), Move());
				return getGlobalTranspositionTable().insert(rootPosition, result);
			}

			std::vector<std::pair<EvaluationRange, Move>> possibleMoves;
			{
				MoveEnumerationResult enumeration = availableMoves(rootPosition);


				possibleMoves.reserve(enumeration.others.size());

				for (auto const& move : enumeration.others) {
					possibleMoves.emplace_back(EvaluationRange(), move);
				}
			}

			if (possibleMoves.size() == 0) {
				SearchResult result(9999, EvaluationRange(terminalEval(rootPosition)), {});
				return getGlobalTranspositionTable().insert(rootPosition, result);
			}

			for (int subDepth = 0; subDepth <= maxExtensionDepth - 1; subDepth++) {
				Evaluation alpha = baseAlpha;
				bool prune = false;
				for (auto & [eval, move] : possibleMoves) {
					if (!prune) {
						Board resultingPosition = rootPosition;
						resultingPosition.makeMove(move);
						SearchResult recursiveResult = recursiveSearch(resultingPosition, history, unstep(baseBeta), unstep(alpha), subDepth - (maxExtensionDepth - maxDepth), subDepth, false);
						recursiveResult.eval_.step();
						eval = recursiveResult.eval_;

						if (subDepth > 0 && eval.low_ >= baseBeta) {
							prune = true;
						}
						if (eval.low_ > alpha) {
							alpha = eval.low_;
						}
					}
					else {
						eval = EvaluationRange();
					}
				}
				std::sort(possibleMoves.begin(), possibleMoves.end(), [](auto& a, auto& b) { return a.first.low_ > b.first.low_; });
				if (subDepth == 0 && maxDepth <= 0) {
					// We are in search extension, only keep the most interesting moves
					possibleMoves.resize(std::min<size_t>(3, possibleMoves.size()));
				}
			}
			std::optional<SearchResult> result;
			if (maxDepth <= 0) {
				SearchResult stopResult(maxExtensionDepth, EvaluationRange(evaluatePosition(rootPosition)), {});
				if (stopResult.eval_.low_ > possibleMoves[0].first.low_) {
					result = stopResult;
				}
			}
			if (!result.has_value()) {
				EvaluationRange evalRange = possibleMoves[0].first;
				if (evalRange.low_ >= baseBeta) {
					// We exited early due to pruning so do not have a precise eval
					evalRange.high_ = bestEval;
				}
				result = SearchResult(maxExtensionDepth, evalRange, possibleMoves[0].second);
			}
			for (auto& [eval, _] : possibleMoves) {
				if (eval.high_ > result->eval_.high_) {
					result->eval_.high_ = eval.high_;
				}
			}

			if (shouldStop_.load(std::memory_order_seq_cst)) {
				return *result;
			}
			else {
				return getGlobalTranspositionTable().insert(rootPosition, *result);
			}
		}
		
		std::atomic<Move> bestMove_;
		std::atomic<Evaluation> eval_;
		std::atomic<int> nodesVisited_;
		std::atomic<int> depth_;
		std::atomic<bool> complete_;
		std::atomic<bool> shouldStop_;
	};

} // namsepace cherry

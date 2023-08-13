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
			std::vector<std::tuple<Move, Evaluation, std::vector<Move>>> topLevelMoves;

			{
				MoveEnumerationResult enumeration = availableMoves(rootPosition);
				topLevelMoves.reserve(enumeration.others.size());

				for (auto const& move : enumeration.others) {
					topLevelMoves.emplace_back(move, Evaluation(), std::vector<Move>{});
				}
			}

			int maxDepth = 0;
			while (!shouldStop_.load(std::memory_order_seq_cst) && maxDepth < maxDepthLimit && !eval_.load(std::memory_order_relaxed).isMate_) {
				maxDepth += 1;

				Evaluation alpha = baseAlpha;

				for (auto& [move, eval, line] : topLevelMoves) {
					Board newPosition = rootPosition;
					newPosition.makeMove(move);

					SearchResult result = recursiveSearch(newPosition, history, unstep(baseBeta), unstep(alpha), maxDepth - 1, maxDepth + 2 - 1, false);
					eval = step(result.eval_.value_);
					if (eval > alpha) {
						alpha = eval;
					}
					line = std::move(result.line_);
					line.push_back(move);
				}
				std::sort(topLevelMoves.begin(), topLevelMoves.end(), [](auto& a, auto& b) { return std::get<1>(a) > std::get<1>(b); });
				// We don't trust partial results...
				if (!shouldStop_.load(std::memory_order_seq_cst)) {
					eval_.store(std::get<1>(topLevelMoves[0]), std::memory_order_relaxed);
					bestMove_.store(std::get<0>(topLevelMoves[0]), std::memory_order_relaxed);
					depth_.store(maxDepth, std::memory_order_relaxed);
					std::cout << "info pv ";
					auto const& line = std::get<2>(topLevelMoves[0]);
					for (size_t i = line.size() - 1; i < line.size() && i >= 0; i--) {
						std::cout << std::format("{}", line[i]) << " ";
					}
					std::cout << std::get<1>(topLevelMoves[0]);
					std::cout << " depth " << depth_;
					std::cout << std::endl;
				}
			}
			complete_.store(true, std::memory_order_release);
		}

		template <size_t historySize>
		SearchResult recursiveSearch(Board const& rootPosition, InlineStack<Board, historySize>& history, Evaluation baseAlpha, Evaluation baseBeta, int maxDepth, int maxExtensionDepth, bool topLevel = true) {
			if (shouldStop_.load(std::memory_order_seq_cst)) {
				return SearchResult(0, EvaluationRange(worstEval, EvalLowerBound), Move(), {});
			}

			nodesVisited_.store(nodesVisited_.load(std::memory_order_relaxed) + 1, std::memory_order_relaxed);

			std::optional<SearchResult> m_result = getGlobalTranspositionTable().lookup(rootPosition);
			if (m_result.has_value() && m_result->depth_ == maxExtensionDepth) {
				// if (m_result->eval_.low_ >= baseBeta || m_result->eval_.low_ == m_result->eval_.high_) {
				if (m_result->eval_.type_ == EvalExact) {
					return *m_result;
				}
				if (m_result->eval_.type_ == EvalLowerBound && m_result->eval_.value_ >= baseBeta) {
					return *m_result;
				}
				if (m_result->eval_.type_ == EvalUpperBound && m_result->eval_.value_ <= baseAlpha) {
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
				return SearchResult(0, EvaluationRange(Evaluation(Evaluation::CPTag(), 0), EvalExact), Move(), {});
			}

			InlineStackWriter(history, rootPosition);

			if (maxExtensionDepth <= 0) {
				SearchResult result(0, EvaluationRange(evaluatePosition(rootPosition), EvalExact), Move(), {});
				return getGlobalTranspositionTable().insert(rootPosition, result);
			}

			std::vector<std::tuple<EvaluationRange, Move, std::vector<Move>>> possibleMoves;
			{
				MoveEnumerationResult enumeration = availableMoves(rootPosition);


				possibleMoves.reserve(enumeration.others.size());

				for (auto const& move : enumeration.others) {
					possibleMoves.emplace_back(EvaluationRange(), move, std::vector<Move>{});
				}
			}

			if (possibleMoves.size() == 0) {
				SearchResult result(9999, EvaluationRange(terminalEval(rootPosition), EvalExact), Move(), {});
				return getGlobalTranspositionTable().insert(rootPosition, result);
			}

			for (auto & [eval, move, line] : possibleMoves) {
				Board resultingPosition = rootPosition;
				resultingPosition.makeMove(move);
				SearchResult recursiveResult = recursiveSearch(resultingPosition, history, worstEval, bestEval, 0, 0, false);
				recursiveResult.eval_.step();
				eval = recursiveResult.eval_;
				line = std::move(recursiveResult.line_);
				line.push_back(move);
			}
			std::sort(possibleMoves.begin(), possibleMoves.end(), [](auto& a, auto& b) { return std::get<0>(a).value_ > std::get<0>(b).value_; });
			if (maxDepth <= 0) {
				// We are in search extension, only keep the most interesting moves
				possibleMoves.resize(std::min<size_t>(5, possibleMoves.size()));
			}
			if (maxExtensionDepth > 1) {
				Evaluation alpha = baseAlpha;
				bool prune = false;
				for (auto& [eval, move, line] : possibleMoves) {
					if (!prune) {
						Board resultingPosition = rootPosition;
						resultingPosition.makeMove(move);
						SearchResult recursiveResult = recursiveSearch(resultingPosition, history, unstep(baseBeta), unstep(alpha), maxDepth - 1, maxExtensionDepth - 1, false);
						recursiveResult.eval_.step();
						eval = recursiveResult.eval_;
						line = std::move(recursiveResult.line_);
						line.push_back(move);

						if (eval.value_ >= baseBeta && maxDepth > 0) {
							prune = true;
						}
						if (eval.value_ > alpha) {
							alpha = eval.value_;
						}
					}
					else {
						eval = EvaluationRange();
					}
				}
				std::sort(possibleMoves.begin(), possibleMoves.end(), [](auto& a, auto& b) { return std::get<0>(a).value_ > std::get<0>(b).value_; });
			}

			std::optional<SearchResult> result;
			if (maxDepth <= 0) {
				SearchResult stopResult(maxExtensionDepth, EvaluationRange(evaluatePosition(rootPosition), EvalExact), Move(), {});
				if (stopResult.eval_.value_ > std::get<0>(possibleMoves[0]).value_) {
					result = stopResult;
				}
			}
			if (!result.has_value()) {
				EvaluationRange evalRange = std::get<0>(possibleMoves[0]);
				result = SearchResult(maxExtensionDepth, evalRange, std::get<1>(possibleMoves[0]), std::get<2>(possibleMoves[0]));
			}
			if (result->eval_.value_ <= baseAlpha && maxDepth > 0) {
				result->eval_.type_ = EvalUpperBound;
			}
			if (result->eval_.value_ >= baseBeta && maxDepth > 0) {
				result->eval_.type_ = EvalLowerBound;
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

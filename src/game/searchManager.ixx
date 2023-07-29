export module cherry.searchManager;

import std;

import cherry.board;
import cherry.evaluation;
import cherry.move;
import cherry.moveEnumeration;
import cherry.positionEval;
import cherry.searchWorker;
import cherry.uci.commandEmitter;
import cherry.uci.cmd.bestmove;
import cherry.uci.cmd.info;

export namespace cherry {

	class SearchManager {
	public:
		SearchManager()
			: currentPosition_(startingPosition) {}

		void setEmitter(uci::CommandEmitter* emitter) {
			emitter_ = emitter;
		}

		void setPosition(Board rootPosition, std::vector<Move> const& moves) {
			while (history_.size() > 0) {
				history_.pop();
			}

			currentPosition_ = rootPosition;
			for (const auto& move : moves) {
				history_.push(currentPosition_);
				currentPosition_.makeMove(move);
			}
		}

		void go(std::optional<int> wTime, std::optional<int> bTime) {
			std::optional<int> m_time = currentPosition_.whiteToPlay_ ? wTime : bTime;
			// Crudely assume there will be another 20 moves and we should use our time evenly
			int allowedTime = m_time.has_value() ? (*m_time / 20) : 1000;

			worker_.reset();
			workerThread_ = std::jthread([this]() {
				worker_.iterativeDeepening(currentPosition_, history_);
				});

			supervisorThread_ = std::jthread([this, allowedTime]() {
				const auto start = std::chrono::steady_clock::now();
				auto now = start;
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				do {
					now = std::chrono::steady_clock::now();
					const auto elapsed = now - start;
					emitter_->emitCommand(uci::command::UCIInfo({
						.timeMs = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count(),
						.nodes = worker_.nodesVisited_.load(std::memory_order_relaxed),
						.depth = worker_.depth_.load(std::memory_order_relaxed),
						.score = worker_.eval_.load(std::memory_order_relaxed)}));
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
				} while (!worker_.complete_.load(std::memory_order_acquire) && std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() < allowedTime);

				worker_.shouldStop_.store(true, std::memory_order_relaxed);
				workerThread_.join();
				Move bestMove = worker_.bestMove_.load(std::memory_order_relaxed);
				if (bestMove != Move()) {
					emitter_->emitCommand(uci::command::BestMove(bestMove));
				}
				});
		}

		void stopSearch() {
			worker_.shouldStop_.store(true, std::memory_order_relaxed);
		}

		Move bestMoveBlocking() {
			supervisorThread_.join();
			return worker_.bestMove_.load(std::memory_order_relaxed);
		}

	private:
		uci::CommandEmitter* emitter_;
		Board currentPosition_;
		SearchWorker worker_;
		std::jthread supervisorThread_;
		std::jthread workerThread_;
		InlineStack<Board, 512> history_;
	};

	SearchManager& getGlobalSearchManager() {
		static SearchManager search = SearchManager();
		return search;
	}

} // namespace cherry

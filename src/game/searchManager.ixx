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

		void setPosition(Board position) {
			currentPosition_ = std::move(position);
		}

		void go(std::optional<int> wTime, std::optional<int> bTime) {
			std::optional<int> m_time = currentPosition_.whiteToPlay_ ? wTime : bTime;
			// Crudely assume there will be another 20 moves and we should use our time evenly
			int allowedTime = m_time.has_value() ? (*m_time / 20) : 1000000;

			worker_.reset();
			workerThread_ = std::jthread([this]() {
				worker_.recursiveSearch(currentPosition_, worstEval, bestEval, 4, 12);
				});

			supervisorThread_ = std::jthread([this, allowedTime]() {
				const auto start = std::chrono::steady_clock::now();
				auto now = start;
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				do {
					now = std::chrono::steady_clock::now();
					const auto elapsed = now - start;
					int nodesVisited = worker_.nodesVisited_.load(std::memory_order_relaxed);
					emitter_->emitCommand(uci::command::UCIInfo({
						.timeMs = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count(),
						.nodes = worker_.nodesVisited_.load(std::memory_order_relaxed),
						.score = worker_.eval_.load(std::memory_order_relaxed)}));
					std::this_thread::sleep_for(std::chrono::milliseconds(100));

					if (worker_.complete_.load(std::memory_order_acquire)) {
						emitter_->emitCommand(uci::command::BestMove(worker_.bestMove_.load(std::memory_order_relaxed)));
						return;
					}

				} while (std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() < allowedTime);

				worker_.shouldStop_.store(true, std::memory_order_relaxed);
				workerThread_.join();
				emitter_->emitCommand(uci::command::BestMove(worker_.bestMove_.load(std::memory_order_relaxed)));
				});
		}

		void stopSearch() {
			worker_.shouldStop_.store(true, std::memory_order_relaxed);
		}

		Move bestMoveBlocking() {
			workerThread_.join();
			return worker_.bestMove_.load(std::memory_order_relaxed);
		}

	private:
		uci::CommandEmitter* emitter_;
		Board currentPosition_;
		SearchWorker worker_;
		std::jthread supervisorThread_;
		std::jthread workerThread_;
	};

	SearchManager& getGlobalSearchManager() {
		static SearchManager search = SearchManager();
		return search;
	}

} // namespace cherry

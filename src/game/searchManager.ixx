export module cherry.searchManager;

import std;

import cherry.board;
import cherry.evaluation;
import cherry.move;
import cherry.moveEnumeration;
import cherry.positionEval;
import cherry.searchWorker;
import cherry.uci.commandEmitter;
import cherry.uci.cmd.info;

export namespace cherry {

	class SearchManager {
	public:
		SearchManager()
			: currentPosition_(startingPosition) {}

		void setPosition(Board position) {
			currentPosition_ = std::move(position);
		}

		Move stopSearch(uci::CommandEmitter* emitter) {
			const auto start = std::chrono::steady_clock::now();
			auto [eval, move] = worker.recursiveSearch(currentPosition_, worstEval, bestEval, 4, 12);
			if (emitter != nullptr) {
				const auto elapsed = std::chrono::steady_clock::now() - start;
				emitter->emitCommand(uci::command::UCIInfo({
					.timeMs = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count(),
					.nodes = worker.nodesVisited_.load(std::memory_order_relaxed),
					.score = eval }));
			}
			return move;
		}

	private:
		Board currentPosition_;
		SearchWorker worker;
	};

	SearchManager& getGlobalSearchManager() {
		static SearchManager search = SearchManager();
		return search;
	}

} // namespace cherry

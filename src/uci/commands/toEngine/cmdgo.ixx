export module cherry.uci.cmd.go;

import std;
import cherry.move;
import cherry.searchManager;
import cherry.uci.commandEmitter;
import cherry.uci.cmd.bestmove;

export namespace cherry::uci::command {

	class Go {
	public:
		explicit Go(std::span<std::string_view> /* tokens */) {
		}
	};

	void handleCommand(CommandEmitter& emitter, Go const& /* command */) {
		Move bestMove = getGlobalSearchManager().stopSearch(&emitter);
		emitter.emitCommand(BestMove(bestMove));
	}

} // namespace cherry::uci::command

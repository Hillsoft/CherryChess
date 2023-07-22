export module cherry.uci.cmd.isready;

import std;
import cherry.uci.commandEmitter;
import cherry.uci.cmd.readyok;

export namespace cherry::uci::command {

	class IsReady {
	public:
		explicit IsReady(std::span<std::string_view> /* tokens */) {
		}
	};

	void handleCommand(CommandEmitter& emitter, IsReady const& /* command */) {
		emitter.emitCommand(ReadyOk());
	}

} // namespace cherry::uci::command

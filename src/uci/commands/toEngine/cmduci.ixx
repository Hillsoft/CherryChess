export module cherry.uci.cmd.uci;

import std;
import cherry.uci.cmd.uciok;
import cherry.uci.cmd.id;
import cherry.uci.commandEmitter;

export namespace cherry::uci::command {

	class UCI {
	public:
		explicit UCI(std::span<std::string_view> /* tokens */) {
		}
	};

	void handleCommand(CommandEmitter& emitter, UCI const& /* command */) {
		emitter.emitCommand(UCIId("name", "Cherry Chess"));
		emitter.emitCommand(UCIId("author", "Matthew Hillman"));
		emitter.emitCommand(UCIOk());
	}

} // namespace cherry::uci::command

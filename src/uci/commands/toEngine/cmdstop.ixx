export module cherry.uci.cmd.stop;

import std;
import cherry.searchManager;
import cherry.uci.commandEmitter;

export namespace cherry::uci::command {

	class Stop {
	public:
		explicit Stop(std::span<std::string_view> /* tokens */) {
		}
	};

	void handleCommand(CommandEmitter& emitter, Stop const& /* command */) {
		getGlobalSearchManager().stopSearch();
	}

} // namespace cherry::uci::command

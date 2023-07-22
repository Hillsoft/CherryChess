export module cherry.uci.cmd.quit;

import std;
import cherry.uci.commandEmitter;

export namespace cherry::uci::command {

	class Quit {
	public:
		explicit Quit(std::span<std::string_view> /* tokens */) {
		}
	};

	void handleCommand(CommandEmitter& /* emitter */, Quit const& /* command */) {}

} // namespace cherry::uci::command

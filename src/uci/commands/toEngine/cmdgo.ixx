export module cherry.uci.cmd.go;

import std;
import cherry.move;
import cherry.searchManager;
import cherry.uci.commandEmitter;

export namespace cherry::uci::command {

	class Go {
	public:
		explicit Go(std::span<std::string_view> tokens) {
			for (size_t i = 0; i < tokens.size(); i++) {
				if (tokens[i] == "wtime" && i + 1 < tokens.size()) {
					wTime_ = std::stoi(std::string(tokens[i + 1]));
				}
				if (tokens[i] == "btime" && i + 1 < tokens.size()) {
					bTime_ = std::stoi(std::string(tokens[i + 1]));
				}
			}
		}

		std::optional<int> wTime_;
		std::optional<int> bTime_;
	};

	void handleCommand(CommandEmitter& emitter, Go const& command) {
		auto& searchManager = getGlobalSearchManager();
		searchManager.setEmitter(&emitter);
		searchManager.go(command.wTime_, command.bTime_);
	}

} // namespace cherry::uci::command

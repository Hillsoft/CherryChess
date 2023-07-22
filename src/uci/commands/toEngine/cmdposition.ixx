export module cherry.uci.cmd.position;

import std;
import cherry.board;
import cherry.searchManager;
import cherry.uci.commandEmitter;

export namespace cherry::uci::command {

	class Position {
	public:
		explicit Position(std::span<std::string_view> tokens) {
			position_ = startingPosition;
			if (tokens.size() < 3) {
				throw std::runtime_error("Unsupported position command");
			}
			auto cursor = tokens.begin();
			if (*cursor != "startpos") {
				throw std::runtime_error("Unsupported position command");
			}
			cursor++;
			if (*cursor != "moves") {
				throw std::runtime_error("Unsupported position command");
			}
			cursor++;
			for (; cursor != tokens.end(); cursor++) {
				position_.makeMove(Move(*cursor));
			}
		}

		Board position_;
	};

	void handleCommand(CommandEmitter& emitter, Position const& command) {
		getGlobalSearchManager().setPosition(command.position_);
	}

} // namespace cherry::uci::command

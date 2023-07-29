export module cherry.uci.cmd.position;

import std;
import cherry.board;
import cherry.searchManager;
import cherry.uci.commandEmitter;

export namespace cherry::uci::command {

	class Position {
	public:
		explicit Position(std::span<std::string_view> tokens) {
			if (tokens.size() < 1) {
				throw std::runtime_error("Unsupported position command");
			}
			auto cursor = tokens.begin();
			if (*cursor == "startpos") {
				rootPosition_ = startingPosition;
				cursor++;
			}
			else if (*cursor == "fen") {
				cursor++;
				if (tokens.size() < 7) {
					throw std::runtime_error("Unsupported position command");
				}
				std::ostringstream fenstream;
				for (size_t i = 0; i < 5; i++) {
					fenstream << *cursor << " ";
					cursor++;
				}
				fenstream << *cursor;
				cursor++;
				rootPosition_ = Board(fenstream.str());
			}
			else {
				throw std::runtime_error("Unsupported position command");
			}

			if (cursor == tokens.end()) {
				return;
			}
			if (*cursor != "moves") {
				throw std::runtime_error("Unsupported position command");
			}
			cursor++;
			for (; cursor != tokens.end(); cursor++) {
				moves_.emplace_back(*cursor);
			}
		}

		Board rootPosition_;
		std::vector<Move> moves_;
	};

	void handleCommand(CommandEmitter& emitter, Position const& command) {
		getGlobalSearchManager().setPosition(command.rootPosition_, command.moves_);
	}

} // namespace cherry::uci::command

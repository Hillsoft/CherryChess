export module cherry.uci.interfaceManager;

import std;

import cherry.concurrentQueue;
import cherry.searchManager;
import cherry.uci.commandEmitter;
import cherry.uci.toEngine;

export namespace cherry::uci {

	class InterfaceManager {
	public:
		InterfaceManager()
			: inputThread_([this]() { runInput(); }),
			workerThread_([this]() { runWorker(); }) {
		}

		~InterfaceManager() {
			inputThread_.join();
			workerThread_.join();
		}

		void runInput() {
			bool quitting = false;
			while (!quitting) {
				std::string currentLine;
				getline(std::cin, currentLine);
				std::optional<ToEngineCommand> command = parseCommand(currentLine);
				if (command.has_value()) {
					if (std::holds_alternative<command::Quit>(command->command_)) {
						quitting = true;
					}
					commandQueue.push(std::move(*command));
				}
			}
		}

		void runWorker() {
			while (true) {
				ToEngineCommand command = commandQueue.pop();

				std::visit([this](auto const& cmd) { command::handleCommand(emitter_, cmd); }, command.command_);
					
				if (std::holds_alternative<command::Quit>(command.command_)) {
					return;
				}
			}
		}
		
	private:
		CommandEmitter emitter_;
		ConcurrentQueue<ToEngineCommand> commandQueue;
		std::thread inputThread_;
		std::thread workerThread_;
	};

} // namespace

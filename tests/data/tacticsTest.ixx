import std;
import test;

import cherry.board;
import cherry.move;
import cherry.searchManager;

std::vector<std::pair<std::string, std::pair<cherry::Board, cherry::Move>>> getTactics() {
	std::vector<std::pair<std::string, std::pair<cherry::Board, cherry::Move>>> result;
	std::ifstream inputFile(std::format("{}{}", cherry::test::kFilePath, "/tactics.txt"));
	if (!inputFile.is_open()) {
		throw std::runtime_error("Could not open test data file");
	}
	std::string section = "";
	int index = 0;
	
	std::string currentLine;
	while (std::getline(inputFile, currentLine)) {
		if (currentLine.size() == 0) {
			continue;
		}
		if (currentLine[0] == '=') {
			section = currentLine.substr(1);
			index = 0;
			continue;
		}

		index++;
		cherry::Board currentBoard(currentLine);
		std::getline(inputFile, currentLine);
		cherry::Move move(currentLine);
		result.push_back({ std::format("{} - {}", section, index), { currentBoard, move } });
	}

	return result;
}

cherry::test::TestP<std::pair<cherry::Board, cherry::Move>> Tactics("Tactics", [](cherry::test::TestRunner& runner, std::pair<cherry::Board, cherry::Move> const& param) {
	const auto& [board, expectedMove] = param;

	cherry::SearchManager searchManager;
	searchManager.setPosition(board);
	cherry::Move move = searchManager.stopSearch(nullptr);

	runner.expectEq(expectedMove, move);
	},
	getTactics());

int main() {
	return cherry::test::runAllTests() ? 0 : 1;
}

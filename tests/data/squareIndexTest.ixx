import std;
import test;

import cherry.squareIndex;

cherry::test::Test SquareIndexParsing("SquareIndexParsing", [](cherry::test::TestRunner& runner) {
	runner.expectEq<std::string_view>("a1", cherry::SquareIndex("a1").getCode());
	runner.expectEq<std::string_view>("c3", cherry::SquareIndex("c3").getCode());
	runner.expectEq<std::string_view>("e1", cherry::SquareIndex("e1").getCode());
	runner.expectEq<std::string_view>("h8", cherry::SquareIndex("h8").getCode());
	});

int main() {
	return cherry::test::runAllTests() ? 0 : 1;
}

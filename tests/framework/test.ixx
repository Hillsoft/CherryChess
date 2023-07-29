export module test;

import std;

#include "framework/test_path.h"

export namespace cherry::test {

	constexpr auto kFilePath = constants::kFilePath;

	class TestRunner {
	public:
		TestRunner(std::string name, std::function<void(TestRunner& owner)> runTest)
			: name_(std::move(name)), run_(std::move(runTest)) {}

		std::string const& getName() const {
			return name_;
		}

		std::vector<std::string> const& run() {
			errors_.clear();
			run_(*this);
			return errors_;
		}

		template<typename T>
		void expectEq(std::vector<T> const& a, std::vector<T> const& b) {
			if (a != b) {
				auto fmtToString = [](T const& x) {
					return std::format("{}", x);
				};
				std::ostringstream aRep;
				std::vector<std::string> aStrs;
				std::transform(a.begin(), a.end(), std::back_inserter(aStrs), fmtToString);
				std::copy(aStrs.begin(), aStrs.end(), std::ostream_iterator<std::string>(aRep, ", "));
				std::ostringstream bRep;
				std::vector<std::string> bStrs;
				std::transform(b.begin(), b.end(), std::back_inserter(bStrs), fmtToString);
				std::copy(bStrs.begin(), bStrs.end(), std::ostream_iterator<std::string>(bRep, ", "));
				errors_.push_back(std::format("Expected equality of:\n\t{}\n\t{}", aRep.str(), bRep.str()));
			}
		}

		template<typename T>
		void expectEq(const T& a, const T& b) {
			if (a != b) {
				errors_.push_back(std::format("Expected equality of {} and {}", a, b));
			}
		}

	private:
		std::string name_;
		std::function<void(TestRunner& owner)> run_;
		std::vector<std::string> errors_;
	};

	std::vector<TestRunner>& getTestRunners() {
		static std::vector<TestRunner> runners = std::vector<TestRunner>();
		return runners;
	}
	
	class Test {
	public:
		Test(std::string name, std::function<void(TestRunner& owner)> runTest) {
			getTestRunners().emplace_back(std::move(name), runTest);
		}
	};

	template<typename TParam>
	class TestP {
	public:
		TestP(std::string name, std::function<void(TestRunner& owner, const TParam& param)> runTest, std::vector<std::pair<std::string, TParam>> params)
			: runTest_(std::move(runTest)), params_(std::move(params)) {
			for (const auto& [paramName, paramData] : params_) {
				getTestRunners().emplace_back(std::format("{}: {}", name, paramName), [&paramData, &runTest_ = this->runTest_](TestRunner& owner) {
					runTest_(owner, paramData);
					});
			}
		}

	private:
		std::function<void(TestRunner& owner, const TParam& param)> runTest_;
		std::vector<std::pair<std::string, TParam>> params_;
	};

	bool runAllTests() {
		auto& runners = getTestRunners();
		std::cout << "Detected " << runners.size() << " tests" << std::endl;

		bool passed = true;
		for (auto& runner : runners) {
			std::cout << "Running: " << runner.getName() << "..." << std::endl;
			std::vector<std::string> const& errors = runner.run();
			if (errors.empty()) {
				std::cout << "\tPassed!" << std::endl;
			}
			else {
				passed = false;
				for (auto const& e : errors) {
					std::cout << "\tError: " << e << std::endl;
				}
			}
		}

		return passed;
	}

} // namespace cherry::test

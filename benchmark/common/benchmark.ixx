export module benchmark;

import std;

export namespace cherry::benchmark {

	template <typename Fn>
	void runBenchmark(std::string_view name, Fn f) {
		std::cout << "Benchmarking " << name << std::endl;
		int iterations = 0;
		auto end = std::chrono::steady_clock::now();
		const auto start = std::chrono::steady_clock::now();
		do {
			f();
			iterations++;
		} while ((end = std::chrono::steady_clock::now()) - start < std::chrono::seconds(10));
		const auto elapsed = end - start;
		std::cout << "Time elapsed: " << elapsed << std::endl;
		std::cout << "Iterations: " << iterations << std::endl;
		std::cout << "Time per iteration: " << elapsed / iterations << std::endl;
	}

} // namespace cherry::benchmark

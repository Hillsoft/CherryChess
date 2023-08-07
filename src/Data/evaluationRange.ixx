export module cherry.evaluationRange;

import std;

import cherry.evaluation;

export namespace cherry {

	class EvaluationRange {
	public:
		constexpr EvaluationRange()
			: low_(worstEval), high_(bestEval) {}

		explicit constexpr EvaluationRange(Evaluation eval)
			: low_(eval), high_(eval) {}

		constexpr EvaluationRange(Evaluation low, Evaluation high)
			: low_(low), high_(high) {}

		constexpr EvaluationRange(EvaluationRange const& other) = default;
		constexpr EvaluationRange(EvaluationRange&& other) = default;

		constexpr EvaluationRange& operator=(EvaluationRange const& other) = default;
		constexpr EvaluationRange& operator=(EvaluationRange&& other) = default;

		void join(EvaluationRange const& other) {
			low_ = std::max(low_, other.low_);
			high_ = std::min(high_, other.high_);
		}

		void step() {
			Evaluation tmp = low_;
			low_ = high_;
			high_ = tmp;
			low_.step();
			high_.step();
		}

		Evaluation low_;
		Evaluation high_;
	};

} // namespace cherry

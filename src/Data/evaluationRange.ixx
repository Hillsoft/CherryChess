export module cherry.evaluationRange;

import std;

import cherry.evaluation;

export namespace cherry {

	enum EvalType {
		EvalExact = 0,
		EvalLowerBound = 1,
		EvalUpperBound = 2,
	};

	class EvaluationRange {
	public:
		constexpr EvaluationRange()
			: value_(worstEval), type_(EvalLowerBound) {}

		constexpr EvaluationRange(Evaluation eval, EvalType type)
			: value_(eval), type_(type) {}

		constexpr EvaluationRange(EvaluationRange const& other) = default;
		constexpr EvaluationRange(EvaluationRange&& other) = default;

		constexpr EvaluationRange& operator=(EvaluationRange const& other) = default;
		constexpr EvaluationRange& operator=(EvaluationRange&& other) = default;

		void join(EvaluationRange const& other) {
			*this = other;
		}

		void step() {
			value_.step();
			if (type_ == EvalLowerBound) {
				type_ = EvalUpperBound;
			}
			else if (type_ == EvalUpperBound) {
				type_ = EvalLowerBound;
			}
		}

		Evaluation value_;
		EvalType type_;
	};

} // namespace cherry

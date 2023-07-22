export module cherry.concurrentQueue;

import std;

export namespace cherry {

	template <typename TItem>
	class ConcurrentQueue {
	public:
		ConcurrentQueue() {
		}

		void push(TItem item) {
			std::scoped_lock queueLock(queueMutex_);
			queue_.push(std::move(item));
			if (!hasMessages_.load(std::memory_order::memory_order_relaxed))
			{
				hasMessages_.store(true, std::memory_order::memory_order_release);
				hasMessages_.notify_one();
			}
		}

		TItem pop() {
			hasMessages_.wait(false, std::memory_order_acquire);
			std::scoped_lock queueLock(queueMutex_);
			TItem result = std::move(queue_.front());
			queue_.pop();
			if (queue_.size() == 0) {
				hasMessages_.store(false, std::memory_order_release);
			}
			return result;
		}

	private:
		std::atomic<bool> hasMessages_;
		std::mutex queueMutex_;
		std::queue<TItem> queue_;
	};

} // namespace cherry

#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t numThreads) {
	this->numThreads = numThreads;
	this->stop = false;

	workerThreadsVector.reserve(numThreads);
	for (size_t i = 1; i <= numThreads; i++) {
		workerThreadsVector.emplace_back([this] {
			this->WorkerThread();
		});
	}
}

ThreadPool::~ThreadPool() {
	stop = true;
	cvJobQ.notify_all();

	for (auto& t : workerThreadsVector) {
		t.join();
	}
}

void ThreadPool::WorkerThread() {
	while (true) {
		unique_lock<mutex> lock(mJobQ);
		cvJobQ.wait(lock, [this]() {return !this->jobs.empty() || stop; });
		
		if (stop && this->jobs.empty())
			return;

		function<void()> job = move(jobs.front());
		jobs.pop();
		lock.unlock();

		job();
	}
}

void ThreadPool::EnqueueJobs(function<void()> job) {
	if (stop) {
		throw runtime_error("Stop All Thread");
	}
	{
		lock_guard<mutex> lock(mJobQ);
		jobs.push(move(job));
	}

	cvJobQ.notify_one();
}
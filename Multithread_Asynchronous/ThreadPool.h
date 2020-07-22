#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <functional>

using namespace std;

class ThreadPool {
private:
	size_t numThreads;
	vector<thread> workerThreadsVector;
	queue<function<void()>> jobs;
	condition_variable cvJobQ;
	mutex mJobQ;

	bool stop;
	void WorkerThread();
public:
	//Constructor
	ThreadPool(size_t numThreads);
	~ThreadPool();
	//add job
	void EnqueueJobs(function<void()> job);
};
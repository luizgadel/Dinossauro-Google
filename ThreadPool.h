#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

class ThreadPool
{
public:
    explicit ThreadPool(int numWorkers)
        : nextIndex_(0),
          rangeEnd_(0),
          workersRemaining_(0),
          batchId_(0),
          stop_(false)
    {
        workerBatchId_.assign(numWorkers, 0);
        workers_.reserve(numWorkers);
        for (int i = 0; i < numWorkers; i++)
        {
            workers_.emplace_back(&ThreadPool::workerLoop, this, i);
        }
    }

    ~ThreadPool()
    {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            stop_ = true;
            batchId_++;
        }
        cvWork_.notify_all();
        for (auto &worker : workers_)
        {
            worker.join();
        }
    }

    void parallelFor(int start, int end, std::function<void(int)> task)
    {
        if (start >= end || workers_.empty())
        {
            return;
        }

        {
            std::lock_guard<std::mutex> lock(mtx_);
            task_ = std::move(task);
            nextIndex_.store(start);
            rangeEnd_ = end;
            workersRemaining_ = static_cast<int>(workers_.size());
            batchId_++;
        }
        cvWork_.notify_all();

        std::unique_lock<std::mutex> lock(mtx_);
        cvDone_.wait(lock, [this] { return workersRemaining_ == 0; });
    }

private:
    void workerLoop(int workerId)
    {
        while (true)
        {
            {
                std::unique_lock<std::mutex> lock(mtx_);
                cvWork_.wait(lock, [this, workerId] {
                    return stop_ || workerBatchId_[workerId] != batchId_;
                });
                if (stop_)
                {
                    return;
                }
                workerBatchId_[workerId] = batchId_;
            }

            while (true)
            {
                int i = nextIndex_.fetch_add(1);
                if (i >= rangeEnd_)
                {
                    break;
                }
                task_(i);
            }

            {
                std::lock_guard<std::mutex> lock(mtx_);
                if (--workersRemaining_ == 0)
                {
                    cvDone_.notify_one();
                }
            }
        }
    }

    std::vector<std::thread> workers_;
    std::vector<int> workerBatchId_;
    std::function<void(int)> task_;
    std::atomic<int> nextIndex_;
    int rangeEnd_;
    int workersRemaining_;
    int batchId_;
    std::mutex mtx_;
    std::condition_variable cvWork_;
    std::condition_variable cvDone_;
    bool stop_;
};

#endif

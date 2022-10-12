#pragma once

#include <functional>
#include <future>
#include <iostream>
#include <queue>
#include <thread>
#include <vector>

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

class ThreadPool
{
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    /* For sync usage, protect the `tasks` queue and `stop` flag. */
    std::mutex mtx;
    std::condition_variable cv;
    bool stop;

  public:
    ThreadPool(const ThreadPool &) = delete;
    ThreadPool(ThreadPool &&) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;
    ThreadPool &operator=(ThreadPool &&) = delete;
    ThreadPool(size_t nr_threads);
    virtual ~ThreadPool();

    template<class F, class... Args>
    std::future<typename std::result_of<F(Args...)>::type> enqueue(F&& f, Args&&... args)
    {
        /* The return type of task `F` */
        using return_type = std::result_of_t<F(Args...)>;

        /* wrapper for no arguments */
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock lock(mtx);

            if (stop)
                throw std::runtime_error("The thread pool has been stop.");

            /* wrapper for no returned value */
            tasks.emplace([task]() -> void { (*task)(); });
        }
        cv.notify_one();
        return res;
    }
};

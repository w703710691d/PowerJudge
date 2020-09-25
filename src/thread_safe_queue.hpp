//
// Created by w703710691d on 18-8-27.
//

#ifndef POWERJUDGE_THREAD_SAFE_QUEUE_HPP
#define POWERJUDGE_THREAD_SAFE_QUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class ThreadSafeQueue {
private:
    std::queue<T *> q;
    std::mutex mut;
    std::condition_variable data_cond;
    bool m_bRunning;
public:
    ThreadSafeQueue() {
        m_bRunning = false;
    }

    void start() {
        m_bRunning = true;
    }

    void stop() {
        m_bRunning = false;
        data_cond.notify_all();
    }

    T *GetFrontAndPop() {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] {
            if (m_bRunning) {
                return !this->q.empty();
            } else {
                return true;
            }
        });
        if (m_bRunning) {
            T *ret = q.front();
            q.pop();
            return ret;
        } else {
            return nullptr;
        }
    }

    void push(T *x) {
        std::unique_lock<std::mutex> lk(mut);
        q.push(*x);
        data_cond.notify_one();
    }
};

#endif //POWERJUDGE_THREAD_SAFE_QUEUE_HPP

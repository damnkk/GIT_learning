#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <mutex>
#include <thread>
#include <condition_variable>
#include <vector>
#include <queue>
#include <functional>

template<typename T, typename ... Args>
std::unique_ptr<T> make_unique(Args&& ... args){
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

class Thread{
private:
    bool                                            m_destroying = false;
    std::thread                                     m_worker;
    std::mutex                                      m_mutex;
    std::queue<std::function<void()>>               m_jobQueue;
    std::condition_variable                         m_condition;

    void jobQueue(){
        while(true){
            std::function<void()> job;
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                m_condition.wait(lock,[this](){return !m_jobQueue.empty()||m_destroying;});
                if(m_destroying){
                    break;
                }
                job = m_jobQueue.front();
            }
            job();
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_jobQueue.pop();
                m_condition.notify_one();
            }
        }
    }
public:
    Thread(){
        m_worker = std::thread(&Thread::jobQueue,this);
    }
    ~Thread(){
        if(m_worker.joinable()){
            wait();
            m_mutex.lock();
            m_destroying = true;
            m_condition.notify_one();
            m_mutex.unlock();
            m_worker.join();
        }
    }

    void addJob(std::function<void()> func){
        std::lock_guard<std::mutex> lock(m_mutex);
        m_jobQueue.push(std::move(func));
        m_condition.notify_one();
    }

    void wait(){
        std::unique_lock<std::mutex> lock(m_mutex);
        m_condition.wait(lock,[this](){return m_jobQueue.empty();});
    }
};

class ThreadPool{
public:
    std::vector<std::unique_ptr<Thread>> m_threads;
    void setPoolSize(uint32_t threadNum){
        m_threads.clear();
        for(int i = 0;i<threadNum;++i){
            m_threads.push_back(make_unique<Thread>());
        }
    }
    void wait(){
        for(auto& i:m_threads){
            i->wait();
        }
    }
};

#endif
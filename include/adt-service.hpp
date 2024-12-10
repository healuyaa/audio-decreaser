#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>


namespace adt {
    class Service {
        public:
        using Task = std::function<void()>;

        Service();
        ~Service();

        void postTask(Task task);

        // void subscribe();

        private:
        std::atomic<bool> is_running;
        std::thread thread;
        std::mutex mtx;
        std::condition_variable cv;
        
        std::queue<std::function<void()>> tasks;

        void processTask();
        void stopService();
    };
}
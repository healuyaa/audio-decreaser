#include "adt-service.hpp"
#include <mutex>

namespace adt {
    Service::Service() : is_running(true), thread(&Service::processTask, this) {}

    Service::~Service() {
        stopService();
    }

    void Service::postTask(Task task) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            tasks.push(std::move(task));
        }
        cv.notify_one();
    }

    void Service::processTask() {
        while (is_running) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [this] () {
                    return !tasks.empty() || !is_running;
                });

                if(!is_running) break;

                task = std::move(tasks.front());
                tasks.pop();
            }
            task();
        }
    }

    void Service::stopService() {
        is_running = false;
        cv.notify_one();
        if(thread.joinable()) {
            thread.join();
        }
    }
}
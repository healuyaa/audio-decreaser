#pragma once

#include "adt-audio-tools.hpp"
#include <filesystem>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <vector>

namespace adt {
    class Compress {
        public:
        Compress(const std::vector<std::string>& paths, int count_threads = 10);
        ~Compress();

        void run();

        private:
        std::vector<std::string> paths;
        int count_threads;

        std::vector<std::thread> threads;
        std::mutex data_mutex;
        
        std::mutex cout_mutex; // delete

        std::shared_ptr<adt::AudioTools> tools;

        std::optional<std::string> GetNextElement();
        void exec_command(int id);

        std::string generate_command(const std::filesystem::path& input);
    };
}
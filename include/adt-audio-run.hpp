#pragma once

#include "adt-audio-tools.hpp"

#include <memory>

namespace adt {
    class AudioRunner {
        public:
        AudioRunner() = default;
        ~AudioRunner() = default;

        void run(std::atomic<float>* progress);
        void ChangeThreads(int n = 10);

        private:
        int number_threads;
        std::shared_ptr<AudioTools> tools;

        void clearDirector(const std::filesystem::path& path);
    };
}
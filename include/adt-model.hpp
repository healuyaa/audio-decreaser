#pragma once

#include <filesystem>
#include <future>
#include <string>

namespace adt {
    class Model {
        public:
        Model();
        ~Model();

        void run();
        void wait();
        int getExitCode() const;
        void SetPaths(std::filesystem::path &name_out_dir, std::filesystem::path &path_to_file);

        private:
        const std::filesystem::path path_to_script = "../model/model.py";
        std::filesystem::path name_out_dir;
        std::filesystem::path path_to_file;

        std::string command;

        int exitCode_;
        bool isRunning_;
        std::future<int> resultFuture_;

        void makeCommand();
        int runScript();
    };
}
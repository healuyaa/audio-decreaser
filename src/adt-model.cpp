#include "adt-model.hpp"

#include <iostream>

namespace adt {
    Model::Model() : exitCode_(0), isRunning_(false) {}

    Model::~Model() {
        wait();
    }

    void Model::run() {
        if (isRunning_) {
            std::cerr << "Script is already running!" << std::endl;
            return;
        }

        makeCommand();

        isRunning_ = true;
        resultFuture_ = std::async(std::launch::async, &Model::runScript, this);
    }

    void Model::wait() {
        if (!isRunning_) {
            std::cerr << "No script is running!" << std::endl;
            return;
        }

        try {
            exitCode_ = resultFuture_.get();
        } catch (const std::exception& e) {
            std::cerr << "Error during script execution: " << e.what() << std::endl;
            exitCode_ = -1;
        }

        isRunning_ = false;
    }

    int Model::getExitCode() const {
        return exitCode_;
    }

    void Model::SetPaths(std::filesystem::path &name_out_dir, std::filesystem::path &path_to_file) {
        this->name_out_dir = name_out_dir;
        this->path_to_file = path_to_file;
    }

    void Model::makeCommand() {
        std::cout << name_out_dir.string() + "/" + path_to_file.filename().string();
        command = "python3 -m encodec " + path_to_file.string() + " " + name_out_dir.string() + "/" + path_to_file.filename().string();
        std::cout << "Generated command: " << command << std::endl;
    }

    int Model::runScript() {
        try {
            return std::system(command.c_str());
        } catch (const std::exception& e) {
            std::cerr << "Error running script: " << e.what() << std::endl;
            return -1;
        }
    }
}
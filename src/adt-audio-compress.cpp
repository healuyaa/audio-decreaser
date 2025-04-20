#include "adt-audio-compress.hpp"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <stringapiset.h>


namespace adt {
    Compress::Compress(const std::vector<std::filesystem::path>& paths, int count_threads) 
        : paths(paths), count_threads(count_threads) {
    }

    Compress::~Compress() {
        for (auto& t : threads) {
            if (t.joinable())
                t.join();
        }

        std::cout << "destructor complete\n";
    }

    void Compress::run() {
        for (int i = 0; i < count_threads; ++i) {
            threads.emplace_back(&Compress::exec_command, this, i);
        }
    }

    std::optional<std::filesystem::path> Compress::GetNextElement() {
        std::lock_guard<std::mutex> lock(data_mutex);

        if (!paths.empty()) {
            auto value = paths.back();
            paths.pop_back();
            return value;
        }

        return std::nullopt;
    }

    void Compress::exec_command(int id) {
        while (true) {
            auto value = GetNextElement();
            if (!value.has_value()) break;

            auto command = generate_command(value.value()) + " > nul 2>&1";

            {
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cout << "[thread " << id << "] work: " << command << std::endl;
                // fix logic                
            }

            system(command.c_str());

            // std::this_thread::sleep_for(std::chrono::milliseconds(1)); // optional
        }
    }

    std::string Compress::generate_command(const std::filesystem::path& input) {        
        std::string filename = "compressed_" + input.filename().string();
        std::filesystem::path output_path = output_dir / std::filesystem::path(filename);

        std::string command = "cd ../model && model-v2.exe " + utf16to1251(input) + ' ' + utf16to1251(output_path); // + " --hq"
        return command;
    }

    std::string Compress::utf16to1251(const std::filesystem::path& path) {
        std::wstring wstr = path.wstring();

        int size_needed = WideCharToMultiByte(1251, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);

        std::string str(size_needed - 1, 0);

        WideCharToMultiByte(1251, 0, wstr.c_str(), -1, &str[0], size_needed, nullptr, nullptr);

        return str;
    }
}

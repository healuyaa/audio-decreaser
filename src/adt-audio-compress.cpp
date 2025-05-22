#include "adt-audio-compress.hpp"
#include "adt-audio-tools.hpp"
#include "adt-flags.hpp"
#include "adt-paths.hpp"

#include <cstdlib>
#include <filesystem>
#include <memory>
#include <mutex>
#include <string>
#include <stringapiset.h>


namespace adt {
    Compress::Compress(const std::vector<std::string>& paths, int count_threads, std::atomic<float>* progress) 
        : paths(paths), count_threads(count_threads), progress(progress) {
            size_paths = paths.size();
    }

    Compress::~Compress() {
        for (auto& t : threads) {
            if (t.joinable())
                t.join();
        }
    }

    void Compress::run() {
        for (int i = 0; i < count_threads; ++i) {
            threads.emplace_back(&Compress::exec_command, this, i);
        }
    }

    std::optional<std::string> Compress::GetNextElement() {
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

            auto command = generate_command(std::filesystem::path(value.value())) + " > nul 2>&1";            

            system(command.c_str());

            int done = ++current_progress;
            if(progress) {
                progress->store(static_cast<float>(done) / size_paths);
            }

            // std::this_thread::sleep_for(std::chrono::milliseconds(1)); // optional
        }
    }

    std::string Compress::generate_command(const std::filesystem::path& input) {
        std::string filename = "compressed_" + input.filename().string();
        std::filesystem::path output_path = std::filesystem::path(Paths::getInstance().GetPath("fragments_dir")) /
                                            std::filesystem::path(filename);

        tools = std::make_shared<adt::AudioTools>();

        std::string command = "cd ../model && model-v2.exe " + tools->utf16to1251(input) + ' ' + tools->utf16to1251(output_path);
        command += Flags::getInstance().GetHqModel() ? " --hq" : "";

        return command;
    }
}

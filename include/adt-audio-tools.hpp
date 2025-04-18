#pragma once

#include <filesystem>
#include <vector>

namespace adt {
    class AudioTools {
        public:
        AudioTools() = default;
        ~AudioTools() = default;

        const std::vector<std::filesystem::path> get_cut_result();
        void split_audio(const std::filesystem::path &path_to_cut);
        void concat_audio(const std::vector<std::filesystem::path>& paths, const std::filesystem::path& compressed_audio);

        private:
        const std::filesystem::path path_to_temp = "../temp";
        const std::filesystem::path path_to_temp_main = "../temp/audio";

        std::vector<std::filesystem::path> fragment_paths;

        std::string utf16to1251(const std::filesystem::path& path);
    };    
}
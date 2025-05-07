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
        std::string utf16to1251(const std::filesystem::path& path);

        private:
        std::vector<std::filesystem::path> fragment_paths;        
    };    
}
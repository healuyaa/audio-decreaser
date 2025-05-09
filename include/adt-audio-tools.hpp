#pragma once

#include <filesystem>
#include <vector>

namespace adt {
    class AudioTools {
        public:
        AudioTools() = default;
        ~AudioTools() = default;

        const std::vector<std::filesystem::path> GetCutResult();

        void splitAudio(const std::filesystem::path& path_to_cut);
        void concatAudio(const std::vector<std::filesystem::path>& paths, const std::filesystem::path& compressed_audio);

        std::filesystem::path GetUniquePath(const std::filesystem::path& base_path);

        std::string utf16to1251(const std::filesystem::path& path);

        private:
        std::vector<std::filesystem::path> fragment_paths;
    };    
}
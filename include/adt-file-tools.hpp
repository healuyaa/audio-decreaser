#pragma once

#include <filesystem>
#include <vector>
#include <string>

namespace adt {
    class FileTools {
        public:

        static FileTools &getInstance();

        const void copyFileAudio(const std::string& main_file);

        const void deleteFragments(const std::vector<std::string>& paths);
        const void delete_audio(const std::string& path);

        const std::string utf16to1251(const std::filesystem::path& path);

        const void initDirs();

        private:
        FileTools() = default;
        ~FileTools() = default;

        void initTempDir();
    };
}
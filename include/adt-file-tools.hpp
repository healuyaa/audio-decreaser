#pragma once

#include <filesystem>

namespace adt {
    class FileTools {
        public:
        FileTools();
        ~FileTools() = default;

        void CopyFile(const std::filesystem::path &path);

        private:
        const std::filesystem::path path_to_out_dir = "../ADOutput";

        void initOutDir();
    };
}
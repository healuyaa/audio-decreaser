#include "adt-file-tools.hpp"

#include <iostream>

namespace adt {
    FileTools::FileTools() {
        initOutDir();
    }

    void FileTools::CopyFile(const std::filesystem::path &path) {
        try {
            if(!std::filesystem::exists(path)) {
                std::cerr << "Error: Source file does not exist: " << path << std::endl;
                return;
            }                        

            std::filesystem::copy(path, path_to_out_dir, std::filesystem::copy_options::overwrite_existing);

            std::cout << "File successfully copied to: " << path_to_out_dir << path.filename() << std::endl;
        } catch(const std::filesystem::filesystem_error &e) {
            std::cerr << "file system err" << e.what();
        } catch(const std::exception &e) {
            std::cerr << "file system err" << e.what();
        }
    }

    void FileTools::initOutDir() {
        if(!std::filesystem::exists(path_to_out_dir)) {
            std::filesystem::create_directory(path_to_out_dir);
            std::cout << "create out dir\n";
            return;
        }

        std::cout << "out dir was exists\n";
    }
}
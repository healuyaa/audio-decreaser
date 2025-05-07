#include "adt-file-tools.hpp"
#include "adt-paths.hpp"

#include <filesystem>
#include <iostream>
#include <stringapiset.h>

namespace adt {
    const void FileTools::initDirs() {
        initTempDir();
    }

    FileTools& FileTools::getInstance() {
        static FileTools instance{};
        return instance;
    }

    const void FileTools::copyFileAudio(const std::string& main_file) {
        const auto path = std::filesystem::path(main_file);
        if(!std::filesystem::exists(path)) {
            std::cout << "does not exist" << std::endl;
            return;
        }
    
        if(!std::filesystem::exists(std::filesystem::path(Paths::getInstance().GetPath("audio_dir")) / path.filename())) {
            std::filesystem::copy(path, 
                                    std::filesystem::path(Paths::getInstance().GetPath("audio_dir")) / path.filename(), 
                               std::filesystem::copy_options::none);
            std::cout << "copy complete: " << path << std::endl;
        } else {
            std::filesystem::path base_name = path.stem();
            std::filesystem::path extension_name = path.extension();
            std::filesystem::path new_file = std::filesystem::path(Paths::getInstance().GetPath("audio_dir")) / path.filename();
            int counter = 1;
    
            while(std::filesystem::exists(new_file)) {
                new_file = std::filesystem::path(Paths::getInstance().GetPath("audio_dir")) /
                (base_name.string() + "(" + std::to_string(counter) + ")" + extension_name.string());
                ++counter;
            }
            
            std::filesystem::copy(path, new_file);
    
            std::cout << "copy complete: " << new_file << std::endl;
        }    
    }

    const void FileTools::deleteFragments(const std::vector<std::string>& paths) {
        for(const auto &p: paths) {
            const auto s = std::filesystem::path(p);
            if(!std::filesystem::exists(s))
                return;        
    
            if(std::filesystem::remove(s))
                std::cout << "delete fragment complete" << std::endl;
        }
    }

    const void FileTools::delete_audio(const std::string& path) {
        const auto& p = std::filesystem::path(path);
        if(!std::filesystem::exists(p))
            return;
    
        if(std::filesystem::remove(p))
            std::cout << "delete audio complete" << std::endl;
    }

    const std::string FileTools::utf16to1251(const std::filesystem::path& path) {
        std::wstring wstr = path.wstring();

        int size_needed = WideCharToMultiByte(1251, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);

        std::string str(size_needed - 1, 0);

        WideCharToMultiByte(1251, 0, wstr.c_str(), -1, &str[0], size_needed, nullptr, nullptr);

        return str;
    }

    void FileTools::initTempDir() {
        const auto& temp_dir = Paths::getInstance().GetPath("temp_dir");
        const auto& fragments_dir = Paths::getInstance().GetPath("fragments_dir");
        const auto& audio_dir = Paths::getInstance().GetPath("audio_dir");
        const auto& results_dir = Paths::getInstance().GetPath("results");

        if(!std::filesystem::exists(temp_dir)) {
            std::filesystem::create_directories(temp_dir);
            std::cout << "create temp_dir\n";
        }

        if(!std::filesystem::exists(fragments_dir)) {
            std::filesystem::create_directories(fragments_dir);
            std::cout << "create fragments_dir\n";
        }

        if(!std::filesystem::exists(audio_dir)) {
            std::filesystem::create_directories(audio_dir);
            std::cout << "create audio_dir\n";
        }

        if(!std::filesystem::exists(results_dir)) {
            std::filesystem::create_directories(results_dir);
            std::cout << "create audio_dir\n";
        }

        std::cout << "all already created\n";
    }
}
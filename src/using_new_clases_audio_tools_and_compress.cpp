#include "adt-audio-tools.hpp"
#include "adt-audio-compress.hpp"

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

const void copy_file_audio(std::filesystem::path &main_file) {
    if(!std::filesystem::exists(main_file)) {
        std::cout << "does not exist" << std::endl;
        return;
    }

    //----------------------------------- delete------------------------------------------------
    if(std::filesystem::exists(std::filesystem::path("../temp/audio/") / main_file.filename())) {
        return;
    }
    //----------------------------------- delete------------------------------------------------

    if(!std::filesystem::exists(std::filesystem::path("../temp/audio/") / main_file.filename())) {
        std::filesystem::copy(main_file, 
                                std::filesystem::path("../temp/audio/") / main_file.filename(), 
                           std::filesystem::copy_options::none);
        std::cout << "copy complete: " << main_file << std::endl;
    } else {
        std::filesystem::path base_name = main_file.stem();
        std::filesystem::path extension_name = main_file.extension();
        std::filesystem::path new_file = std::filesystem::path("../temp/audio/") / main_file.filename();
        int counter = 1;

        while(std::filesystem::exists(new_file)) {
            new_file = std::filesystem::path("../temp/audio/") / (base_name.string() + "(" + std::to_string(counter) + ")" + extension_name.string());
            ++counter;
        }
        
        std::filesystem::copy(main_file, new_file);

        std::cout << "copy complete: " << new_file << std::endl;
    }    
}

const void delete_fragments(std::vector<std::filesystem::path> &paths) {
    for(auto &p: paths) {
        if(!std::filesystem::exists(p))
            return;        

        if(std::filesystem::remove(p))
            std::cout << "delete fragments complete" << std::endl;
    }
}

const void delete_audio(std::filesystem::path& path) {
    if(!std::filesystem::exists(path))
        return;

    if(std::filesystem::remove(path))
        std::cout << "delete audio complete" << std::endl;
}

void m123() {
    std::filesystem::path inputFile = "C:/tools/audio/s1.wav";

    copy_file_audio(inputFile);

    std::filesystem::path path_audio = "../temp/audio/s1.wav";

    adt::AudioTools at;
    at.split_audio(path_audio);

    auto paths = at.get_cut_result();

    for(auto &p: paths) {
        std::cout << p.string() << std::endl;
    }

    delete_audio(path_audio);

    {
        adt::Compress compressor(paths, 10);
        compressor.run();
    }

    delete_fragments(paths);

    std::vector<std::filesystem::path> compressed_paths;
    compressed_paths.resize(paths.size());
    
    for(int i = 0; i < compressed_paths.size(); ++i) {
        compressed_paths[i] = paths[i].parent_path().string() + "/compressed_" + paths[i].filename().string();
    }

    for(auto &cp: compressed_paths) {
        std::cout << cp.string() << std::endl;
    }

    std::filesystem::path compressed_audio = path_audio.parent_path().string() + "/compressed_" + path_audio.filename().string();
    at.concat_audio(compressed_paths, compressed_audio);

    delete_fragments(compressed_paths);
}

#include "adt-paths.hpp"

#include <mutex>
#include <stdexcept>
#include <string>

namespace adt {
    Paths::Paths() {
        // dir paths       
        registerPath("temp_dir","../temp"); // Path to temp dir for temporary files, which creates as run program
        registerPath("fragments_dir","../temp/fragment"); // Path to subdirectory of temp
        registerPath("audio_dir","../temp/audio"); // Path to subdirectory of temp
        registerPath("results","../temp/results"); // Path to ended output dir
        // dir paths

        // assets paths
        registerPath("icon_load_file", "../assets/load-file.png"); // includes in file adt-inteface-top-line 32px
        registerPath("icon_load_group_files", "../assets/load-group-files.png"); // includes in file adt-inteface-top-line 32px
        registerPath("icon_output_folder", "../assets/output-folder.png"); // includes in file adt-inteface-top-line 32px
        registerPath("icon_compress", "../assets/compress.png"); // includes in file adt-inteface-top-line 32px
        registerPath("icon_delete", "../assets/delete-all-files.png"); // includes in file adt-inteface-top-line 32px
        registerPath("icon_settings", "../assets/settings.png"); // includes in file adt-inteface-top-line 32px
        registerPath("icon_audio", "../assets/audio.png"); // includes in file adt-inteface-left-line 64px
        registerPath("recycle_bin", "../assets/recycle-bin.png"); // includes in file adt-inteface-left-line 24px
        registerPath("open_file", "../assets/open-file-location.png"); // includes in file adt-inteface-left-line 24px
        registerPath("icon_audio_ai", "../assets/audio-ai.png"); // includes in file adt-inteface-right-line 64px
        // assets paths
    }

    Paths &Paths::getInstance() {
        static Paths isntance;
        return isntance;
    }

    std::string_view Paths::GetPath(const std::string &name) {
        std::lock_guard<std::mutex> lock(mutex_);

        auto it = named_paths.find(name);
        if(it != named_paths.end()) {
            return it->second;
        } else {
            throw std::runtime_error("Path not found: " + name);
        }
    }

    void Paths::registerPath(const std::string& name, const std::string& path) {
        std::lock_guard<std::mutex> lock(mutex_);

        if(path_pool.insert(path).second) {
            named_paths[name] = std::string_view(path_pool.find(path)->c_str());
        }
    }

    void Paths::addTempPath(const std::string& path) {
        std::lock_guard<std::mutex> lock(mutex_);
        temp_pool[std::to_string(id)] = path;
        ++id;
    }

    std::string Paths::getTempPath(const std::string& id) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = temp_pool.find(id);
        if(it != temp_pool.end()) {
            return it->second;
        } else {
            throw std::runtime_error("Temp path not found" + id);
        }
    }

    void Paths::removeTempPath(const std::string& id) {
        std::lock_guard<std::mutex> lock(mutex_);
        temp_pool.erase(id);
    }

    std::size_t Paths::GetSizeTempPool() {
        std::lock_guard<std::mutex> lock(mutex_);
        return temp_pool.size();
    }

    void Paths::clearTempPath() {
        std::lock_guard<std::mutex> lock(mutex_);
        temp_pool.clear();
        id = 0;
    }
}
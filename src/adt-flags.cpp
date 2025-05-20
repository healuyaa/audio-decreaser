#include "adt-flags.hpp"
#include <mutex>

namespace adt {
    Flags &Flags::getInstance() {
        static Flags instance;
        return instance;
    }

    //methods for set/get bool flags from FInterface
    void Flags::SetLoadTopMenu(bool b) {
        finterface.is_load_top_menu = b;
    }

    bool Flags::GetLoadTopMenu() {
        return finterface.is_load_top_menu;
    }

    void Flags::SetLoadRightLine(bool b) {
        std::lock_guard<std::mutex> lock(mutex_);
        finterface.is_load_right_line = b;
    }

    bool Flags::GetLoadRightLine() {
        std::lock_guard<std::mutex> lock(mutex_);
        return finterface.is_load_right_line;
    }
    //methods for set/get bool flags from FInterface


    //methods for set/get bool flags from FTopline
    void Flags::SetFileDialogOpen(bool b) {
        std::lock_guard<std::mutex> lock(mutex_);
        ftopline.is_open_file_dialog = b;
    }

    bool Flags::GetFileDialogOpen() {
        std::lock_guard<std::mutex> lock(mutex_);
        return ftopline.is_open_file_dialog;
    }
    
    void Flags::SetFolderDialog(bool b) {
        std::lock_guard<std::mutex> lock(mutex_);
        ftopline.is_open_folder_dialog = b;
    }

    bool Flags::GetFolderDialog() {
        std::lock_guard<std::mutex> lock(mutex_);
        return ftopline.is_open_folder_dialog;
    }

    void Flags::SetOpenOutput(bool b) {
        std::lock_guard<std::mutex> lock(mutex_);
        ftopline.is_open_output = b;
    }

    bool Flags::GetOpenOutput() {
        std::lock_guard<std::mutex> lock(mutex_);
        return ftopline.is_open_output;
    }

    void Flags::SetCompress(bool b) {
        std::lock_guard<std::mutex> lock(mutex_);
        ftopline.is_compress = b;
    }

    bool Flags::GetCompress() {
        std::lock_guard<std::mutex> lock(mutex_);
        return ftopline.is_compress;
    }

    void Flags::SetGlobalDelete(bool b) {
        std::lock_guard<std::mutex> lock(mutex_);
        ftopline.is_global_delete = b;
    }

    bool Flags::GetGlobalDelete() {
        return ftopline.is_global_delete;
    }

    void Flags::SetSettings(bool b) {
        std::lock_guard<std::mutex> lock(mutex_);
        ftopline.is_setting = b;
    }

    bool Flags::GetSettings() {
        std::lock_guard<std::mutex> lock(mutex_);
        return ftopline.is_setting;
    }
    //methods for set/get bool flags from FTopline

    //methods for set/get bool flags from FTopline
    void Flags::SetHqModel(bool b) {
        std::lock_guard<std::mutex> lock(mutex_);
        faudiocompress.is_hq_model = b;
    }

    bool Flags::GetHqModel() {
        std::lock_guard<std::mutex> lock(mutex_);
        return faudiocompress.is_hq_model;
    }

    void Flags::SetCountThreads(int count) {
        faudiocompress.count_threads = count;
    }

    int Flags::GetCountThreads() {
        return faudiocompress.count_threads;
    }
    //methods for set/get bool flags from FTopline

    //methods for set/get bool flags from FTopline
    void Flags::SetIsCountChangeThreads(bool b) {
        faudiorunner.is_change_count_threads = b; 
    }

    bool Flags::GetIsCountChangeThreads() {
        return faudiorunner.is_change_count_threads;
    }
    //methods for set/get bool flags from FTopline
}
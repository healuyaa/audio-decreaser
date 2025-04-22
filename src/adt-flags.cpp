#include "adt-flags.hpp"

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
    //methods for set/get bool flags from FInterface


    //methods for set/get bool flags from FTopline
    void Flags::SetFileDialogOpen(bool b) {
        ftopline.is_open_file_dialog = b;
    }

    bool Flags::GetFileDialogOpen() {
        return ftopline.is_open_file_dialog;
    }
    
    void Flags::SetFileDialogOpenDir(bool b) {
        ftopline.is_open_file_dialog_dir = b;
    }

    bool Flags::GetFileDialogOpenDir() {
        return ftopline.is_open_file_dialog_dir;
    }

    void Flags::SetOpenOutput(bool b) {
        ftopline.is_open_output = b;
    }

    bool Flags::GetOpenOutput() {
        return ftopline.is_open_output;
    }

    void Flags::SetCompress(bool b) {
        ftopline.is_compress = b;
    }

    bool Flags::GetCompress() {
        return ftopline.is_compress;
    }

    void Flags::SetSettings(bool b) {
        ftopline.is_setting = b;
    }

    bool Flags::GetSettings() {
        return ftopline.is_setting;
    }
    //methods for set/get bool flags from FTopline
}
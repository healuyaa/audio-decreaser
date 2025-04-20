#include "adt-flags.hpp"

namespace adt {
    Flags &Flags::getInstance() {
        static Flags instance;
        return instance;
    }

    void Flags::SetLoadTopMenu(bool b) {
        finterface.is_load_top_menu = b;
    }

    bool Flags::GetLoadTopMenu() {
        return finterface.is_load_top_menu;
    }

    void Flags::SetFileDialogOpen(bool b) {
        finterface.is_file_dialog_open = b;
    }

    bool Flags::GetFileDialogOpen() {
        return finterface.is_file_dialog_open;
    }
}
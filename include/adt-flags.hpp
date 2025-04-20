#pragma once

#include <mutex>

namespace adt {
    class Flags {
        public:
        Flags() = default;
        ~Flags() = default;

        static Flags &getInstance();

        //methods for set/get bool flags from FInterface
        void SetLoadTopMenu(bool);
        bool GetLoadTopMenu();

        void SetFileDialogOpen(bool);
        bool GetFileDialogOpen();

        private:
        // struct of all bool flags which uses in adt-interface.hpp
        struct FInterface {
          bool is_load_top_menu = false;   
          bool is_file_dialog_open = false;       
        };

        // struct of all bool flags which uses in adt-interface-top-line.hpp
        struct FTopLine {
            bool is_open_file_dialog = false;
            bool is_open_output = false;
            bool is_compress = false;
            bool is_setting = false;
        };
        
        Flags(const Flags&) = delete;
        Flags& operator=(const Flags&) = delete;  


        FInterface finterface;
        FTopLine ftopline;

        mutable std::mutex mutex_;
    };    
}
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
        //methods for set/get bool flags from FInterface

        //methods for set/get bool flags from FTopline
        void SetFileDialogOpen(bool);
        bool GetFileDialogOpen();

        void SetFileDialogOpenDir(bool);
        bool GetFileDialogOpenDir();

        void SetOpenOutput(bool);
        bool GetOpenOutput();

        void SetCompress(bool);
        bool GetCompress();

        void SetSettings(bool);
        bool GetSettings();
        //methods for set/get bool flags from FTopline

        private:

        Flags(const Flags&) = delete;
        Flags& operator=(const Flags&) = delete; 
        

        // struct of all bool flags which uses in adt-interface.hpp
        struct FInterface {
          bool is_load_top_menu = false;
        };

        // struct of all bool flags which uses in adt-interface-top-line.hpp
        struct FTopLine {
            bool is_open_file_dialog = false;
            bool is_open_file_dialog_dir = false;
            bool is_open_output = false;
            bool is_compress = false;
            bool is_setting = false;
        }; 


        FInterface finterface;
        FTopLine ftopline;

        mutable std::mutex mutex_;
    };    
}
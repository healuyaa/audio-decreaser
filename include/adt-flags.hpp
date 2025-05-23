#pragma once

#include <mutex>
#include <thread>

namespace adt {
    class Flags {
        public:
        Flags() = default;
        ~Flags() = default;

        static Flags& getInstance();

        //methods for set/get bool flags from FInterface
        void SetLoadTopMenu(bool);
        bool GetLoadTopMenu();

        void SetLoadRightLine(bool);
        bool GetLoadRightLine();
        //methods for set/get bool flags from FInterface

        //methods for set/get bool flags from FTopline
        void SetFileDialogOpen(bool);
        bool GetFileDialogOpen();

        void SetFolderDialog(bool);
        bool GetFolderDialog();

        void SetOpenOutput(bool);
        bool GetOpenOutput();

        void SetCompress(bool);
        bool GetCompress();

        void SetGlobalDelete(bool);
        bool GetGlobalDelete();

        void SetSettings(bool);
        bool GetSettings();
        //methods for set/get bool flags from FTopline

        //methods for set/get bool flags from FLLine
        void SetLIsDelete(bool);
        bool GetLIsDelete();
        //methods for set/get bool flags from FLLine

        //methods for set/get bool flags from FRLine
        void SetRIsDelete(bool);
        bool GetRIsDelete();
        //methods for set/get bool flags from FRLine

        //methods for set/get bool flags from FAudioCompress
        void SetHqModel(bool);
        bool GetHqModel();

        void SetCountThreads(int);
        int GetCountThreads();
        //methods for set/get bool flags from FAudioCompress

        //methods for set/get bool flags from FAudioRunner
        void SetIsCountChangeThreads(bool);
        bool GetIsCountChangeThreads();
        //methods for set/get bool flags from FAudioRunner

        //methods for set/get bool flags from FInterfaceComfirm
        void SetIsConfirm(bool);
        bool GetIsConfirm();
        //methods for set/get bool flags from FInterfaceComfirm

        private:
        Flags(const Flags&) = delete;
        Flags& operator=(const Flags&) = delete; 
        

        // struct of all bool flags which uses in adt-interface.hpp && adt-interface.cpp
        struct FInterface {
            bool is_load_top_menu = false;
            bool is_load_right_line = false;
        };

        // struct of all bool flags which uses in adt-interface-top-line.hpp && adt-interface-top-line.cpp
        struct FTopLine {
            bool is_open_file_dialog = false;
            bool is_open_folder_dialog = false;
            bool is_open_output = false;
            bool is_compress = false;
            bool is_global_delete = false;
            bool is_setting = false;
        };

        // struct of all bool flags which uses in adt-interface-left-line.hpp && adt-adt-interface-left-line.cpp
        struct FLLine {
            bool is_delete;
        };

        // struct of all bool flags which uses in adt-interface-right-line.hpp && adt-adt-interface-right-line.cpp
        struct FRLine {
            bool is_delete;
        };
        
        // struct of all bool/int flags which uses in adt-audio-compress.hpp && adt-audio-compress.cpp
        struct FAudioCompress {
            bool is_hq_model = false;
            int count_threads = std::thread::hardware_concurrency() / 2;
        };

        // struct of all bool flags which uses in adt-audio-run.hpp && adt-audio-run.cpp
        struct FAudioRunner {
            bool is_change_count_threads = false;
        };

        // struct of all bool flags which uses in adt-audio-run.hpp && adt-audio-run.cpp
        struct FInterfaceComfirm {
            bool is_confirm = false;
        };


        FInterface finterface;
        FTopLine ftopline;
        FAudioCompress faudiocompress;
        FAudioRunner faudiorunner;
        FInterfaceComfirm finterfaceconfirm;
        FLLine flline;
        FRLine frline;

        mutable std::mutex mutex_;
    };    
}
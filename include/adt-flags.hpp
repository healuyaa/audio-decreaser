#pragma once

#include <atomic>
#include <memory>

namespace adt {
    class Flags {
        private:
        // struct of all bool flags which uses in adt-interface.hpp
        struct FInterface {
            bool is_load_top_menu;

            bool is_playing_audio;

            bool is_delete_all;
            bool is_delete_current;

            bool is_file_existed;

            std::atomic<bool> is_file_dialog_open;

            FInterface()
                : is_load_top_menu(false), is_playing_audio(false), is_delete_all(false),
                  is_delete_current(false), is_file_existed(false), is_file_dialog_open(false) {}
        };

        // struct of all bool flags which uses in adt-audio.hpp
        struct FAudio {
            bool is_playing;
            bool is_pause;
            bool is_stop;

            FAudio() : is_playing(false), is_pause(false), is_stop(false) {}
        };

        // struct of all bool flags which uses in adt-interface-top-line.hpp
        struct FTopLine {
            std::atomic<bool> is_select_file;
            bool is_open_output;
            bool is_compress;
            bool is_setting;

            FTopLine() : is_select_file(false), is_open_output(false), is_compress(false), is_setting(false) {}
        };

        public:
        Flags();
        ~Flags() = default;

        std::unique_ptr<FInterface> finterface;
        std::unique_ptr<FAudio> faudio;
        std::unique_ptr<FTopLine> ftopline;
    };    
}

// class Flags {
//     public:
//         struct FInterface {
//             std::atomic<bool> is_load_top_menu{false};
//             std::atomic<bool> is_playing_audio{false};
//         };
    
//         struct FAudio {
//             std::atomic<bool> is_playing{false};
//             std::atomic<bool> is_paused{false};
//         };
    
//         struct FTopLine {
//             std::atomic<bool> is_select_file{false};
//         };
    
//         FInterface finterface;
//         FAudio faudio;
//         FTopLine ftopline;
    
//         static Flags& getInstance() {
//             static Flags instance;
//             return instance;
//         }
    
//     private:
//         Flags() = default;
//         ~Flags() = default;
//         Flags(const Flags&) = delete;
//         Flags& operator=(const Flags&) = delete;
//     };

//Flags::getInstance().faudio.is_playing = true;
    
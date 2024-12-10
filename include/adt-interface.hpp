#pragma once

#include "adt-audio.hpp"
#include "adt-interface-left-line.hpp"
#include "adt-interface-right-line.hpp"
#include "adt-file-tools.hpp"
#include "adt-interface-top-line.hpp"

#include <atomic>
#include <future>
#include <memory>
#include <vector>

namespace adt {
    class Interface {
        public:
        Interface() = default;
        ~Interface() = default;

        void UInterface();

        std::filesystem::path GetOutDir();

        private:
        std::vector<std::unique_ptr<Lline>> Llines;
        std::vector<std::unique_ptr<Rline>> Rlines;
        std::vector<std::unique_ptr<Tline>> Tlines;

        Audio audio_player;
        bool is_playing = false;        

        float current_time = 0.0f;
        float length = 0.0f;

        bool load_top = false;

        int is_selected_left = -1;
        int is_selected_right = -1;

        bool delete_all = false;
        bool delete_current = false;

        std::atomic<bool> is_file_dialog_open = false;
        std::future<void> future;
        std::mutex path_mutex;
        std::filesystem::path path_to_copy;
        bool file_existed = false;

        std::vector<std::filesystem::path> paths;
        std::vector<std::filesystem::path> paths_out;
        std::filesystem::path name_out_dir = "../ADOutput";

        FileTools tools{};
        
        void TopBar();
        void TopSection(float width = 0.0f, float height = 0.0f);
        void MidSection(float width = 0.0f, float height = 0.0f);
        void BotSection(float width = 0.0f, float height = 0.0f);

        void LeftSection(float width = 0.0f, float height = 0.0f);
        void RightSection(float width = 0.0f, float height = 0.0f);

        void OpenFileDialog();

        void IsShouldDelteSMTH();
    };    
}
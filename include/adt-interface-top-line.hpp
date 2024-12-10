#pragma once

#include "adt-interface-Iline.hpp"
#include "adt-model.hpp"

#include <atomic>
#include <filesystem>
#include <future>
#include <string>
#include <unordered_map>

namespace adt {
    class Tline : ILine {
        public:
        Tline();
        ~Tline() override = default;

        void lineUI(const std::string& name) override;
        bool IsShouldRun();
        void SetShouldRun();
        bool GetProgress();
        bool GetCanView();

        void SetDeleteCurrent(bool);
        bool GetDeleteCurrent();
        void SetDeleteAll(bool);
        bool GetDeleteAll();

        void SetPaths(std::filesystem::path name_output_dir, std::filesystem::path path_to_file);

        private:
        const std::filesystem::path icon_file_delete = "../assets/icon-file-delete.png"; // 32px
        const std::filesystem::path icon_folder_show = "../assets/icon-folder-show.png"; // 32px
        const std::filesystem::path icon_folder_hide = "../assets/icon-folder-hide.png"; // 32px
        const std::filesystem::path icon_folder_pick = "../assets/icon-folder-pick.png"; // 32px

        std::atomic<bool> is_file_dialog_open = false;
        std::future<void> file_dialog_future;

        std::filesystem::path name_output_dir;
        std::filesystem::path path_to_file;
        Model model;

        bool is_should_run = true;
        bool is_in_progress = false;
        bool is_can_view = false;

        bool is_delete_current = false;
        bool is_delete_all = false;

        std::unordered_map<std::string, std::unique_ptr<Icon>> icons;

        std::unordered_map<std::string, bool> is_hovered;

        void loadIcon() override;
        Icon* getIcon(const std::string) override;

        void IsHoveredIcon(Icon* current, Icon* hovered);
        void IsHoveredTopitem(bool is_hovered);
        void initIsHovered();
    };
}
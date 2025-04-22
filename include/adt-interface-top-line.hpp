#pragma once

#include "adt-interface-Iline.hpp"

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

        private:
        std::unordered_map<std::string, std::unique_ptr<Icon>> icons;
        std::unordered_map<std::string, bool> is_hovered;

        void loadIcon() override;
        Icon* getIcon(const std::string) override;

        void IsHoveredTopitem(bool is_hovered);
        void initIsHovered();

        std::future<void> future;
        std::mutex mutex_;
        int id = 0;

        void OpenFileDialog();
    };
}
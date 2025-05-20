#pragma once

#include "adt-file-dialog.hpp"
#include "adt-interface-Iline.hpp"
#include "adt-interface-settings.hpp"

#include <memory>
#include <string>
#include <unordered_map>

namespace adt {
    class Tline : ILine {
        public:
        Tline();
        ~Tline() override = default;

        void lineUI(const std::string& name, std::atomic<float>* progress);
        bool IsShouldRun();

        private:
        std::unordered_map<std::string, std::unique_ptr<Icon>> icons;
        std::unordered_map<std::string, bool> is_hovered;

        void loadIcon() override;
        Icon* getIcon(const std::string) override;

        void IsHoveredTopitem(bool is_hovered);
        void initIsHovered();

        std::shared_ptr<adt::ADTFileDialog> dialog;
        std::shared_ptr<adt::Settigns> settings;
    };
}
#pragma once

#include "adt-file-dialog.hpp"
#include "adt-interface-Iline.hpp"

#include <memory>
#include <unordered_map>

namespace adt {
    class Lline : ILine {
        public:
        Lline();
        ~Lline() override = default;

        void lineUI(const std::string& name);

        private:
        std::unordered_map<std::string, std::unique_ptr<Icon>> icons;
        std::shared_ptr<adt::ADTFileDialog> dialog;

        void loadIcon() override;
        Icon* getIcon(const std::string) override;
    };
}
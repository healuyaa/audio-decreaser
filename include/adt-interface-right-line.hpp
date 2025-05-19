#pragma once

#include "adt-file-dialog.hpp"
#include "adt-interface-Iline.hpp"

#include <memory>
#include <unordered_map>

namespace adt {
    struct RLineState {
        std::string path;
        bool is_ready = false;
        bool was_checked = false;
    };
    
    class Rline : ILine {
        public:
        Rline();
        ~Rline() override = default;

        void lineUI(const std::string& name, bool status, std::atomic<float>* progress);

        private:
        std::unordered_map<std::string, std::unique_ptr<Icon>> icons;
        std::shared_ptr<adt::ADTFileDialog> dialog;

        void loadIcon() override;
        Icon* getIcon(const std::string) override;
    };
}
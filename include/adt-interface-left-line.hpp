#pragma once

#include "adt-interface-Iline.hpp"

#include <filesystem>
#include <memory>
#include <unordered_map>

namespace adt {
    class Lline : ILine {
        public:
        Lline();
        ~Lline() override = default;

        void lineUI(const std::string& name) override;

        private:
        const std::filesystem::path icon_audio = "../assets/icon-file-audio.png"; // 64px

        std::unordered_map<std::string, std::unique_ptr<Icon>> icons;

        void loadIcon() override;
        Icon* getIcon(const std::string) override;
    };
}
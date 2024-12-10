#pragma once

#include "adt-interface-Iline.hpp"

#include <filesystem>
#include <unordered_map>

namespace adt {
    class Rline : ILine{
        public:
        Rline();
        ~Rline() override = default;

        void lineUI(const std::string& name) override;

        private:
        const std::filesystem::path icon_audio_ai = "../assets/icon-file-audio-ai.png"; // 64px  

        std::unordered_map<std::string, std::unique_ptr<Icon>> icons;

        void loadIcon() override;
        Icon* getIcon(const std::string) override;
    };
}
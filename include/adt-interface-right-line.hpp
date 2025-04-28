#pragma once

#include "adt-interface-Iline.hpp"

#include <unordered_map>

namespace adt {
    class Rline : ILine{
        public:
        Rline();
        ~Rline() override = default;

        void lineUI(const std::string& name) override;

        private:
        std::unordered_map<std::string, std::unique_ptr<Icon>> icons;

        void loadIcon() override;
        Icon* getIcon(const std::string) override;
    };
}
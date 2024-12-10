#pragma once

#include "adt-icon.hpp"

namespace adt {
    class ILine {
        public:
        virtual ~ILine() = default;

        virtual void lineUI(const std::string& name) = 0;

        private:
        virtual void loadIcon() = 0;
        virtual Icon* getIcon(const std::string) = 0;
    };
}
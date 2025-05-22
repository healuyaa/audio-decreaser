#pragma once

#include <string>

namespace adt {
    class ConfirmWindow {
        public:
        ConfirmWindow() = default;
        ~ConfirmWindow() = default;

        void runConfirm();
        void runConfirm(const std::string& name);

        private:
    };
}
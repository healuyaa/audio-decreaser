#pragma once

#include <string>

namespace adt {
    class ConfirmWindow {
        public:
        ConfirmWindow();
        ~ConfirmWindow();

        void runUI(const std::string& name);

        private:
    };
}
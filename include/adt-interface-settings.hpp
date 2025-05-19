#pragma once

namespace adt {
    class Settigns {
        public:
        Settigns() = default;
        ~Settigns() = default;

        void runUI();

        private:
        int count_threads;
    };
}
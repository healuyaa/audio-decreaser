#pragma once
namespace adt {
    class Settigns {
        public:
        Settigns();
        ~Settigns() = default;

        void runUI();

        int count_threads;
        int hq_model;

        private:
        void apply();
    };
}
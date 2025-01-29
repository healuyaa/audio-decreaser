#pragma once

#include "adt-service.hpp"
#include "adt-window.hpp"

namespace adt {
    class App {
        public:
        static constexpr int WIDTH = 1200;
        static constexpr int HEIGHT = 800;

        void run();

        private:
        Window window{WIDTH, HEIGHT, "audio-decreaser", &service};
        Service service;
    };
}
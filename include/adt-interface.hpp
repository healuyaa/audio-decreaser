#pragma once

#include "adt-interface-left-line.hpp"
#include "adt-interface-right-line.hpp"
#include "adt-interface-top-line.hpp"
#include "adt-service.hpp"

#include <memory>
#include <vector>

namespace adt {
    class Interface {
        public:
        Interface(Service *service);
        ~Interface() = default;

        void UInterface();

        private:
        std::vector<std::unique_ptr<Lline>> Llines;
        std::vector<std::unique_ptr<Rline>> Rlines;
        std::unique_ptr<Tline> tline;

        Service* service;

        float current_time = 0.0f;
        float length = 0.0f;

        int is_selected_left = -1;
        int is_selected_right = -1;

        std::vector<std::filesystem::path> paths;
        std::vector<std::filesystem::path> paths_out;
        
        void TopBar();
        void TopSection(float width = 0.0f, float height = 0.0f);
        void MidSection(float width = 0.0f, float height = 0.0f);

        void LeftSection(float width = 0.0f, float height = 0.0f);
        void RightSection(float width = 0.0f, float height = 0.0f);        
    };    
}
#include "adt-interface-settings.hpp"
#include "adt-flags.hpp"

#include "imgui.h"

namespace adt {
    Settigns::Settigns() {
        count_threads = std::thread::hardware_concurrency() / 2;
        hq_model = 0;
    }

    void Settigns::runUI() {        
        bool settings_status = true;
         
        if(ImGui::BeginPopupModal("Settings", &settings_status, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Compression Settings");

            ImGui::SeparatorText("Model");
            ImGui::RadioButton("Base Model", &hq_model, 0);
            ImGui::SameLine();
            ImGui::RadioButton("High Quality Model", &hq_model, 1);

            ImGui::SeparatorText("Threads");
            int max_threads = std::thread::hardware_concurrency();
            ImGui::SliderInt("Thread Count", &count_threads, 1, max_threads);
            ImGui::Text("Available: %u", max_threads);

            ImGui::Separator();
            if (ImGui::Button("OK")) {
                Flags::getInstance().SetSettings(false);

                apply();

                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel")) {
                Flags::getInstance().SetSettings(false);

                count_threads = std::thread::hardware_concurrency() / 2;
                hq_model = 0;

                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        if(!settings_status) {
            Flags::getInstance().SetSettings(false);
        }        
    }

    void Settigns::apply() {
        if(count_threads != (std::thread::hardware_concurrency() / 2)) {
            Flags::getInstance().SetIsCountChangeThreads(true);
            Flags::getInstance().SetCountThreads(count_threads);
        }            

        Flags::getInstance().SetHqModel(hq_model);
    }
}
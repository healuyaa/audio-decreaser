#include "adt-interface-confirm.hpp"
#include "adt-flags.hpp"
#include "imgui.h"

namespace adt {
    void ConfirmWindow::runConfirm() {
        bool status_confirm = true;

        if(ImGui::BeginPopupModal("##confirm window", &status_confirm, ImGuiWindowFlags_AlwaysAutoResize)) {
            std::string text = "Are u sure to delete all?";
            ImGui::TextUnformatted(text.c_str());

            ImGui::Separator();
            if (ImGui::Button("Yes")) {
                Flags::getInstance().SetIsConfirm(false);
                Flags::getInstance().SetGlobalDelete(true);

                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel")) {
                Flags::getInstance().SetIsConfirm(false);

                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        if(!status_confirm) {
            Flags::getInstance().SetIsConfirm(false);
        }
    }

    void ConfirmWindow::runConfirm(const std::string& name) {
        bool status_confirm = true;

        if(ImGui::BeginPopupModal("##confirm window", &status_confirm, ImGuiWindowFlags_AlwaysAutoResize)) {
            std::string text = "Are u sure to delete?";
            ImGui::TextUnformatted(text.c_str());
            ImGui::TextUnformatted(name.c_str());

            ImGui::Separator();
            if (ImGui::Button("OK")) {
                Flags::getInstance().SetIsConfirm(false);

                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Cancel")) {
                Flags::getInstance().SetIsConfirm(false);

                ImGui::CloseCurrentPopup();
            }
        }

        if(!status_confirm) {
            Flags::getInstance().SetIsConfirm(false);
        }
    }
}
#include "adt-interface.hpp"

#include "adt-file-tools.hpp"
#include "adt-flags.hpp"
#include "adt-interface-top-line.hpp"
#include "adt-paths.hpp"
#include "adt-service.hpp"
#include "imgui.h"

#include <cstddef>
#include <windows.h>
#include <commdlg.h>
#include <filesystem>
#include <memory>
#include <string>

namespace adt {
    Interface::Interface(Service* service) : service(service) {
        FileTools::getInstance().initDirs();
    }

    void Interface::UInterface() {
        ImGui::Begin("##top tab", NULL);

        ImVec2 window_size = ImGui::GetContentRegionAvail();

        const float top_section_height = 66.0f; // size top bar
        float mid_section_height = window_size.y * 0.89f - ImGui::GetStyle().ItemSpacing.y; // 89

        TopSection(window_size.x, top_section_height);
        MidSection(window_size.x, mid_section_height);

        ImGui::End();
    }

    void Interface::TopSection(float width, float height) {
        ImGui::BeginChild("TopSection", ImVec2(0, height), true);

        if(!Flags::getInstance().GetLoadTopMenu()) {
            Flags::getInstance().SetLoadTopMenu(true);
            auto tline = std::make_unique<Tline>();
            this->tline = std::move(tline);
        }
        
        tline->lineUI("name top");

        ImGui::EndChild();
    }

    void Interface::MidSection(float width, float height) {
        ImGui::BeginChild("MidSection", ImVec2(0, height), false);

        float leftPanelWidth = width * 0.3f;
        float rightPanelWidth = width - leftPanelWidth - ImGui::GetStyle().ItemSpacing.x;

        LeftSection(leftPanelWidth, 0);

        ImGui::SameLine();

        RightSection(rightPanelWidth, 0);

        ImGui::EndChild();
    }

    void Interface::LeftSection(float width, float height) {
        ImGui::BeginChild("LeftSection", ImVec2(width, height), true);
        ImGui::BeginGroup();

        if(Paths::getInstance().GetSizeTempPool() != Llines.size()) {
            auto lline = std::make_unique<Lline>();
            Llines.push_back(std::move(lline));
        }

        for(int i = 0; i < Llines.size(); ++i) {
            std::string child_id = "##child_id_l" + std::to_string(i);

            bool is_hovered = false;

            ImVec4 default_bg = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
            ImVec4 hover_bg   = ImVec4(0.1f, 0.1f, 0.1f, 0.15f);

            ImGui::PushStyleColor(ImGuiCol_ChildBg, default_bg);

            if (ImGui::BeginChild(child_id.c_str(), ImVec2(0, 64.0f))) { // 80.0f size + spacing
                is_hovered = ImGui::IsWindowHovered();
    
                if (is_hovered) {
                    ImDrawList* draw = ImGui::GetWindowDrawList();
                    ImVec2 min = ImGui::GetWindowPos();
                    ImVec2 max = ImVec2(min.x + ImGui::GetWindowSize().x, min.y + ImGui::GetWindowSize().y);
                    draw->AddRectFilled(min, max, ImColor(hover_bg));
                }

                Llines[i]->lineUI(Paths::getInstance().getTempPath(std::to_string(i)));
            }

            ImGui::PopStyleColor();
            ImGui::EndChild();            
        }

        ImGui::EndGroup();
        ImGui::EndChild();
    }

    void Interface::RightSection(float width, float height) {
        ImGui::BeginChild("RightPanel", ImVec2(width, height), true);
        ImGui::BeginGroup();

        if(Rlines.size() < Llines.size() && Flags::getInstance().GetLoadRightLine()) {
            auto rline = std::make_unique<Rline>();
            Rlines.push_back(std::move(rline));
        }

        for(int i = 0; i < Rlines.size(); ++i) {
            std::string child_id = "##child_id_r" + std::to_string(i);

            if(is_selected_right == i) {
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.1f));
            } else {
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
            }

            if(ImGui::BeginChild(child_id.c_str(), ImVec2(0, 100.0f))) {
                if(ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                    if(is_selected_right == i) {
                        is_selected_right = -1;
                    } else {
                        is_selected_right = i;
                    }                    
                }

                if(is_selected_right != -1 && is_selected_left != -1) {
                    is_selected_left = -1;
                }

                std::filesystem::path filename(Paths::getInstance().getTempPath(std::to_string(i)));
                std::filesystem::path r_path = Paths::getInstance().GetPath("results") / filename.filename();

                if(!std::filesystem::exists(r_path))
                    return;

                Rlines[i]->lineUI(r_path.string());
            }

            ImGui::PopStyleColor();
            ImGui::EndChild();
        }

        ImGui::EndGroup();
        ImGui::EndChild();
    }
}
#include "adt-interface-right-line.hpp"

#include "adt-paths.hpp"
#include "imgui.h"
#include <iostream>

namespace adt {
    Rline::Rline() {
        loadIcon();
    }

    void Rline::lineUI(const std::string& name) {
        const ImVec2 icon_file_size(64.0f, 64.0f);
        const ImVec2 icon_button_size(24.0f, 24.0f);
        const float spacing = ImGui::GetStyle().ItemSpacing.x;
        
        ImGui::BeginGroup();
        {
            ImGui::Image((void*)(intptr_t)getIcon("audio-ai")->GetID(), icon_file_size);
        }
        ImGui::EndGroup();

        ImGui::SameLine();

        ImGui::BeginGroup();
        {
            ImGui::TextWrapped("%s", name.c_str());
    
            // uintmax_t size = std::filesystem::file_size(name.c_str());
            // double size_in_mb = static_cast<double>(size) / (1024 * 1024);

            // std::ostringstream oss;
            // oss << std::fixed << std::setprecision(1) << size_in_mb << " MB";
            // ImGui::Text("%s", oss.str().c_str());
        }
        ImGui::EndGroup();
        
        ImGui::SameLine();

        ImGui::BeginGroup();
        {
            float group_start_y = ImGui::GetCursorScreenPos().y;
            float content_region_width = ImGui::GetContentRegionAvail().x;
            float icon_x = ImGui::GetCursorScreenPos().x + content_region_width - icon_button_size.x;

            ImDrawList* draw = ImGui::GetWindowDrawList();

            ImVec2 icon_pos_folder = ImVec2(icon_x, group_start_y);
            ImGui::SetCursorScreenPos(icon_pos_folder);

            ImGui::InvisibleButton("##open_file", icon_button_size);

            if (ImGui::IsItemHovered()) {
                draw->AddRectFilled(
                    icon_pos_folder,
                    ImVec2(icon_pos_folder.x + icon_button_size.x, icon_pos_folder.y + icon_button_size.y),
                    IM_COL32(100, 100, 255, 50), // blue
                    4.0f
                );
            }

            draw->AddImage(
                (void*)(intptr_t)getIcon("open_file")->GetID(),
                icon_pos_folder,
                ImVec2(icon_pos_folder.x + icon_button_size.x, icon_pos_folder.y + icon_button_size.y)
            );

            if (ImGui::IsItemClicked()) {
                std::cout << "Clicked open_file" << std::endl;
            }

            ImVec2 icon_pos_bin = ImVec2(icon_x, icon_pos_folder.y + icon_button_size.y + spacing);
            ImGui::SetCursorScreenPos(icon_pos_bin);

            ImGui::InvisibleButton("##recycle_bin", icon_button_size);

            if (ImGui::IsItemHovered()) {
                draw->AddRectFilled(
                    icon_pos_bin,
                    ImVec2(icon_pos_bin.x + icon_button_size.x, icon_pos_bin.y + icon_button_size.y),
                    IM_COL32(255, 100, 100, 50), // red
                    4.0f
                );
            }

            draw->AddImage(
                (void*)(intptr_t)getIcon("recycle_bin")->GetID(),
                icon_pos_bin,
                ImVec2(icon_pos_bin.x + icon_button_size.x, icon_pos_bin.y + icon_button_size.y)
            );

            if (ImGui::IsItemClicked()) {
                std::cout << "Clicked recycle_bin" << std::endl;
            }
        }
        ImGui::EndGroup();
    }

    void Rline::loadIcon() {
        icons["audio-ai"] = std::make_unique<Icon>(Paths::getInstance().GetPath("icon_audio_ai"));
        icons["recycle_bin"] = std::make_unique<Icon>(Paths::getInstance().GetPath("recycle_bin"));
        icons["open_file"] = std::make_unique<Icon>(Paths::getInstance().GetPath("open_file"));
    }

    Icon* Rline::getIcon(const std::string name) {
        auto it = icons.find(name);
        if (it != icons.end()) {
            return it->second.get();
        }
        return nullptr;
    }
}
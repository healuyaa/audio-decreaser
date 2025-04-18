#include "adt-interface-top-line.hpp"

#include "adt-icon.hpp"
#include "adt-model.hpp"
#include "imgui.h"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>

namespace adt {
    Tline::Tline() {
        // loadIcon();
        initIsHovered();
    }

    void Tline::lineUI(const std::string& name) {
        ImVec2 size{160.0f, 50.0f};

        IsHoveredTopitem(is_hovered["select_file"]);
        
        ImGui::BeginChild("##file to compress", size, true);
        {
            // ImGui::Image((void*)(intptr_t) icons["select_file"]->GetID(), ImVec2(32.0f, 32.0f));

            is_hovered["select_file"] = ImGui::IsWindowHovered();

            ImGui::SameLine();

            ImGui::Text("select_file");

            if (is_hovered["select_file"] && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                is_file_dialog_open = true;                
                std::cout << "here select_file" << std::endl;                
            }
        }
        ImGui::PopStyleColor();
        ImGui::EndChild();

        ImGui::SameLine();

        IsHoveredTopitem(is_hovered["open_output"]);
        
        ImGui::BeginChild("##open_output", size, true);
        {
            // ImGui::Image((void*)(intptr_t) icons["open_output"]->GetID(), ImVec2(32.0f, 32.0f));

            is_hovered["open_output"] = ImGui::IsWindowHovered();

            ImGui::SameLine();

            ImGui::Text("open_output");

            if (is_hovered["open_output"] && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                is_file_dialog_open = true;
                file_dialog_future = std::async(std::launch::async, [this] {
                    std::cout << "here open_output" << std::endl;

                    is_file_dialog_open = false;
                });
            }
        }
        ImGui::PopStyleColor();
        ImGui::EndChild();

        ImGui::SameLine();

        IsHoveredTopitem(is_hovered["compress"]);

        ImGui::BeginChild("##compress", size, true);
        {
            is_hovered["compress"] = ImGui::IsWindowHovered();

            // ImGui::Image((void*)(intptr_t) icons["compress"]->GetID(), ImVec2(32.0f, 32.0f));

            ImGui::SameLine();

            ImGui::Text("compress");

            if (is_hovered["compress"] && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                std::cout << "here compress" << std::endl;
            }
        }
        ImGui::PopStyleColor();
        ImGui::EndChild();

        ImGui::SameLine();

        IsHoveredTopitem(is_hovered["settings"]);

        ImGui::BeginChild("##settings", size, true);
        {
            is_hovered["settings"] = ImGui::IsWindowHovered();

            // ImGui::Image((void*)(intptr_t) icons["settings"]->GetID(), ImVec2(32.0f, 32.0f));

            ImGui::SameLine();

            ImGui::Text("settings");

            if (is_hovered["settings"] && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                std::cout << "here settings" << std::endl;                  
            }
        }
        ImGui::PopStyleColor();
        ImGui::EndChild();

        // ImGui::BeginChild("##run", size, true);
        // {
        //     is_hovered["run"] = ImGui::IsWindowHovered();

        //     ImGui::Text("run");

        //     if (is_hovered["run"] && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        //         is_should_run = false;
        //         is_in_progress = false;
        //         std::cout << "clicked run" << std::endl;
        //     }

        //     if(!is_should_run && !is_in_progress) {
        //         is_in_progress = true;
        //         if(!path_to_file.empty()) {
        //             model.SetPaths(name_output_dir, path_to_file);
        //             model.run();
        //         }
        //     }

        //     if(is_in_progress) {
        //         if(model.isTaskFinished()) {
        //             is_in_progress = false;
        //             is_should_run = true;

        //             if(model.getExitCode() == 0) {
        //                 is_can_view = true;
        //             } else {
        //                 std::cerr << "task failed, code: " << model.getExitCode() << std::endl;
        //             }
        //         }
        //     }
        // }
        // ImGui::PopStyleColor();
        // ImGui::EndChild();
    }

    void Tline::loadIcon() {       
        icons["select_file"] = std::make_unique<Icon>(icon_select_file);
        icons["open_output"] = std::make_unique<Icon>(icon_open_output);

        icons["compress"] = std::make_unique<Icon>(icon_compress);
        
        icons["settings"] = std::make_unique<Icon>(icon_settings);
    }

    Icon* Tline::getIcon(const std::string name) {
        auto it = icons.find(name);
        if (it != icons.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    // void Tline::IsHoveredIcon(Icon* current, Icon* hovered) {
    //     ImVec2 size{32.0f, 32.0f};
    //     ImGui::InvisibleButton("##hover_icon", size);

    //     Icon* icon_to_render = ImGui::IsItemHovered() ? hovered : current;
    //     ImGui::SetCursorScreenPos(ImGui::GetItemRectMin());

    //     ImGui::Image((void*)(intptr_t)icon_to_render->GetID(), size);
    // }

    void Tline::IsHoveredTopitem(bool is_hovered) {
        if(is_hovered) {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.1f));
        } else {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        }
    }

    void Tline::initIsHovered() {
        is_hovered["select_file"] = false;
        is_hovered["open_output"] = false;
        is_hovered["compress"] = false;
        is_hovered["settings"] = false;        
    }

    void Tline::SetPaths(std::filesystem::path name_output_dir, std::filesystem::path path_to_file) {
        this->name_output_dir = name_output_dir;
        this->path_to_file = (path_to_file);
    }
}
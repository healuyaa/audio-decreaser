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
        loadIcon();
        initIsHovered();
    }

    void Tline::lineUI(const std::string& name) {
        ImVec2 size{160.0f, 50.0f};

        IsHoveredTopitem(is_hovered["output_dir"]);
        
        ImGui::BeginChild("##output dir", size, true);
        {
            Icon* current = getIcon("folder_hide");
            Icon* hovered = getIcon("folder_show");

            is_hovered["output_dir"] = ImGui::IsWindowHovered();

            IsHoveredIcon(current, hovered);

            ImGui::SameLine();

            ImGui::Text("output dir");

            if (is_hovered["output_dir"] && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                is_file_dialog_open = true;
                file_dialog_future = std::async(std::launch::async, [this] {
                    // const char *command = "nautilus ../ADOutput";                    
                    // system(command); 
                    std::cout << "here dir out ";

                    is_file_dialog_open = false;
                });
            }
        }
        ImGui::PopStyleColor();
        ImGui::EndChild();

        ImGui::SameLine();

        IsHoveredTopitem(is_hovered["pick"]);

        ImGui::BeginChild("##pick", size, true);
        {
            is_hovered["pick"] = ImGui::IsWindowHovered();

            ImGui::Image((void*)(intptr_t) icons["folder_pick"]->GetID(), ImVec2(32.0f, 32.0f));

            ImGui::SameLine();

            ImGui::Text("pick output dir");

            if (is_hovered["pick"] && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                std::cout << "clicked pick" << std::endl;
            }
        }
        ImGui::PopStyleColor();
        ImGui::EndChild();

        ImGui::SameLine();

        IsHoveredTopitem(is_hovered["delete"]);

        ImGui::BeginChild("##delete", size, true);
        {
            is_hovered["delete"] = ImGui::IsWindowHovered();

            ImGui::Image((void*)(intptr_t) icons["file_delete"]->GetID(), ImVec2(32.0f, 32.0f));

            ImGui::SameLine();

            ImGui::Text("file_delete");

            if (is_hovered["delete"] && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                std::cout << "clicked delete" << std::endl;  
                is_delete_current = true;
            }
        }
        ImGui::PopStyleColor();
        ImGui::EndChild();

        ImGui::SameLine();

        IsHoveredTopitem(is_hovered["delete_all"]);

        ImGui::BeginChild("##delete_all", size, true);
        {
            is_hovered["delete_all"] = ImGui::IsWindowHovered();

            ImGui::Image((void*)(intptr_t) icons["file_delete"]->GetID(), ImVec2(32.0f, 32.0f));

            ImGui::SameLine();

            ImGui::Text("file_delete_all");

            if (is_hovered["delete_all"] && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                std::cout << "clicked delete_all" << std::endl;
                is_delete_all = true;
            }
        }
        ImGui::PopStyleColor();
        ImGui::EndChild();

        ImGui::SameLine();

        IsHoveredTopitem(is_hovered["run"]);

        ImGui::BeginChild("##run", size, true);
        {
            is_hovered["run"] = ImGui::IsWindowHovered();

            ImGui::Text("run");

            if (is_hovered["run"] && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                is_should_run = false;
                is_in_progress = false;
                std::cout << "clicked run" << std::endl;
            }

            if(!is_should_run && !is_in_progress) {
                is_in_progress = true;
                if(!path_to_file.empty()) {
                    model.SetPaths(name_output_dir, path_to_file);
                    model.run();
                }
            }

            if(is_in_progress) {
                if(model.isTaskFinished()) {
                    is_in_progress = false;
                    is_should_run = true;

                    if(model.getExitCode() == 0) {
                        is_can_view = true;
                    } else {
                        std::cerr << "task failed, code: " << model.getExitCode() << std::endl;
                    }
                }
            }
        }
        ImGui::PopStyleColor();
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::SetCursorScreenPos(ImVec2(ImGui::GetItemRectMin().x + size.x + size.x + 18.0f, ImGui::GetItemRectMin().y));
        ImGui::BeginChild("##status", size, true);
        {            
            ImGui::Text("status: ");

            ImGui::SameLine();

            if(is_in_progress) {
                ImGui::Text("in progress");
            } else {
                ImGui::Text("waiting...");
            }
        }
        ImGui::EndChild();
    }

    void Tline::loadIcon() {       
        icons["folder_show"] = std::make_unique<Icon>(icon_folder_show);
        icons["folder_hide"] = std::make_unique<Icon>(icon_folder_hide);

        icons["folder_pick"] = std::make_unique<Icon>(icon_folder_pick);
        
        icons["file_delete"] = std::make_unique<Icon>(icon_file_delete);
    }

    Icon* Tline::getIcon(const std::string name) {
        auto it = icons.find(name);
        if (it != icons.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    void Tline::IsHoveredIcon(Icon* current, Icon* hovered) {
        ImVec2 size{32.0f, 32.0f};
        ImGui::InvisibleButton("##hover_icon", size);

        Icon* icon_to_render = ImGui::IsItemHovered() ? hovered : current;
        ImGui::SetCursorScreenPos(ImGui::GetItemRectMin());

        ImGui::Image((void*)(intptr_t)icon_to_render->GetID(), size);
    }

    void Tline::IsHoveredTopitem(bool is_hovered) {
        if(is_hovered) {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.1f));
        } else {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        }
    }

    void Tline::initIsHovered() {
        is_hovered["output_dir"] = false;
        is_hovered["pick"] = false;
        is_hovered["delete"] = false;
        is_hovered["delete_all"] = false;        
        is_hovered["run"] = false;
    }

    bool Tline::IsShouldRun() {
        return is_should_run;
    }

    void Tline::SetShouldRun() {
        is_should_run = false;
    }

    void Tline::SetPaths(std::filesystem::path name_output_dir, std::filesystem::path path_to_file) {
        this->name_output_dir = name_output_dir;
        this->path_to_file = (path_to_file);
    }

    bool Tline::GetProgress() {
        return is_in_progress;
    }

    bool Tline::GetCanView() {
        return is_can_view;
    }

    void Tline::SetDeleteCurrent(bool s) {
        is_delete_current = s;
    }

    bool Tline::GetDeleteCurrent() {
        return is_delete_current;
    }

    void Tline::SetDeleteAll(bool s) {
        is_delete_all = s;
    }

    bool Tline::GetDeleteAll() {
        return is_delete_all;
    }
}
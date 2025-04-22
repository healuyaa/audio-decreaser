#include "adt-interface-top-line.hpp"

#include "adt-flags.hpp"
#include "adt-icon.hpp"
#include "adt-paths.hpp"
#include "imgui.h"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <windows.h>

namespace adt {
    Tline::Tline() {
        loadIcon();
        initIsHovered();
    }

    void Tline::lineUI(const std::string& name) {
        ImVec2 size{160.0f, 50.0f};

        IsHoveredTopitem(is_hovered["load_file"]);
        
        ImGui::BeginChild("##load_file", size, true);
        {
            ImGui::Image((void*)(intptr_t) icons["load_file"]->GetID(), ImVec2(32.0f, 32.0f));

            is_hovered["load_file"] = ImGui::IsWindowHovered();

            ImGui::SameLine();

            ImGui::Text("load_file");

            if (is_hovered["load_file"] && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !Flags::getInstance().GetFileDialogOpen()) {
                OpenFileDialog();
                std::cout << "here load_file" << std::endl;
            }
        }
        ImGui::PopStyleColor();
        ImGui::EndChild();

        ImGui::SameLine();

        IsHoveredTopitem(is_hovered["load_group_files"]);
        
        ImGui::BeginChild("##load_group_files", size, true);
        {
            ImGui::Image((void*)(intptr_t) icons["load_group_files"]->GetID(), ImVec2(32.0f, 32.0f));

            is_hovered["load_group_files"] = ImGui::IsWindowHovered();

            ImGui::SameLine();

            ImGui::Text("load_group_files");

            if (is_hovered["load_group_files"] && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !Flags::getInstance().GetFileDialogOpenDir()) {
                Flags::getInstance().SetFileDialogOpenDir(true);
                std::cout << "here load_group_files" << std::endl;
            }
        }
        ImGui::PopStyleColor();
        ImGui::EndChild();

        ImGui::SameLine();

        IsHoveredTopitem(is_hovered["output_folder"]);
        
        ImGui::BeginChild("##output_folder", size, true);
        {
            ImGui::Image((void*)(intptr_t) icons["output_folder"]->GetID(), ImVec2(32.0f, 32.0f));

            is_hovered["output_folder"] = ImGui::IsWindowHovered();

            ImGui::SameLine();

            ImGui::Text("output_folder");

            if (is_hovered["output_folder"] && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                Flags::getInstance().SetOpenOutput(true);
            }
        }
        ImGui::PopStyleColor();
        ImGui::EndChild();

        ImGui::SameLine();

        IsHoveredTopitem(is_hovered["compress"]);

        ImGui::BeginChild("##compress", size, true);
        {
            is_hovered["compress"] = ImGui::IsWindowHovered();

            ImGui::Image((void*)(intptr_t) icons["compress"]->GetID(), ImVec2(32.0f, 32.0f));

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

            ImGui::Image((void*)(intptr_t) icons["settings"]->GetID(), ImVec2(32.0f, 32.0f));

            ImGui::SameLine();

            ImGui::Text("settings");

            if (is_hovered["settings"] && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                std::cout << "here settings" << std::endl;                  
            }
        }
        ImGui::PopStyleColor();
        ImGui::EndChild();
    }

    void Tline::loadIcon() {       
        icons["load_file"] = std::make_unique<Icon>(Paths::getInstance().GetPath("icon_load_file"));
        icons["load_group_files"] = std::make_unique<Icon>(Paths::getInstance().GetPath("icon_load_group_files"));
        icons["output_folder"] = std::make_unique<Icon>(Paths::getInstance().GetPath("icon_output_folder"));
        icons["compress"] = std::make_unique<Icon>(Paths::getInstance().GetPath("icon_compress"));
        icons["settings"] = std::make_unique<Icon>(Paths::getInstance().GetPath("icon_settings"));
    }

    Icon* Tline::getIcon(const std::string name) {
        auto it = icons.find(name);
        if (it != icons.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    void Tline::IsHoveredTopitem(bool is_hovered) {
        if(is_hovered) {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.1f));
        } else {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        }
    }

    void Tline::initIsHovered() {
        is_hovered["load_file"] = false;
        is_hovered["load_group_files"] = false;
        is_hovered["output_folder"] = false;
        is_hovered["compress"] = false;
        is_hovered["settings"] = false;        
    }

    void Tline::OpenFileDialog() {
        Flags::getInstance().SetFileDialogOpen(true);

        future = std::async(std::launch::async, [this]() -> void {
            char buffer[MAX_PATH] = { 0 };

            OPENFILENAME ofn;
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = nullptr;
            ofn.lpstrFilter = "Audio files (*.wav)\0*.wav\0All files (*.*)\0*.*\0";
            ofn.lpstrFile = buffer;
            ofn.nMaxFile = MAX_PATH;
            ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

            if (GetOpenFileName(&ofn)) {
                std::string path = buffer;

                {
                    std::lock_guard<std::mutex> lock(mutex_);
                    Paths::getInstance().addTempPath(std::to_string(id), path);
                    ++id;
                }
            }

            Flags::getInstance().SetFileDialogOpen(false);
        });
    }
}
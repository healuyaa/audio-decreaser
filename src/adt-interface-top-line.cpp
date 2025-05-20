#include "adt-interface-top-line.hpp"

#include "adt-audio-run.hpp"
#include "adt-file-dialog.hpp"
#include "adt-flags.hpp"
#include "adt-icon.hpp"
#include "adt-interface-settings.hpp"
#include "adt-paths.hpp"
#include "imgui.h"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

namespace adt {
    Tline::Tline() {
        loadIcon();
        initIsHovered();
        settings = std::make_shared<adt::Settigns>();
    }

    void Tline::lineUI(const std::string& name, std::atomic<float>* progress) {
        ImVec2 size{160.0f, 50.0f}; // size novigate rectangles

        IsHoveredTopitem(is_hovered["load_file"]);
        
        ImGui::BeginChild("##load_file", size, true);
        {
            ImGui::Image((void*)(intptr_t) icons["load_file"]->GetID(), ImVec2(32.0f, 32.0f));

            is_hovered["load_file"] = ImGui::IsWindowHovered();

            ImGui::SameLine();

            ImGui::Text("Add File");

            if (is_hovered["load_file"] && ImGui::IsMouseClicked(ImGuiMouseButton_Left) 
                && !Flags::getInstance().GetFileDialogOpen() && !Flags::getInstance().GetCompress()) {
                dialog = std::make_shared<adt::ADTFileDialog>();
                dialog->OpenFileDialog();
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

            ImGui::Text("Add Files");

            if (is_hovered["load_group_files"] && ImGui::IsMouseClicked(ImGuiMouseButton_Left) 
                && !Flags::getInstance().GetFolderDialog() && !Flags::getInstance().GetCompress()) {
                dialog = std::make_shared<adt::ADTFileDialog>();
                dialog->OpenFolderDialog();
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

            ImGui::Text("Output");

            if (is_hovered["output_folder"] && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !Flags::getInstance().GetOpenOutput()) {                
                dialog = std::make_shared<adt::ADTFileDialog>();
                dialog->OpenOutputDir(Paths::getInstance().GetPath("results"));
                std::cout << "here output_folder" << std::endl;
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

            ImGui::Text("Compress");

            if (is_hovered["compress"] && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !Flags::getInstance().GetCompress()) {                                
                std::thread([this, progress]() {
                    AudioRunner ar;
                    ar.run(progress);
                }).detach();

                std::cout << "here compress" << std::endl;
            }
        }
        ImGui::PopStyleColor();
        ImGui::EndChild();

        ImGui::SameLine();

        IsHoveredTopitem(is_hovered["delete"]);

        ImGui::BeginChild("##delete", size, true);
        {
            is_hovered["delete"] = ImGui::IsWindowHovered();

            ImGui::Image((void*)(intptr_t) icons["delete"]->GetID(), ImVec2(32.0f, 32.0f));

            ImGui::SameLine();

            ImGui::Text("Delete");

            if (is_hovered["delete"] && ImGui::IsMouseClicked(ImGuiMouseButton_Left) 
                && !Flags::getInstance().GetGlobalDelete() && !Flags::getInstance().GetCompress()) {

                Flags::getInstance().SetGlobalDelete(true);
                std::cout << "here delete" << std::endl;
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

            ImGui::Text("Settings");

            if (is_hovered["settings"] && ImGui::IsMouseClicked(ImGuiMouseButton_Left) 
                && !Flags::getInstance().GetSettings() && !Flags::getInstance().GetCompress()) {
                Flags::getInstance().SetSettings(true);

                ImGui::OpenPopup("Settings");

                std::cout << "here settings" << std::endl;
            }

            if(Flags::getInstance().GetSettings()) {                
                settings->runUI();
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
        icons["delete"] = std::make_unique<Icon>(Paths::getInstance().GetPath("icon_delete"));
        icons["settings"] = std::make_unique<Icon>(Paths::getInstance().GetPath("icon_settings")); // icon_delete
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
}
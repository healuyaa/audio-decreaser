#include "adt-interface.hpp"

#include "adt-file-tools.hpp"
#include "adt-flags.hpp"
#include "adt-interface-right-line.hpp"
#include "adt-interface-top-line.hpp"
#include "adt-paths.hpp"
#include "imgui.h"

#include <cstddef>
#include <utility>
#include <windows.h>
#include <commdlg.h>
#include <filesystem>
#include <memory>
#include <string>

namespace adt {
    Interface::Interface()  {
        FileTools::getInstance().initDirs();        
    }

    void Interface::UInterface() {
        ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);

        ImGuiWindowFlags window_flags = 
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus;

        ImGui::Begin("##main window", nullptr, window_flags);

        ImVec2 window_size = ImGui::GetContentRegionAvail();

        const float top_section_height = 66.0f; // size top bar
        float mid_section_height = window_size.y * 0.89f - ImGui::GetStyle().ItemSpacing.y; // 89

        TopSection(window_size.x, top_section_height);
        MidSection(window_size.x, mid_section_height);

        if(Flags::getInstance().GetGlobalDelete()) {
            GlobalDelete();
        }

        ImGui::End();
    }

    void Interface::TopSection(float width, float height) {
        ImGui::BeginChild("TopSection", ImVec2(0, height), true);

        if(!Flags::getInstance().GetLoadTopMenu()) {
            Flags::getInstance().SetLoadTopMenu(true);
            auto tline = std::make_unique<Tline>();
            this->tline = std::move(tline);
        }
        
        tline->lineUI("name top", &progress);

        ImGui::EndChild();
    }

    void Interface::MidSection(float width, float height) {
        ImGui::BeginChild("MidSection", ImVec2(0, height));

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
            ReindexLlines();
        }

        for(int i = 0; i < Llines.size(); ++i) {
            std::string child_id = "##child_id_l" + std::to_string(i);

            bool is_hovered = false;

            ImVec4 default_bg = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
            ImVec4 hover_bg   = ImVec4(0.1f, 0.1f, 0.1f, 0.15f);

            ImGui::PushStyleColor(ImGuiCol_ChildBg, default_bg);

            if (ImGui::BeginChild(child_id.c_str(), ImVec2(0, 64.0f))) { // 64.0f size + spacing
                is_hovered = ImGui::IsWindowHovered();
    
                if (is_hovered) {
                    ImDrawList* draw = ImGui::GetWindowDrawList();
                    ImVec2 min = ImGui::GetWindowPos();
                    ImVec2 max = ImVec2(min.x + ImGui::GetWindowSize().x, min.y + ImGui::GetWindowSize().y);
                    draw->AddRectFilled(min, max, ImColor(hover_bg));
                }

                Llines[i]->lineUI(std::to_string(i));
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

            std::filesystem::path filename(Paths::getInstance().getTempPath(std::to_string(Rlines.size() - 1)));
            std::string r_path = std::string(Paths::getInstance().GetPath("results")) + "/compressed_" + filename.filename().string();
            r_path = std::filesystem::absolute(r_path).string();

            RlineStates.push_back({.path = r_path, .is_ready = false, .was_checked = false});
        }

        int active_index = -1;
        for(size_t i = 0; i < RlineStates.size(); ++i) {
            if(!RlineStates[i].is_ready) {
                active_index = i;
                break;
            }            
        }

        for(int i = 0; i < Rlines.size(); ++i) {
            std::string child_id = "##child_id_r" + std::to_string(i);

            bool is_hovered = false;

            ImVec4 default_bg = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
            ImVec4 hover_bg   = ImVec4(0.1f, 0.1f, 0.1f, 0.15f);

            ImGui::PushStyleColor(ImGuiCol_ChildBg, default_bg);

            if(ImGui::BeginChild(child_id.c_str(), ImVec2(0, 64.0f))) {
                is_hovered = ImGui::IsWindowHovered();

                if (is_hovered) {
                    ImDrawList* draw = ImGui::GetWindowDrawList();
                    ImVec2 min = ImGui::GetWindowPos();
                    ImVec2 max = ImVec2(min.x + ImGui::GetWindowSize().x, min.y + ImGui::GetWindowSize().y);
                    draw->AddRectFilled(min, max, ImColor(hover_bg));
                }

                if(!RlineStates[i].was_checked) {
                    if(std::filesystem::exists(RlineStates[i].path)) {
                        RlineStates[i].was_checked = true;
                        RlineStates[i].is_ready = true;
                    }
                }

                bool status = RlineStates[i].is_ready;
                if(i == active_index && Flags::getInstance().GetCompress()) {
                    Rlines[i]->lineUI(RlineStates[i].path, status, &progress);
                } else {
                    Rlines[i]->lineUI(RlineStates[i].path, status, nullptr);
                }
            }

            ImGui::PopStyleColor();
            ImGui::EndChild();
        }

        ImGui::EndGroup();
        ImGui::EndChild();
    }

    void Interface::GlobalDelete() {
        Flags::getInstance().SetGlobalDelete(false);

        Llines.clear();
        Rlines.clear();
        Paths::getInstance().clearTempPath();
    }

    void Interface::ReindexLlines() {
        Llines.clear();

        for(size_t i = 0; i < Paths::getInstance().GetSizeTempPool(); ++i) {
            auto lline = std::make_unique<Lline>();
            Llines.push_back(std::move(lline));
        }

        Flags::getInstance().SetLIsDelete(false);
    }
}
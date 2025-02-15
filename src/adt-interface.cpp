#include "adt-interface.hpp"

#include "adt-interface-top-line.hpp"
#include "adt-service.hpp"
#include "imgui.h"
#include "miniaudio.h"

#include <cstddef>
#include <windows.h>
#include <commdlg.h>
#include <filesystem>
#include <memory>
#include <string>

namespace adt {
    Interface::Interface(Service* service) : service(service) {}

    void Interface::UInterface() {
        ImGui::Begin("##top tab", NULL, ImGuiWindowFlags_MenuBar);

        TopBar();

        ImVec2 window_size = ImGui::GetContentRegionAvail();

        float top_section_height = window_size.y * 0.11f;
        float mid_section_height = window_size.y * 0.74f - ImGui::GetStyle().ItemSpacing.y;
        float bot_section_height = window_size.y - mid_section_height - top_section_height - ImGui::GetStyle().ItemSpacing.y - ImGui::GetStyle().ItemSpacing.y;

        TopSection(window_size.x, top_section_height);
        MidSection(window_size.x, mid_section_height);
        BotSection(window_size.x, bot_section_height);

        // IsShouldDelteSMTH();

        ImGui::End();
    }

    void Interface::TopBar() {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::Button("open")) {
                if(!flags.finterface->is_file_dialog_open) {
                    OpenFileDialog();
                }
            } 

            if(!path_to_copy.empty() && !flags.finterface->is_file_existed) {
                paths.push_back(path_to_copy);

                auto lline = std::make_unique<Lline>();
                Llines.push_back(std::move(lline));

                flags.finterface->is_file_existed = true;
                path_to_copy.clear();
            }

            flags.finterface->is_file_existed = false;

            if(ImGui::Button("info")) {

            }

        }        
        ImGui::EndMenuBar();
    }

    void Interface::TopSection(float width, float height) {
        ImGui::BeginChild("TopSection", ImVec2(0, height), true);

        if(!flags.finterface->is_load_top_menu) {
            flags.finterface->is_load_top_menu = true;
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

    void Interface::BotSection(float width, float height) {
        ImGui::BeginChild("BottomSection", ImVec2(width, height), true);
        if (ImGui::Button("play")) {
            if (is_selected_left != -1) {
                audio_player.playAudioFile(paths[is_selected_left], flags.finterface->is_playing_audio);
            } else if (is_selected_right != -1) {
                audio_player.playAudioFile(paths_out[is_selected_right], flags.finterface->is_playing_audio);
            }
        }

        ImGui::SameLine();

        if(ImGui::Button("stop")) {
            if(flags.finterface->is_playing_audio) {
                if(is_selected_left != -1) {
                    audio_player.stopAudioFile(flags.finterface->is_playing_audio);
                } else if(is_selected_right != -1) {
                    audio_player.stopAudioFile(flags.finterface->is_playing_audio);
                }
            }
        }

        auto cur_volume = audio_player.GetVolume();
        ImGui::PushItemWidth(100);
        if (ImGui::SliderFloat("Volume", &cur_volume, 0.0f, 1.0f, "%.2f")) {
            audio_player.SetVolume(cur_volume);
        }

        if(flags.finterface->is_playing_audio) {
            length = audio_player.GetAudioLength();
            current_time = audio_player.GetTime(flags.finterface->is_playing_audio);
        }

        ImGui::PushItemWidth(300);
        if (ImGui::SliderFloat("Time line", &current_time, 0.0f, length, "%.2f")) {
            audio_player.SetTime(flags.finterface->is_playing_audio, current_time);
        }
        
        ImGui::EndChild();
    }

    void Interface::LeftSection(float width, float height) {
        ImGui::BeginChild("LeftSection", ImVec2(width, height), true);
        ImGui::BeginGroup();

        for(int i = 0; i < Llines.size(); ++i) {
            std::string child_id = "##child_id_l" + std::to_string(i);

            if(is_selected_left == i) {
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.1f));
            } else {
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            }

            if(ImGui::BeginChild(child_id.c_str(), ImVec2(0, 64.0f))) {
                if(ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                    if(is_selected_left == i) {
                        is_selected_left = -1;
                    } else {
                        is_selected_left = i;
                    }
                }

                if(is_selected_right != -1 && is_selected_left != -1) {
                    is_selected_right = -1;
                }

                if(is_selected_left && tline->IsShouldRun()) {
                    tline->SetPaths(name_out_dir, paths[i]);
                }

                Llines[i]->lineUI(paths[i].string());
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

        if(tline->GetCanView() && Rlines.size() < Llines.size()) {
            auto rline = std::make_unique<Rline>();
            Rlines.push_back(std::move(rline));

            std::filesystem::path path_to_out = name_out_dir / paths[is_selected_left].filename();
            paths_out.push_back(path_to_out);
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

                Rlines[i]->lineUI(paths_out[i].string());
            }

            ImGui::PopStyleColor();
            ImGui::EndChild();
        }


        ImGui::EndGroup();
        ImGui::EndChild();
    }

    void Interface::OpenFileDialog() {
        flags.finterface->is_file_dialog_open = true;

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
                    std::lock_guard<std::mutex> lock(path_mutex);
                    path_to_copy = std::filesystem::path(path);
                }
            }

            flags.finterface->is_file_dialog_open = false;
        });
    }

    std::filesystem::path Interface::GetOutDir() {
        return name_out_dir;
    }

    void Interface::IsShouldDelteSMTH() {
        if(tline->GetDeleteCurrent() && is_selected_left != -1) {
            Llines.erase(Llines.begin() + is_selected_left);
            paths.erase(paths.begin() + is_selected_left);

            if(is_selected_left < Rlines.size()) {
                Rlines.erase(Rlines.begin() + is_selected_left);
                paths_out.erase(paths_out.begin() + is_selected_left);
            }
        }
    }
}
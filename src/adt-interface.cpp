#include "adt-interface.hpp"

#include "adt-interface-top-line.hpp"
#include "imgui.h"
#include "miniaudio.h"

#include <cstddef>
#include <filesystem>
#include <memory>

namespace adt {
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
                if(!is_file_dialog_open) {
                    OpenFileDialog();
                }
            } 

            if(!path_to_copy.empty() && !file_existed) {
                paths.push_back(path_to_copy);

                auto lline = std::make_unique<Lline>();
                Llines.push_back(std::move(lline));

                file_existed = true;
                path_to_copy.clear();
            }

            file_existed = false;

            if(ImGui::Button("info")) {

            }

            }        
        ImGui::EndMenuBar();
    }

    void Interface::TopSection(float width, float height) {
        ImGui::BeginChild("TopSection", ImVec2(0, height), true);

        if(!load_top) {
            load_top = true;
            auto tline = std::make_unique<Tline>();
            Tlines.push_back(std::move(tline));
        }

        for(auto& tl: Tlines) {
            tl->lineUI("name top");
        }

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
                audio_player.playAudioFile(paths[is_selected_left], is_playing);
            } else if (is_selected_right != -1) {
                audio_player.playAudioFile(paths_out[is_selected_right], is_playing);
            }
        }

        ImGui::SameLine();

        if(ImGui::Button("stop")) {
            if(is_playing) {
                if(is_selected_left != -1) {
                    audio_player.stopAudioFile(is_playing);
                } else if(is_selected_right != -1) {
                    audio_player.stopAudioFile(is_playing);
                }
            }
        }

        auto cur_volume = audio_player.GetVolume();
        ImGui::PushItemWidth(100);
        if (ImGui::SliderFloat("Volume", &cur_volume, 0.0f, 1.0f, "%.2f")) {
            audio_player.SetVolume(cur_volume);
        }

        if(is_playing) {
            length = audio_player.GetAudioLength();
            current_time = audio_player.GetTime(is_playing);
        }

        ImGui::PushItemWidth(300);
        if (ImGui::SliderFloat("Time line", &current_time, 0.0f, length, "%.2f")) {
            audio_player.SetTime(is_playing, current_time);
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

                if(is_selected_left && Tlines[0]->IsShouldRun()) {
                    Tlines[0]->SetPaths(name_out_dir, paths[i]);
                }

                Llines[i]->lineUI(paths[i]);
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

        if(Tlines[0]->GetCanView() && Rlines.size() < Llines.size()) {
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

                Rlines[i]->lineUI(paths_out[i]);
            }

            ImGui::PopStyleColor();
            ImGui::EndChild();
        }


        ImGui::EndGroup();
        ImGui::EndChild();
    }

    void Interface::OpenFileDialog() {
        is_file_dialog_open = true;

        future = std::async(std::launch::async, [this]() -> void {
            char buffer[1024];
            FILE* pipe = popen("zenity --file-selection --title='Select a file' --file-filter='Audio files (*.wav) | *.wav' --file-filter='All files (*.*) | *.*'", "r");
            if (!pipe) return;

            fgets(buffer, sizeof(buffer), pipe);
            pclose(pipe);
            
            std::string path = buffer;
            path.erase(path.find_last_not_of("\n") + 1);

            {
                std::lock_guard<std::mutex> lock(path_mutex);
                path_to_copy = std::filesystem::path(path);
            }

            is_file_dialog_open = false;
        });
    }

    std::filesystem::path Interface::GetOutDir() {
        return name_out_dir;
    }

    void Interface::IsShouldDelteSMTH() {
        if(Tlines[0]->GetDeleteCurrent() && is_selected_left != -1) {
            Llines.erase(Llines.begin() + is_selected_left);
            paths.erase(paths.begin() + is_selected_left);

            if(is_selected_left < Rlines.size()) {
                Rlines.erase(Rlines.begin() + is_selected_left);
                paths_out.erase(paths_out.begin() + is_selected_left);
            }
        }
    }
}
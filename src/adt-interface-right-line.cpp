#include "adt-interface-right-line.hpp"

#include "adt-paths.hpp"
#include "imgui.h"

namespace adt {
    Rline::Rline() {
        loadIcon();
    }

    void Rline::lineUI(const std::string& name) {
        ImGui::Image((void*)(intptr_t)getIcon("audio-ai")->GetID(), ImVec2(64.0f, 64.0f));

        ImGui::SameLine();

        ImGui::SetCursorPos(ImVec2(64.0f + ImGui::GetStyle().ItemSpacing.x,
                                            64.0f + ImGui::GetTextLineHeight() + ImGui::GetStyle().ItemSpacing.y - 64.0f));

        ImGui::Text("%s", name.c_str());

        ImVec2 cursor_pos = ImGui::GetCursorPos();

        uintmax_t size = std::filesystem::file_size(name.c_str());        
        double size_in_mb = static_cast<double>(size) / (1024 * 1024);

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << size_in_mb << " MB";
        
        ImGui::SetCursorPos(ImVec2(cursor_pos.x + ImGui::GetStyle().ItemSpacing.x + 64.0f,
                                            cursor_pos.y + ImGui::GetTextLineHeight() + ImGui::GetStyle().ItemSpacing.y - 48.0f));
        ImGui::Text("%s", oss.str().c_str());
    }

    void Rline::loadIcon() {
        icons["audio-ai"] = std::make_unique<Icon>(Paths::getInstance().GetPath("icon_audio_ai"));
    }

    Icon* Rline::getIcon(const std::string name) {
        auto it = icons.find(name);
        if (it != icons.end()) {
            return it->second.get();
        }
        return nullptr;
    }
}
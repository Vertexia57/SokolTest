#pragma once

#include "imgui.h"
#include "imgui_internal.h"

static bool DEBUG = false;

namespace ImGui
{
    inline bool BeginCollapsingHeaderEx(const char* id, const char* title, bool* isOpen, float sizeX, float paddingX, float paddingY)
    {
        ImGuiContext& g = *GImGui;
        float getFrameHeight = ImGui::GetFrameHeight();
        ImGui::BeginGroup();

        ImGui::BeginGroup();
        ImGui::PushID("id");

        ImDrawList* drawList = ImGui::GetWindowDrawList();

        ImVec2 vec = ImGui::GetCursorScreenPos();
        float screenPosX = vec.x;
        float screenPosY = vec.y;
        float windowWidth = ImGui::GetWindowContentRegionWidth();

        ImU32 frameColor = ImGui::GetColorU32(ImVec4(0x24 / 255.0f, 0x24 / 255.0f, 0x24 / 255.0f, 255 / 255.0f));

        int borderRound = 4;

        int roundingCorners = ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_TopRight;

        drawList->AddRectFilled(ImVec2(screenPosX, screenPosY), ImVec2(screenPosX + windowWidth, screenPosY + getFrameHeight), frameColor, borderRound, roundingCorners);

        bool flag = renderFrameContent(title, isOpen);

        ImGui::PopID();
        ImGui::EndGroup();

        float bk = g.Style.ItemSpacing.y;
        g.Style.ItemSpacing.y = 0;
        int offset = 0;

        if (flag)
            offset = bk;

        ImGui::ItemSize(ImVec2(windowWidth, getFrameHeight - offset));
        g.Style.ItemSpacing.y = bk;

        float y = ImGui::GetCursorPosY();

        ImVec2 vec2 = ImGui::GetCursorScreenPos();

        float height = screenPosY + getFrameHeight;

        vec2 = ImGui::GetCursorScreenPos();

        if (flag)
            ImGui::BeginChildEx(id, sizeX, paddingX, paddingY);
        return flag;
    };

    inline void EndCollapsingHeaderEx(bool isOpen)
    {
        ImGuiContext& g = *GImGui;

        float height = 0;

        ImGuiWindow* childWindow = NULL;
        ImGuiWindow* parentWindow = NULL;

        if (isOpen) {
            childWindow = g.CurrentWindow;
            ImGui::EndChildEx();
        }

        parentWindow = g.CurrentWindow;

        float bk = g.Style.ItemSpacing.y;

        if (!isOpen)
            g.Style.ItemSpacing.y = 0;
        ImGui::EndGroup();
        if (!isOpen)
            g.Style.ItemSpacing.y = bk;

        float getFrameHeight = ImGui::GetFrameHeight();
        ImVec2 vec = ImGui::GetCursorScreenPos();
        float screenPosX = parentWindow->DC.CursorPos.x;
        float screenPosY = parentWindow->DC.CursorPosPrevLine.y;

        float windowWidth = ImGui::GetWindowContentRegionWidth();
        int offset = 0;
        if (isOpen)
            offset = g.Style.ItemSpacing.y;

        if (childWindow != NULL)
            height = childWindow->Size.y + screenPosY + getFrameHeight;
        else
            height = screenPosY + getFrameHeight;

        ImDrawList* drawList = ImGui::GetWindowDrawList();

        windowWidth = ImGui::GetWindowContentRegionWidth();

        float rounding = 5;

        int roundingCorners = ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_TopRight;

        ImU32 borderColor = ImGui::GetColorU32(ImVec4(0x40 / 255.0f, 0x40 / 255.0f, 0x49 / 255.0f, 255 / 255.0f));

        drawList->AddRect(ImVec2(screenPosX, screenPosY), ImVec2(screenPosX + windowWidth, height), borderColor, rounding, roundingCorners, 1.0f);
    };
}
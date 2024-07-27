#pragma once

#include "imgui.h"
#include "imgui_internal.h"

static bool renderFrameContent(const char* title, bool* value);

static bool DEBUG = false;

namespace ImGui
{
    inline bool BeginChildCustomEx(const char* id, float sizeX, float paddingX, float paddingY)
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* parent_window = g.CurrentWindow;

        ImGuiWindowFlags flags = 0;
        flags |= ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_AlwaysUseWindowPadding;
        flags |= (parent_window->Flags & ImGuiWindowFlags_NoMove);  // Inherit the NoMove flag

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 4.f, 4.f });

        const float backup_border_size = g.Style.ChildBorderSize;
        g.Style.ChildBorderSize = 0.0f;
        bool ret = ImGui::Begin(id, NULL, flags);
        g.Style.ChildBorderSize = backup_border_size;

        ImGui::PopStyleVar();

        return ret;
    };

    inline void EndChildCustomEx()
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;

        float bottomOffset = g.Style.ItemSpacing.y;
        float y = ImGui::GetCursorPosY() - bottomOffset;

        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 vec = ImGui::GetCursorStartPos();

        float windowX = window->Pos.x + vec.x;
        float windowY = window->Pos.y + vec.y;

        float windowPaddingX = windowX - window->WindowPadding.x;
        float windowPaddingY = windowY - window->WindowPadding.y;

        float windowWidth = window->Size.x - window->WindowPadding.x * 2;
        float windowHeight = y - window->WindowPadding.y;

        float windowPaddingWidth = windowWidth + window->WindowPadding.x * 2;
        float windowPaddingHeight = windowHeight + window->WindowPadding.y * 2;

        ImGui::SetWindowSize(ImVec2(window->ContentRegionRect.GetWidth(), windowPaddingHeight));

        ImGui::EndChild();

        if (DEBUG) {
            float rounding = 0;
            //Render space + padding
            drawList->AddRect(ImVec2(windowPaddingX, windowPaddingY), ImVec2(windowPaddingX + windowPaddingWidth, windowPaddingY + windowPaddingHeight), ImGui::GetColorU32(ImVec4(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f)), rounding);

            // Render space
            drawList->AddRect(ImVec2(windowX, windowY), ImVec2(windowX + windowWidth, windowY + windowHeight), ImGui::GetColorU32(ImVec4(0.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f)), rounding);
        }
    };

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
        float windowWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;

        ImU32 frameColor = ImGui::GetColorU32(ImVec4(0x24 / 255.0f, 0x24 / 255.0f, 0x24 / 255.0f, 255 / 255.0f));

        int borderRound = 4;

        

        int roundingCorners = ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight;

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
            ImGui::BeginChildCustomEx(id, sizeX, paddingX, paddingY);
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
            ImGui::EndChildCustomEx();
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

        float windowWidth = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
        int offset = 0;
        if (isOpen)
            offset = g.Style.ItemSpacing.y;

        if (childWindow != NULL)
            height = childWindow->Size.y + screenPosY + getFrameHeight;
        else
            height = screenPosY + getFrameHeight;

        ImDrawList* drawList = ImGui::GetWindowDrawList();

        float rounding = 5;

        int roundingCorners = ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight;

        ImU32 borderColor = ImGui::GetColorU32(ImVec4(0x40 / 255.0f, 0x40 / 255.0f, 0x49 / 255.0f, 255 / 255.0f));

        drawList->AddRect(ImVec2(screenPosX, screenPosY), ImVec2(screenPosX + windowWidth, height), borderColor, rounding, roundingCorners, 1.0f);
    };
}

static bool renderFrameContent(const char* title, bool* value)
{
    ImGui::BeginGroup();
    ImGui::PushID("frame");

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float getFrameHeight = ImGui::GetFrameHeight();
    float removedSize = 5.5f; // reduce few pixels
    float halfSize = (getFrameHeight) / 2.0f - removedSize;

    ImVec2 vec = ImGui::GetCursorScreenPos();

    float screenPosX = vec.x;
    float screenPosY = vec.y;
    float arrowPaddingLeft = 6;

    float x = screenPosX + halfSize + arrowPaddingLeft;
    float y = screenPosY + getFrameHeight / 2.0f;

    bool hovered = ImGui::IsMouseHoveringRect(ImVec2(x - halfSize - removedSize, y - halfSize - removedSize),
        ImVec2(x + halfSize + removedSize, y + halfSize + removedSize));

    ImU32 hoveredColor = ImGui::GetColorU32(ImVec4(255 / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f));
    ImU32 clickColor = ImGui::GetColorU32(ImVec4(0 / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f));
    ImU32 whiteColor = ImGui::GetColorU32(ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f));

    bool isWindowHovered = ImGui::IsWindowHovered();

    if (isWindowHovered) {
        if (hovered) {
            if (ImGui::IsMouseDown(0))
                hoveredColor = clickColor;
            if (ImGui::IsMouseReleased(0))
                *value = !*value;
        }

        if (hovered)
            drawList->AddCircleFilled(ImVec2(x, y), halfSize * 2, hoveredColor);
    }

    float triA_X = 0;
    float triA_Y = 0;
    float triB_X = 0;
    float triB_Y = 0;
    float triC_X = 0;
    float triC_Y = 0;

    if (*value) {
        // arrow down
        float offset = -0.5f;
        triA_X = x - halfSize + offset;
        triA_Y = y - halfSize;
        triB_X = x + halfSize + offset;
        triB_Y = y - halfSize;
        triC_X = x + offset;
        triC_Y = y + halfSize;
    }
    else {
        // arrow right
        triA_X = x - halfSize;
        triA_Y = y - halfSize;
        triB_X = x + halfSize;
        triB_Y = y;
        triC_X = x - halfSize;
        triC_Y = y + halfSize;
    }

    ImVec2 textSize = ImGui::CalcTextSize(title, NULL);

    float offsetY = (getFrameHeight - textSize.y) / 2.0f;
    float offsetX = 5;

    drawList->AddText(ImVec2(x + halfSize * 2 + offsetX, screenPosY + offsetY), whiteColor, title, NULL);

    drawList->AddTriangleFilled(ImVec2(triA_X, triA_Y), ImVec2(triB_X, triB_Y), ImVec2(triC_X, triC_Y), whiteColor);

    ImGui::PopID();
    ImGui::EndGroup();

    return *value;
}
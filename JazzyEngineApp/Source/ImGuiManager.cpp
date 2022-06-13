#include "stdafx.h"
#include "ImGuiManager.h"
#include "imgui/imgui.h"

ImGuiManager::ImGuiManager()
{
	// Init ImGui system
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

ImGuiManager::~ImGuiManager()
{
	// Clean ImGui and shutdown
	ImGui::DestroyContext();
}

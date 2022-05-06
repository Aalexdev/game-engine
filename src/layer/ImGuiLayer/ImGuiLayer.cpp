#include "engine/pch.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/layer/ImGuiLayer/ImGuiLayer.hpp"
#include "engine/renderer/Renderer.hpp"

#include "engine/layer/ImGuiLayer/OpenGlImGuiLayer.hpp"
 
#include <libs/ImGuiFileDialog/ImGuiFileDialog.h>

namespace engine{
	Ref<ImGuiLayer> ImGuiLayer::create(){
		ENGINE_PROFILE_FUNCTION();
		switch (Renderer::getRenderAPI()){
			case Renderer::RenderAPI::OpenGL: return OpenGlImGuiLayer::create();
		}

		// test
		ImGuiFileDialog::Instance();

		ENGINE_CORE_ASSERT(false, "unknwon render API");
		return nullptr;
	}

	void ImGuiLayer::setStyle(){
		auto &style = ImGui::GetStyle();
		
		style.Colors[ImGuiCol_WindowBg] = {0.1, 0.1, 0.1, 1};

		// button
		// style.Colors[ImGuiCol_Button] = {0.13, 0.13, 0.13, 1};
		// style.Colors[ImGuiCol_ButtonHovered] = {0.41, 0.41, 0.41, 0.7};
		// style.Colors[ImGuiCol_ButtonActive] = {0.6, 0.6, 0.6, 1};

		// style.Colors[ImGuiCol_Border] = {0.41, 0.41, 0.41, 0.7};
		// style.Colors[ImGuiCol_BorderShadow] = {0.21, 0, 0.42, 1};

		// style.Colors[ImGuiCol_FrameBg] = {0.13, 0.13, 0.13, 1};
		// style.Colors[ImGuiCol_FrameBgActive] = {0.6, 0.6, 0.6, 1};
		// style.Colors[ImGuiCol_FrameBgHovered] = {0.41, 0.41, 0.41, 0.7};
		
		// style.Colors[ImGuiCol_ScrollbarBg] = {0.05, 0, 0.1, 1};
		// style.Colors[ImGuiCol_ScrollbarGrab] = {0.13, 0.13, 0.13, 1};
		// style.Colors[ImGuiCol_ScrollbarGrabActive] = {0.6, 0.6, 0.6, 1};
		// style.Colors[ImGuiCol_ScrollbarGrabHovered] = {0.41, 0.41, 0.41, 0.7};
		
		// style.Colors[ImGuiCol_TitleBg] = ImVec4();
		// style.Colors[ImGuiCol_TitleBgActive];
		// style.Colors[ImGuiCol_TitleBgCollapsed];
		
		// style.Colors[ImGuiCol_Text] = {1, 1, 1, 1};
		// style.Colors[ImGuiCol_TextDisabled] = {0.5, 0.5, 0.5, 0.7};
		// style.Colors[ImGuiCol_TextSelectedBg] = {0.41, 0.41, 0.41, 0.7};

		// style.Colors[ImGuiCol_TabUnfocused] = {0.13, 0.13, 0.13, 1};
		// style.Colors[ImGuiCol_TabUnfocusedActive] = {0.41, 0.41, 0.41, 0.7};
		// style.Colors[ImGuiCol_Tab] = {0.13, 0.13, 0.13, 1};
		// style.Colors[ImGuiCol_TabActive] = {0.6, 0.6, 0.6, 1};;
		// style.Colors[ImGuiCol_TabHovered] = {0.41, 0.41, 0.41, 0.7};

		// style.Colors[ImGuiCol_CheckMark];
		// style.Colors[ImGuiCol_ChildBg];

		// style.Colors[ImGuiCol_DockingEmptyBg] = {0.1, 0.1, 0.1, 1};
		// style.Colors[ImGuiCol_DockingPreview] = {0.41, 0.41, 0.41, 0.7};

		// style.Colors[ImGuiCol_DragDropTarget] = {0.41, 0.41, 0.41, 0.7};

		// style.Colors[ImGuiCol_Header] = {0.13, 0.13, 0.13, 1};
		// style.Colors[ImGuiCol_HeaderActive] = {0.6, 0.6, 0.6, 1};
		// style.Colors[ImGuiCol_HeaderHovered] = {0.41, 0.41, 0.41, 0.7};

		// style.Colors[ImGuiCol_MenuBarBg] = {0.13, 0.13, 0.13, 1};
		// style.Colors[ImGuiCol_ModalWindowDimBg] = {1, 1, 1, 0.5};

		// style.Colors[ImGuiCol_NavHighlight];
		// style.Colors[ImGuiCol_NavWindowingDimBg] = {0.05, 0, 0.1, 1};
		// style.Colors[ImGuiCol_NavWindowingHighlight];

		// style.Colors[ImGuiCol_PlotHistogram] = {0.13, 0.13, 0.13, 1};
		// style.Colors[ImGuiCol_PlotHistogramHovered] = {0.41, 0.41, 0.41, 0.7};

		// style.Colors[ImGuiCol_PlotLines] = {0.13, 0.13, 0.13, 1};
		// style.Colors[ImGuiCol_PlotLinesHovered] = {0.41, 0.41, 0.41, 0.7};

		// style.Colors[ImGuiCol_PopupBg] = {0.1, 0.1, 0.1, 1};

		// style.Colors[ImGuiCol_ResizeGrip] = {0.13, 0.13, 0.13, 1};
		// style.Colors[ImGuiCol_ResizeGripActive] = {0.6, 0.6, 0.6, 1};
		// style.Colors[ImGuiCol_ResizeGripHovered] = {0.41, 0.41, 0.41, 0.7};

		// style.Colors[ImGuiCol_Separator] = {0.13, 0.13, 0.13, 1};
		// style.Colors[ImGuiCol_SeparatorActive] = {0.6, 0.6, 0.6, 1};
		// style.Colors[ImGuiCol_SeparatorHovered] = {0.41, 0.41, 0.41, 0.7};

		// style.Colors[ImGuiCol_SliderGrab] = {0.13, 0.13, 0.13, 1};
		// style.Colors[ImGuiCol_SliderGrabActive] = {0.6, 0.6, 0.6, 1};

		// style.Colors[ImGuiCol_TableBorderLight];
		// style.Colors[ImGuiCol_TableBorderStrong];
		// style.Colors[ImGuiCol_TableHeaderBg];
		// style.Colors[ImGuiCol_TableRowBg];
		// style.Colors[ImGuiCol_TableRowBgAlt];
	}
}
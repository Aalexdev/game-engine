#include "EditorSerializer.hpp"
#include "EditorLayer.hpp"
#include "panels/ContentBrowserPanel.hpp"
#include "panels/SceneHierarchyPanel.hpp"
#include <libs/yaml-cpp/yaml.h>


namespace YAML{
	Emitter& operator<<(YAML::Emitter& out, const ImVec2 &v){
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	Emitter& operator<<(YAML::Emitter& out, const ImVec4 &v){
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	Emitter& operator<<(YAML::Emitter& out, const glm::u32vec2 &v){
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	template<>
	struct convert<ImVec2>{
		static Node encode(const ImVec2 &vec){
			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			return node;
		}

		static bool decode(const Node &node, ImVec2 &vec){
			if (!node.IsSequence() || node.size() != 2) return false;

			vec.x = node[0].as<float>();
			vec.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<ImVec4>{
		static Node encode(const ImVec4 &vec){
			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			node.push_back(vec.z);
			node.push_back(vec.w);
			return node;
		}

		static bool decode(const Node &node, ImVec4 &vec){
			if (!node.IsSequence() || node.size() != 4) return false;

			vec.x = node[0].as<float>();
			vec.y = node[1].as<float>();
			vec.z = node[2].as<float>();
			vec.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::u32vec2>{
		static Node encode(const glm::u32vec2 &vec){
			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			return node;
		}

		static bool decode(const Node &node, glm::u32vec2 &vec){
			if (!node.IsSequence() || node.size() != 2) return false;

			vec.x = node[0].as<uint32_t>();
			vec.y = node[1].as<uint32_t>();
			return true;
		}
	};
}

namespace engine{
	static void deserializeImGui(YAML::Node data){
		YAML::Node styleNode = data["style"];
		if (!styleNode) return;

		auto &style = ImGui::GetStyle();

		style.Alpha = styleNode["Alpha"].as<float>();
		style.AntiAliasedFill = styleNode["AntiAliasedFill"].as<bool>();
		style.AntiAliasedLines = styleNode["AntiAliasedLines"].as<bool>();
		style.AntiAliasedLinesUseTex = styleNode["AntiAliasedLinesUseTex"].as<bool>();
		style.ButtonTextAlign = styleNode["ButtonTextAlign"].as<ImVec2>();
		style.CellPadding = styleNode["CellPadding"].as<ImVec2>();
		style.ChildBorderSize = styleNode["ChildBorderSize"].as<float>();
		style.ChildRounding = styleNode["ChildRounding"].as<float>();
		style.CircleTessellationMaxError = styleNode["CircleTessellationMaxError"].as<float>();
		style.ColorButtonPosition = styleNode["ColorButtonPosition"].as<ImGuiDir>();
		style.ColumnsMinSpacing = styleNode["ColumnsMinSpacing"].as<ImGuiDir>();
		style.CurveTessellationTol = styleNode["CurveTessellationTol"].as<float>();
		style.DisabledAlpha = styleNode["DisabledAlpha"].as<float>();
		style.DisplaySafeAreaPadding = styleNode["DisplaySafeAreaPadding"].as<ImVec2>();
		style.DisplayWindowPadding = styleNode["DisplayWindowPadding"].as<ImVec2>();
		style.FrameBorderSize = styleNode["FrameBorderSize"].as<float>();
		style.FramePadding = styleNode["FramePadding"].as<ImVec2>();
		style.FrameRounding = styleNode["FrameRounding"].as<float>();
		style.GrabMinSize = styleNode["GrabMinSize"].as<float>();
		style.GrabRounding = styleNode["GrabRounding"].as<float>();
		style.IndentSpacing = styleNode["IndentSpacing"].as<float>();
		style.ItemInnerSpacing = styleNode["ItemInnerSpacing"].as<ImVec2>();
		style.ItemSpacing = styleNode["ItemSpacing"].as<ImVec2>();
		style.LogSliderDeadzone = styleNode["LogSliderDeadzone"].as<float>();
		style.MouseCursorScale = styleNode["MouseCursorScale"].as<float>();
		style.PopupBorderSize = styleNode["PopupBorderSize"].as<float>();
		style.PopupRounding = styleNode["PopupRounding"].as<float>();
		style.ScrollbarRounding = styleNode["ScrollbarRounding"].as<float>();
		style.ScrollbarSize = styleNode["ScrollbarSize"].as<float>();
		style.SelectableTextAlign = styleNode["SelectableTextAlign"].as<ImVec2>();
		style.TabBorderSize = styleNode["TabBorderSize"].as<float>();
		style.TabMinWidthForCloseButton = styleNode["TabMinWidthForCloseButton"].as<float>();
		style.TabRounding = styleNode["TabRounding"].as<float>();
		style.TouchExtraPadding = styleNode["TouchExtraPadding"].as<ImVec2>();
		style.WindowBorderSize = styleNode["WindowBorderSize"].as<float>();
		style.WindowMenuButtonPosition = styleNode["WindowMenuButtonPosition"].as<ImGuiDir>();
		style.WindowMinSize = styleNode["WindowMinSize"].as<ImVec2>();
		style.WindowPadding = styleNode["WindowPadding"].as<ImVec2>();
		style.WindowRounding = styleNode["WindowRounding"].as<float>();
		style.WindowTitleAlign = styleNode["WindowTitleAlign"].as<ImVec2>();

		YAML::Node colorsNode = styleNode["Colors"];
		if (colorsNode){
			size_t i=0;
			for (auto color : colorsNode){
				style.Colors[i] = color["color"].as<ImVec4>();
				i++;
			}
		}
	}

	static void serializeImGui(YAML::Emitter &out){

		out << YAML::Key << "style";
		out << YAML::BeginMap;

		auto &style = ImGui::GetStyle();

		out << YAML::Key << "Alpha" << YAML::Value << style.Alpha;
		out << YAML::Key << "AntiAliasedFill" << YAML::Value << style.AntiAliasedFill;
		out << YAML::Key << "AntiAliasedLines" << YAML::Value << style.AntiAliasedLines;
		out << YAML::Key << "AntiAliasedLinesUseTex" << YAML::Value << style.AntiAliasedLinesUseTex;
		out << YAML::Key << "ButtonTextAlign" << YAML::Value << style.ButtonTextAlign;
		out << YAML::Key << "CellPadding" << YAML::Value << style.CellPadding;
		out << YAML::Key << "ChildBorderSize" << YAML::Value << style.ChildBorderSize;
		out << YAML::Key << "ChildRounding" << YAML::Value << style.ChildRounding;
		out << YAML::Key << "CircleTessellationMaxError" << YAML::Value << style.CircleTessellationMaxError;
		out << YAML::Key << "ColorButtonPosition" << YAML::Value << style.ColorButtonPosition;
		out << YAML::Key << "ColumnsMinSpacing" << YAML::Value << style.ColumnsMinSpacing;
		out << YAML::Key << "CurveTessellationTol" << YAML::Value << style.CurveTessellationTol;
		out << YAML::Key << "DisabledAlpha" << YAML::Value << style.DisabledAlpha;
		out << YAML::Key << "DisplaySafeAreaPadding" << YAML::Value << style.DisplaySafeAreaPadding;
		out << YAML::Key << "DisplayWindowPadding" << YAML::Value << style.DisplayWindowPadding;
		out << YAML::Key << "FrameBorderSize" << YAML::Value << style.FrameBorderSize;
		out << YAML::Key << "FramePadding" << YAML::Value << style.FramePadding;
		out << YAML::Key << "FrameRounding" << YAML::Value << style.FrameRounding;
		out << YAML::Key << "GrabMinSize" << YAML::Value << style.GrabMinSize;
		out << YAML::Key << "GrabRounding" << YAML::Value << style.GrabRounding;
		out << YAML::Key << "IndentSpacing" << YAML::Value << style.IndentSpacing;
		out << YAML::Key << "ItemInnerSpacing" << YAML::Value << style.ItemInnerSpacing;
		out << YAML::Key << "ItemSpacing" << YAML::Value << style.ItemSpacing;
		out << YAML::Key << "LogSliderDeadzone" << YAML::Value << style.LogSliderDeadzone;
		out << YAML::Key << "MouseCursorScale" << YAML::Value << style.MouseCursorScale;
		out << YAML::Key << "PopupBorderSize" << YAML::Value << style.PopupBorderSize;
		out << YAML::Key << "PopupRounding" << YAML::Value << style.PopupRounding;
		out << YAML::Key << "ScrollbarRounding" << YAML::Value << style.ScrollbarRounding;
		out << YAML::Key << "ScrollbarSize" << YAML::Value << style.ScrollbarSize;
		out << YAML::Key << "SelectableTextAlign" << YAML::Value << style.SelectableTextAlign;
		out << YAML::Key << "TabBorderSize" << YAML::Value << style.TabBorderSize;
		out << YAML::Key << "TabMinWidthForCloseButton" << YAML::Value << style.TabMinWidthForCloseButton;
		out << YAML::Key << "TabRounding" << YAML::Value << style.TabRounding;
		out << YAML::Key << "TouchExtraPadding" << YAML::Value << style.TouchExtraPadding;
		out << YAML::Key << "WindowBorderSize" << YAML::Value << style.WindowBorderSize;
		out << YAML::Key << "WindowMenuButtonPosition" << YAML::Value << style.WindowMenuButtonPosition;
		out << YAML::Key << "WindowMinSize" << YAML::Value << style.WindowMinSize;
		out << YAML::Key << "WindowPadding" << YAML::Value << style.WindowPadding;
		out << YAML::Key << "WindowRounding" << YAML::Value << style.WindowRounding;
		out << YAML::Key << "WindowTitleAlign" << YAML::Value << style.WindowTitleAlign;

		out << YAML::Key << "Colors" << YAML::Value;
		out << YAML::BeginSeq;

		for (size_t i=0; i<55; i++){
			out << YAML::BeginMap;
			out << YAML::Key << "color" << YAML::Value << style.Colors[i];
			out << YAML::EndMap;
		}

		out << YAML::EndSeq;

		out << YAML::EndMap;
	}

	void EditorSerializer::serializeEditor(YAML::Emitter& out){

		out << YAML::Key << "recentProjects";
		out << YAML::BeginSeq;

		std::vector<std::filesystem::path> writenPaths;
		for (const auto& project : editor->recentProjects){

			bool exist = false;
			for (auto &path : writenPaths){
				if (project == path){
					exist = true;
					break;
				}
			}
			
			if (!exist){
				out << YAML::BeginMap;
				out << YAML::Key << "projectPath" << YAML::Value << project.string();
				writenPaths.push_back(project);
				out << YAML::EndMap;
			}
		}
		out << YAML::EndSeq;
		
	}

	void EditorSerializer::serialize(const std::string &filepath){
		if (!editor) return;

		YAML::Emitter out;

		out << YAML::BeginMap;

		out << YAML::Key << "Window";
		{
			auto &window = editor->app->getDisplay();
			out << YAML::BeginMap;
			out << YAML::Key << "WindowSize" << YAML::Value << glm::u32vec2(window.getWidth(), window.getHeight());
			out << YAML::Key << "WindowPos" << YAML::Value << glm::u32vec2(window.getX(), window.getY());
			out << YAML::EndMap;
		}

		out << YAML::Key << "Editor";
		out << YAML::BeginMap;

		serializeEditor(out);
		editor->panels.serialize(out);

		out << YAML::Key << "ImGui";
		out << YAML::BeginMap;

		serializeImGui(out);

		out << YAML::EndMap;

		out << YAML::EndMap;

		std::ofstream file(filepath);
		file << out.c_str();
		file.close();
	}

	bool EditorSerializer::deserialize(const std::string &filepath){
		std::ifstream file(filepath);

		if (!file.is_open()) return false;

		std::stringstream strStream;
		strStream << file.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());

		{
			auto &window = editor->app->getDisplay();
			YAML::Node windowNode = data["Window"];
			glm::u32vec2 size = windowNode["WindowSize"].as<glm::u32vec2>();
			glm::u32vec2 pos = windowNode["WindowPos"].as<glm::u32vec2>();

			// window.setPos(pos.x, pos.y);
			window.setSize(size.x, size.y);
		}

		YAML::Node editorNode = data["Editor"];
		if (!editorNode) return false;
		
		size_t i=0;
		for (auto path : editorNode["recentProjects"]){
			editor->recentProjects[i] = path["projectPath"].as<std::string>();
			if (++i == 10) break;
		}
		
		for (;i<10; i++){
			editor->recentProjects[i].clear();
		}


		editor->panels.deserialize(editorNode["Panels"]);

		// ImGui
		YAML::Node ImGuiNode = data["ImGui"];

		if (ImGuiNode) deserializeImGui(ImGuiNode);

		return true;
	}
}
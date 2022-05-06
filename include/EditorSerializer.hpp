#pragma once

#include <engine/engine.hpp>
#include <libs/yaml-cpp/yaml.h>

#include "panels/ContentBrowserPanel.hpp"
#include "panels/SceneHierarchyPanel.hpp"

namespace engine{
	class EditorLayer;

	class EditorSerializer{
		public:
			EditorSerializer(EditorLayer *editor) : editor{editor}{}
			~EditorSerializer() {}

			void serialize(const std::string &filepath);
			bool deserialize(const std::string &filepath);
		
		private:

			static void serializeContentBroswerPanel(YAML::Emitter& out, const ContentBrowserPanel &panel);
			static void serializeHierarchyPanel(YAML::Emitter& out, const SceneHierarchyPanel &panel);
			void serializeEditor(YAML::Emitter& out);

			static void deserializeContentBrowserPanel(YAML::Node data, ContentBrowserPanel *panel);
			static void deserializeHierarchyPanel(YAML::Node data, SceneHierarchyPanel *panel);


			EditorLayer* editor;
	};
}
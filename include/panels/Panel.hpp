#pragma once

#include <engine/engine.hpp>
#include <libs/yaml-cpp/yaml.h>
#include "../Event/EditorEvents.hpp"

namespace engine{

	enum class PanelType : uint8_t{
		NONE = 0,
		CONTENT_BROWSER_PANEL,
		SCENE_HIERARCHY_PANEL,
		TOOLBAR_PANEL,
		SCENE_PANEL,
	};

	class EditorLayer;

	class Panel{
		friend class PanelLibrary;
		friend class EditorLayer;
		public:
			virtual ~Panel() = default;

			virtual void OnImGuiRender() = 0;
			virtual void OnEvent(EditorEvent &e) = 0;

			virtual void serialize(YAML::Emitter &out) = 0;
			virtual void deserialize(YAML::Node data) = 0;
			
			inline void setCallback(const EditorEventCallbackFn &callback) {this->callback = callback;}
			inline void setInputs(const Ref<Input> &inputs) {this->inputs = inputs;}
			inline void setEditor(EditorLayer *editor) {this->editor = editor;}

			virtual PanelType getStaticType() const = 0;

			static Ref<Panel> create(PanelType type, const EditorEventCallbackFn &callback, const Ref<Input> &inputs, EditorLayer *editor);

		protected:
			EditorEventCallbackFn callback;
			Ref<Input> inputs;
			EditorLayer* editor;

			static Ref<Panel> load(YAML::Node data, const EditorEventCallbackFn &callback, const Ref<Input> &inputs, EditorLayer *editor);
	};
}
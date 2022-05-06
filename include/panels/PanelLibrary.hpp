#pragma once

#include <libs/yaml-cpp/yaml.h>
#include "Panel.hpp"

namespace engine{
	class PanelLibrary{
		public:
			PanelLibrary(){}

			void push(const Ref<Panel> &panel);
			void pop(const Ref<Panel> &panel);
			void clear();

			int size() const {return panels.size();}

			void serialize(YAML::Emitter &out);
			void deserialize(YAML::Node node);

			void OnEvent(EditorEvent &e);
			void OnImGuiRender();

			void init(const EditorEventCallbackFn &callback, const Ref<Input> &inputs, EditorLayer* editor);
		
		private:
			// containe the panel and the id of the panel
			std::list<Ref<Panel>> panels;

			EditorEventCallbackFn callback;
			Ref<Input> inputs;
			EditorLayer* editor;
	};
}
#include "panels/PanelLibrary.hpp"

namespace engine{
	void PanelLibrary::serialize(YAML::Emitter &out){
		// out << YAML::BeginMap;
		out << YAML::Key << "Panels";

		out << YAML::BeginSeq;

		for (auto &panel : panels){
			out << YAML::BeginMap;
			out << YAML::Key << "panel" << YAML::Value << static_cast<int>(panel->getStaticType());
			panel->serialize(out);

			out << YAML::EndMap;
		}

		out << YAML::EndSeq;
		// out << YAML::EndMap;
	}

	void PanelLibrary::deserialize(YAML::Node data){
		for (auto panelNode : data){
			Ref<Panel> panel = Panel::load(panelNode, callback, inputs, editor);
			if (panel) push(panel);
		}
	}

	void PanelLibrary::OnEvent(EditorEvent &e){
		for (auto &panel : panels){
			panel->OnEvent(e);
		}
	}

	void PanelLibrary::OnImGuiRender(){
		uint32_t id=0;
		for (auto &panel : panels){
			ImGui::PushID(static_cast<int>(id));;
			panel->OnImGuiRender();
			ImGui::PopID();
			id++;
		}
	}

	void PanelLibrary::push(const Ref<Panel> &panel){
		panels.push_back(panel);
	}

	void PanelLibrary::clear(){
		panels.clear();
	}

	void PanelLibrary::pop(const Ref<Panel> &panel){
		std::list<Ref<Panel>>::iterator it;

		for (it=panels.begin(); it!=panels.end(); it++){
			if (panel == *it){
				panels.erase(it);
				break;
			}
		}
	}

	void PanelLibrary::init(const EditorEventCallbackFn &callback, const Ref<Input> &inputs, EditorLayer* editor){
		this->callback = callback;
		this->inputs = inputs;
		this->editor = editor;
	}

}
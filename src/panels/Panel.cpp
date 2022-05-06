#include "panels/Panel.hpp"

#include "EditorLayer.hpp"

#include "panels/ContentBrowserPanel.hpp"
#include "panels/SceneHierarchyPanel.hpp"
#include "panels/ToolbarPanel.hpp"

namespace engine{
	Ref<Panel> Panel::create(PanelType type, const EditorEventCallbackFn &callback, const Ref<Input> &inputs, EditorLayer *editor){

		Ref<Panel> panel = nullptr;
		switch (type){
			case PanelType::CONTENT_BROWSER_PANEL:
				panel = ContentBrowserPanel::create();
				break;

			case PanelType::SCENE_HIERARCHY_PANEL:
				panel = SceneHierarchyPanel::create();
				break;
			
			case PanelType::TOOLBAR_PANEL:
				panel = ToolbarPanel::create();
				break;
		}

		if (panel){
			panel->setCallback(callback);
			panel->setInputs(inputs);
			panel->setEditor(editor);
			return panel;

		} else {

			ENGINE_ASSERT(false, "unknown type");
			return nullptr;
		}
	}

	Ref<Panel> Panel::load(YAML::Node data, const EditorEventCallbackFn &callback, const Ref<Input> &inputs, EditorLayer *editor){
		int type = 0;

		try{
			type = data["panel"].as<int>();
		} catch (const YAML::Exception &e){
			ENGINE_ERROR("yaml error : ", e.what());
			return nullptr;
		}

		if (type == 0){
			ENGINE_ERROR("invalid panel type : '", type, "'");
			return nullptr;
		}

		Ref<Panel> panel = create(static_cast<PanelType>(type), callback, inputs, editor);

		if (!panel) return nullptr;
		panel->deserialize(data);
		return panel;
	}
}
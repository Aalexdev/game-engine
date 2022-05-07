#include "panels/ScenePanel.hpp"
#include "EditorLayer.hpp"

#include <libs/glm/gtc/type_ptr.hpp>

namespace engine{
	Ref<ScenePanel> ScenePanel::create(){
		return createRef<ScenePanel>();
	}

	void ScenePanel::OnImGuiRender(){
		if (ImGui::Begin("scene")){
			
			if (ImGui::TreeNodeEx("physics")){
				
				ImGui::DragFloat2("gravity", glm::value_ptr(editor->activeScene->getGravity()), 0.1);
				ImGui::TreePop();
			}

		}
		ImGui::End();
	}

	void ScenePanel::OnEvent(EditorEvent &e){

	}

	void ScenePanel::serialize(YAML::Emitter &out){

	}

	void ScenePanel::deserialize(YAML::Node data){

	}
}
#include "engine/pch.hpp"
#include "engine/renderer/EditorCamera.hpp"
#include "engine/Events/Keycodes.hpp"
#include "engine/Events/MouseButtons.hpp"

namespace engine{
	void EditorCamera::OnViewportResized(uint32_t width, uint32_t height){
		aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		updateProjection();
	}

	void EditorCamera::OnUpdate(Timestep ts){
		glm::vec2 delta = (lastMousePos - glm::vec2(input->getMouseX(), -input->getMouseY())) * speed * zoom;
		lastMousePos = glm::vec2(input->getMouseX(),- input->getMouseY());

		if (input->isMouseButtonDown(ENGINE_MOUSE_BUTTON_RIGHT)){
			camera.setPosition(camera.getPosition() + delta);
			camera.update();
		}
	}

	void EditorCamera::updateProjection(){
		float left = -0.5f * aspectRatio * zoom;
		camera.orthographic(left, -left, -zoom / 2.f, zoom / 2.f, -1.f, 1.f);
	}

	void EditorCamera::OnEvent(Event &e){
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(ENGINE_BIND_EVENT_FN(OnMouseScrolled));
	}
	
	bool EditorCamera::OnMouseScrolled(MouseScrolledEvent &e){
		if (input->isKeyDown(Key::KEY_LEFT_SHIFT) || input->isKeyDown(Key::KEY_RIGHT_SHIFT)){
			speed += e.getYOffset() * 0.001f;
		} else {
			zoom -= e.getYOffset() * 0.25f;
			zoom = std::max(zoom, 0.25f);
			updateProjection();
		}
		return false;
	}
}
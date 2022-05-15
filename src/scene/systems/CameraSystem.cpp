#include "engine/pch.hpp"
#include "engine/scene/systems/CameraSystem.hpp"
#include "engine/scene/ECS.hpp"
#include "engine/scene/Scene.hpp"
#include "engine/scene/Entity.hpp"
#include "engine/scene/components/TransformComponent.hpp"

namespace engine::ECS::systems{
	Camera::Camera(Scene *scene) : scene{scene}{}

	void Camera::init(){
		auto &registry = scene->getRegistry();
		Signature signature{};
		signature.set(registry.getComponentType<components::Camera>());
		signature.set(registry.getComponentType<components::Transform>());
		registry.setSystemSignature<Camera>(signature);
	}

	SceneCamera* Camera::update(){
		SceneCamera *primaryCamera = nullptr;

		float cameraPriority = 0.f;
		for (const auto &e : entities){
			engine::Entity entity = {e, scene};

			auto &camera = entity.getComponent<components::Camera>();
			auto &transform = entity.getComponent<components::Transform>().transform;

			if (camera.priorityLevel > cameraPriority){
				primaryCamera = &camera.camera;
				cameraPriority = camera.priorityLevel;
			}

			camera.camera.setPosition(transform.translation);
			camera.camera.setRotation(transform.rotation);
			camera.camera.updateView();
		}

		if (primaryCamera == nullptr && !entities.empty()){
			primaryCamera = &engine::Entity(*entities.begin(), scene).getComponent<components::Camera>().camera;
		}

		return primaryCamera;
	}

	void Camera::updateViewport(uint32_t width, uint32_t height){
		float aspect = static_cast<float>(width) / static_cast<float>(height);

		for (const auto &e : entities){
			engine::Entity entity = {e, scene};
			auto &camera = entity.getComponent<components::Camera>();

			camera.camera.setAspectRatio(aspect);
			camera.viewportWidth = width;
			camera.viewportHeight = height;
		}
	}
}
#include "engine/pch.hpp"
#include "engine/scene/BatchRenderer.hpp"
#include "engine/scene/Scene.hpp"
#include "engine/scene/Entity.hpp"
#include "engine/scene/components/TransformComponent.hpp"
#include "engine/scene/components/TriangleRenderer.hpp"
#include "engine/scene/systems/CircleRendererSystem.hpp"
#include "engine/scene/systems/SpriteSystem.hpp"
#include "engine/scene/systems/TriangleRendererSystem.hpp"

namespace engine{

	BatchRenderer::BatchRenderer(Scene *scene) : scene{scene}{
		auto &registry = scene->getRegistry();

		circleRendererSystem = registry.RegisterSystem<ECS::systems::CircleRenderer>(scene);
		spriteSystem = registry.RegisterSystem<ECS::systems::Sprite>(scene);
		triangleRendererSystem = registry.RegisterSystem<ECS::systems::TriangleRenderer>(scene);
	}

	void BatchRenderer::render(){
		auto renderer = scene->getRenderer();
		std::set<uint16_t> ids;
		
		auto sprites = spriteSystem->sort(ids);
		auto circles = circleRendererSystem->sort(ids);
		auto triangles = triangleRendererSystem->sort(ids);

		for (const auto &id : ids){
			{
				auto it = sprites.find(id);
				if (it != sprites.end()){
					for (auto &entity : sprites[id]){
						auto &sprite = entity.getComponent<ECS::components::Sprite>();
						auto &transform = entity.getComponent<ECS::components::Transform>();
						renderer->drawQuad(transform.transformMat, sprite.color, static_cast<uint32_t>(entity), sprite.texture);
					}
				}
			}

			{
				auto it = circles.find(id);
				if (it != circles.end()){
					for (auto &entity : circles[id]){
						auto &circle = entity.getComponent<ECS::components::CircleRenderer>();
						auto &transform = entity.getComponent<ECS::components::Transform>();
						renderer->drawCircle(transform.transformMat, circle.color, static_cast<uint32_t>(entity), circle.thickness, circle.fade);
					}
				}
			}

			{
				auto it = triangles.find(id);
				if (it != triangles.end()){
					for (auto &entity : triangles[id]){
						auto &triangle = entity.getComponent<ECS::components::TriangleRenderer>();
						auto &transform = entity.getComponent<ECS::components::Transform>();
						renderer->drawTriangle(triangle.p1, triangle.p2, triangle.p3, transform.transformMat, triangle.texture, static_cast<uint32_t>(entity));
					}
				}
			}

			renderer->endScene();
			renderer->reloadScene();
		}
	}
}
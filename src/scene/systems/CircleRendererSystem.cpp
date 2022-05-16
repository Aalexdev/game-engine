#include "engine/scene/systems/CircleRendererSystem.hpp"
#include "engine/scene/Scene.hpp"
#include "engine/scene/Entity.hpp"
#include "engine/scene/components/TransformComponent.hpp"

namespace engine::ECS::systems{
	
	CircleRenderer::CircleRenderer(Scene *scene) : scene{scene}{}

	void CircleRenderer::init(){
		auto &registry = scene->getRegistry();
		Signature signature;
		signature.set(registry.getComponentType<components::Transform>());
		signature.set(registry.getComponentType<components::CircleRenderer>());
		registry.setSystemSignature<CircleRenderer>(signature);
	}
	
	std::unordered_map<uint16_t, std::list<engine::Entity>> CircleRenderer::sort(std::set<uint16_t> &ids){
		std::unordered_map<uint16_t, std::list<engine::Entity>> map{};

		for (const auto &e : entities){
			engine::Entity entity = {e, scene};
			auto &circle = entity.getComponent<components::CircleRenderer>();
			uint16_t id = circle.batchGroup;

			ids.insert(id);
			map[id].push_back(entity);
		}

		return map;
	}

}
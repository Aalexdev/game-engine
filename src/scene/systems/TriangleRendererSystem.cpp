#include "engine/pch.hpp"
#include "engine/scene/systems/TriangleRendererSystem.hpp"
#include "engine/scene/Scene.hpp"
#include "engine/scene/Entity.hpp"
#include "engine/scene/components/TransformComponent.hpp"
#include "engine/scene/components/TriangleRenderer.hpp"

namespace engine::ECS::systems{
	void TriangleRenderer::init(){
		auto &registry = scene->getRegistry();
		Signature signature;
		signature.set(registry.getComponentType<components::Transform>());
		signature.set(registry.getComponentType<components::TriangleRenderer>());
		registry.setSystemSignature<TriangleRenderer>(signature);
	}

	std::unordered_map<uint16_t, std::list<engine::Entity>> TriangleRenderer::sort(std::set<uint16_t> &ids){
		std::unordered_map<uint16_t, std::list<engine::Entity>> map{};

		for (const auto &e : entities){
			engine::Entity entity = {e, scene};
			auto &triangle = entity.getComponent<components::TriangleRenderer>();
			uint16_t id = triangle.batchGroup;

			ids.insert(id);
			map[id].push_back(entity);
		}

		return map;
	}
}
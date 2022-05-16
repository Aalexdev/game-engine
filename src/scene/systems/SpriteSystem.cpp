#include "engine/pch.hpp"
#include "engine/scene/Scene.hpp"
#include "engine/scene/systems/SpriteSystem.hpp"
#include "engine/scene/components/SpriteComponent.hpp"
#include "engine/scene/components/TransformComponent.hpp"
#include "engine/scene/Entity.hpp"

namespace engine::ECS::systems{
	void Sprite::init(){
		auto &registry = scene->getRegistry();
		Signature signature;
		signature.set(registry.getComponentType<components::Transform>());
		signature.set(registry.getComponentType<components::Sprite>());
		registry.setSystemSignature<Sprite>(signature);
	}

	std::unordered_map<uint16_t, std::list<engine::Entity>> Sprite::sort(std::set<uint16_t> &ids){
		std::unordered_map<uint16_t, std::list<engine::Entity>> map{};

		for (const auto &e : entities){
			engine::Entity entity = {e, scene};
			auto &sprite = entity.getComponent<components::Sprite>();
			uint16_t id = sprite.batchGroup;

			ids.insert(id);
			map[id].push_back(entity);
		}

		return map;
	}
}

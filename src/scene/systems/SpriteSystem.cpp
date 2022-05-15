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

	// void Sprite::OnEntityPushed(Entity entity){
	// 	sprites.pushBack(scene->getRegistry().getComponent<components::EntityData>(entity).uuid);
	// }

	// void Sprite::OnEntityRemoved(Entity entity){
	// 	sprites.erase(scene->getRegistry().getComponent<components::EntityData>(entity).uuid);
	// }

	void Sprite::update(){
		auto renderer = scene->getRenderer();
		std::map<uint16_t, std::vector<engine::Entity>> sprites;

		for (const auto &e : entities){
			engine::Entity entity = {e, scene};
			auto &sprite = entity.getComponent<components::Sprite>();
			sprites[sprite.batchGroup].push_back(entity);
		}

		for (auto &pair : sprites){
			auto &vec = pair.second;
			for (auto &entity : vec){
				auto &transform = entity.getComponent<components::Transform>();
				auto &sprite = entity.getComponent<components::Sprite>();
				renderer->drawQuad(transform.transformMat, sprite.color, static_cast<Entity>(entity), sprite.texture);
			}
			renderer->endScene();
			renderer->reloadScene();
		}

		// renderer->reloadScene();
		// auto it = spriteGroups.begin();

		// while (it){
		// 	engine::Entity entity = {scene->get(*it), scene};
		// 	auto &transform = entity.getComponent<components::Transform>();
		// 	auto &sprite = entity.getComponent<components::Sprite>();

		// 	renderer->drawQuad(transform.transformMat, sprite.color, static_cast<uint32_t>(entity), sprite.texture);

		// 	it++;
		// }


	}
	
	// void Sprite::moveBack(UUID uuid){
	// 	spriteGroups.swapBackward(uuid);
	// }

	// void Sprite::moveFront(UUID uuid){
	// 	spriteGroups.swapForward(uuid);
	// }

	// void Sprite::moveToBack(UUID uuid){
	// 	spriteGroups.pushFront(uuid);
	// }

	// void Sprite::moveToFront(UUID uuid){
	// 	spriteGroups.pushBack(uuid);
	// }
}

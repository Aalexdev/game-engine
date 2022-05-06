#pragma once

#include "../core.hpp"
#include "../Timestep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/EditorCamera.hpp"

// #include "components/NativeScriptComponent.hpp"
#include "components/SpriteComponent.hpp"
#include "../dataStruct/Set.hpp"

#include <libs/entt/entt.hpp>

class b2World;

namespace engine{
	class Entity;
	class SpriteQueue;

	class ENGINE_API Scene{
		public:
			Scene(const Ref<Renderer> &renderer);
			~Scene();
		
			static Ref<Scene> create(const Ref<Renderer> &renderer) {return createRef<Scene>(renderer);}
			static Ref<Scene> copy(const Ref<Scene> &scene);

			Entity createEntity(const std::string &name = "new Entity");
			Entity createIDEntity(const std::string &name = "new Entity");
			Entity createIDEntity(uint64_t id, const std::string &name = "new Entity");

			Entity duplicateEntity(Entity entity);
			void destroyEntity(Entity entity);
			void clear();

			void OnUpdateRuntime(Timestep dt);
			void OnUpdateEditor(const EditorCamera &camera);
			void OnRuntimeStart();
			void OnRuntimeStop();
			void OnRuntimePause();
			void OnRuntimePlay();

			void setViewportSize(uint32_t width, uint32_t height);
			
			uint32_t getViewportWidth() const {return viewportWidth;}
			uint32_t getViewportHeight() const {return viewportHeight;}

			Entity getPrimaryCamera();
			
			entt::registry& getENTTRegistry() {return registry;}

			void OnSpriteComponentAdded(Entity entity);
			void OnSpriteComponentRemoved(Entity entity);

			void moveSpriteToBack(Entity entity); // move the entity to the front of the sprites queue
			void moveSpriteToTop(Entity entity); // move the entity to the back of the sprite queue
			void moveSpriteForward(Entity entity); // move the entity by one unit to the front of the queue
			void moveSpriteBackward(Entity entity); // move the entity bo one unit to the back of the queue

			glm::vec2 &getGravity() {return gravity;}

		private:
			void drawSprites();
			void drawEntityChilds(Entity parent, const glm::mat4 &parentTransform);

			entt::registry registry;
			Ref<Renderer> renderer;

			// containe all the sprite with a rendering priority
			// the fisrt element of the queue is rendered is first and so it's at the back of the scene
			// it's also useful for a rendering on a separate thread
			Set<UUID> sprites;

			uint32_t viewportWidth;
			uint32_t viewportHeight;

			std::unordered_map<UUID, Entity>* entityIDMap;

			// runtime an Box2D
			b2World* physicsWorld = nullptr;
			glm::vec2 gravity = {0.0f, -9.8f}; // default hearth gravity
			bool runtimeStopped = false;
			std::unordered_map<UUID, void*> physicsBodies;

			int32_t velocityIteration = 6;
			int32_t positionIteration = 2;
			
			// === methods ===


			friend class Entity;
			friend class SceneSerializer;
	};
}
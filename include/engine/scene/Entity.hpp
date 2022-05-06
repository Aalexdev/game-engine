#pragma once

#include "../core.hpp"
#include "../debug/Log.hpp"
#include "Scene.hpp"
#include "../dataStruct/Set.hpp"

#include <libs/entt/entt.hpp>

namespace engine{
	class ENGINE_API Entity{
		public:
			Entity(entt::entity handle, Scene *scene);
			Entity(uint32_t handle, Scene *scene) : handle{static_cast<entt::entity>(handle)}, scene{scene}{}
			Entity(const Entity &) = default;
			Entity() = default;

			~Entity(){
				OnParentRemoved();
			}
			
			template<typename T>
			bool hasComponent(){
				return scene->registry.all_of<T>(handle);
			}

			template<typename T, typename ...Args>
			T& addComponent(Args&& ... args){
				if (hasComponent<T>()) return getComponent<T>();
				T& component = scene->registry.emplace<T>(handle, std::forward<Args>(args)...);
				if (typeid(T) == typeid(components::Sprite)) scene->OnSpriteComponentAdded(*this);
				return component;
			}

			template<typename T>
			void removeComponent(){
				if (!hasComponent<T>()) return;
				if (typeid(T) == typeid(components::Sprite)) scene->OnSpriteComponentRemoved(*this);
				scene->registry.erase<T>(handle);
			}

			template<typename T>
			T& getComponent(){
				ENGINE_CORE_ASSERT(hasComponent<T>(), "entity does not have component");
				return scene->registry.get<T>(handle);
			}

			void OnParentAdded(Entity parent){
				ENGINE_CORE_ASSERT(parent == nullptr, "cannt add two parents on the same entity");
				this->parent = new Entity(parent);
			}

			void OnParentRemoved(){
				if (parent){
					delete parent;
				}
			}

			Entity addChild(Entity child){
				childs.pushBack(child);
				child.OnParentAdded(*this);
				return child;
			}

			Entity addChild(){
				return addChild(scene->createIDEntity());
			}

			bool isChild(){
				return parent != nullptr;
			}

			bool isParent(){
				return !childs.empty();
			}

			Set<Entity>& getChilds() {return childs;}

			inline void release(){handle = entt::null;}
			
			operator bool() const {return (handle != entt::null && scene->getENTTRegistry().valid(handle));}
			operator uint32_t() const {return static_cast<uint32_t>(handle);}
			operator entt::entity() const {return handle;}

			inline void operator=(entt::entity entity) {handle = entity;}
			inline bool operator==(const Entity &other) const {return handle == other.handle;}

			Scene* getScene() const {return scene;}
			
		private:
			entt::entity handle = entt::null;
			Set<Entity> childs;
			Entity* parent = nullptr;
			Scene* scene = nullptr;
	};

	inline std::ostream& operator<<(std::ostream& os, Entity e){
		return os << static_cast<uint32_t>(e);
	}
}
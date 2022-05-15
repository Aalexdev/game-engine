#pragma once

#include "../core.hpp"
#include "../debug/Log.hpp"
#include "Scene.hpp"
#include "../dataStruct/Set.hpp"
#include "ECS.hpp"

namespace engine{
	class ENGINE_API Entity{
		public:
			Entity(ECS::Entity handle, Scene *scene) : handle{handle}, scene{scene}{}
			Entity(const Entity &) = default;
			Entity() = default;

			class ChildIterator{
				public:
					ChildIterator(std::vector<ECS::Entity>::iterator it, Scene* scene) : it{it}, scene{scene}{}
					std::vector<ECS::Entity>::iterator getIterator() const {return it;}

					ChildIterator operator++() {return ChildIterator(it++, scene);}
					ChildIterator operator--() {return ChildIterator(it--, scene);}
					Entity operator*() {return {*it, scene};}
					Entity operator->() {return {*it, scene};}
					bool operator!=(const ChildIterator &other) {return other.it != it;}

				private:
					std::vector<ECS::Entity>::iterator it;
					Scene *scene;
			};
			
			template<typename T>
			bool hasComponent(){
				return scene->getRegistry().hasComponent<T>(handle);
			}

			template<typename T, typename ...Args>
			T& addComponent(Args&& ... args){
				if (hasComponent<T>()) return getComponent<T>();
				return scene->getRegistry().addComponent<T>(handle, {std::forward<Args>(args)...});;
			}

			template<typename T>
			void removeComponent(){
				if (!hasComponent<T>()) return;
				scene->getRegistry().removeComponent<T>(handle);
			}

			template<typename T>
			T& getComponent(){
				return scene->getRegistry().getComponent<T>(handle);
			}

			inline void release(){
				handle = ECS::null;
			}

			ECS::components::EntityData& data(){
				return scene->getRegistry().getData(handle);
			}

			UUID &getUUID(){
				return data().uuid;
			}

			std::string &getTag(){
				return data().tag;
			}

			Entity getParent(){
				return {data().parent, scene};
			}

			bool hasParent(){
				return data().parent != ECS::null;
			}

			bool hasChildren(){
				return !data().childs.empty();
			}

			void setParent(Entity parent);
			void resetParent();
			bool isParent(Entity parent);
			bool isChild(Entity child);
			void pushChild(Entity child);

			ChildIterator begin() {return {data().childs.begin(), scene};}
			ChildIterator end() {return {data().childs.end(), scene};}

			
			operator bool() const {return (handle != ECS::null && ECS::valid(handle));}
			operator ECS::Entity() const {return handle;}

			inline void operator=(ECS::Entity entity) {handle = entity;}
			inline bool operator==(const Entity &other) const {return handle == other.handle;}

			Scene* getScene() const {return scene;}
			
		private:
			ECS::Entity handle = ECS::null;
			Scene* scene = nullptr;
	};

	inline std::ostream& operator<<(std::ostream& os, Entity e){
		return os << static_cast<uint32_t>(e);
	}
}
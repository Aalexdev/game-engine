#pragma once
// =============================================================================
// ECS
// architecture from Austin Morlan : https://austinmorlan.com/posts/entity_component_system/
// =============================================================================

#include "../pch.hpp"
#include "../debug/Log.hpp"
#include "../core.hpp"
#include "../UUID.hpp"

namespace engine::ECS{
	/**
	 * @brief set the entity and componenent type, which also define the max physical count of entities and component
	 */
	using Entity = std::uint32_t;
	using ComponentType = std::uint8_t;

	static constexpr Entity null = 0;
	static constexpr Entity MAX_ENTITIES = 1024; // thoses values are just exemples and can be change
	static constexpr ComponentType MAX_COMPONENT = 32;

	/**
	 * @brief used to define entites components, for an example, is an entity has Transfoorm (type 1) RigidBody (type 2) and Gravity (type 3) the bitset will be 0b111
	 * each bits represent a component, and the MAX_COMPONENENT delclaration define the lenght of the bitset.
	 * as an example a bitset of lenght 8 will be like 0b00000000 with 8 zeros
	 */
	using Signature = std::bitset<MAX_COMPONENT>; 

	static inline constexpr bool valid(Entity entity){
		return entity != null && entity < MAX_ENTITIES;
	}

	namespace components{
		struct EntityData{
			std::string tag = "new entity";
			UUID uuid;

			Entity parent = null;
			std::vector<Entity> childs{};

			EntityData() = default;
		};
	}

	/**
	* @brief the mother class used for any component Array
	* 
	*/
	class IComponentArray{
		public:
			virtual ~IComponentArray() = default;
			virtual void entityDestroyed(Entity entity) = 0;
	};

	template<typename T>
	class ComponentArray : public IComponentArray{
		public:
			void insert(Entity entity, T component){
				ENGINE_CORE_ASSERT(_entityToIndexMap.find(entity) == _entityToIndexMap.end(), "Component added to same entity more than once. : ", typeid(T).name());

				// Put new entry at end and update the maps
				size_t newIndex = _size;
				_entityToIndexMap[entity] = newIndex;
				_indexToEntityMap[newIndex] = entity;
				_componentArray[newIndex] = component;
				++_size;
			}

			void remove(Entity entity){
				ENGINE_CORE_ASSERT(_entityToIndexMap.find(entity) != _entityToIndexMap.end(), "Removing non-existent component. : ", typeid(T).name());

				// Copy element at end into deleted element's place to maintain density
				size_t indexOfRemovedEntity = _entityToIndexMap[entity];
				size_t indexOfLastElement = _size - 1;
				_componentArray[indexOfRemovedEntity] = _componentArray[indexOfLastElement];

				// Update map to point to moved spot
				Entity entityOfLastElement = _indexToEntityMap[indexOfLastElement];
				_entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
				_indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

				_entityToIndexMap.erase(entity);
				_indexToEntityMap.erase(indexOfLastElement);

				--_size;
			}

			T& get(Entity entity){
				ENGINE_CORE_ASSERT(has(entity), "Retrieving non-existent component. : ", typeid(T).name());

				// Return a reference to the entity's component
				return _componentArray[_entityToIndexMap[entity]];
			}

			void entityDestroyed(Entity entity) override{
				if (_entityToIndexMap.find(entity) != _entityToIndexMap.end()){
					// Remove the entity's component if it existed
					remove(entity);
				}
			}

			bool has(Entity entity){
				return _entityToIndexMap.find(entity) != _entityToIndexMap.end();
			}
		
		private:
			/**
			* @brief the packed array (no holes) of generic type T
			* set the maximal entity count
			*/
			std::array<T, MAX_ENTITIES> _componentArray{};

			/**
			* @brief map entity id to array index
			*/
			std::unordered_map<Entity, std::size_t> _entityToIndexMap{};

			/**
			* @brief map array index to entity id
			* the invers of the _entotyToIndexMap
			*/
			std::unordered_map<std::size_t, Entity> _indexToEntityMap{};

			/**
			* @brief total size of entries in the array
			*/
			std::size_t _size;
	};

	class ComponentManager{
		public:
			ComponentManager() = default;
			ComponentManager(const ComponentManager &) = default;
		
			template<typename T> 
			void registerComponent(){
				size_t id = typeid(T).hash_code();

				ENGINE_CORE_ASSERT(_ComponentTypes.find(id) == _ComponentTypes.end(), "Registering component type more than once. : ", typeid(T).name());

				// Add this component type to the component type map
				_ComponentTypes[id] = _NextComponentType;

				// Create a ComponentArray pointer and add it to the component arrays map
				_componentArrays[id] = std::make_shared<ComponentArray<T>>();

				// Increment the value so that the next component registered will be different
				_NextComponentType++;
			}

			template<typename T>
			ComponentType getComponentType(){
				size_t id = typeid(T).hash_code();

				ENGINE_CORE_ASSERT(_ComponentTypes.find(id) != _ComponentTypes.end(), "Component not registered before use. : ", typeid(T).name());

				// Return this component's type - used for creating signatures
				return _ComponentTypes[id];
			}

			template<typename T>
			void addComponent(Entity entity, T component){
				// Add a component to the array for an entity
				getComponentArray<T>()->insert(entity, component);
			}

			template<typename T>
			void removeComponent(Entity entity){
				// Remove a component from the array for an entity
				getComponentArray<T>()->remove(entity);
			}

			template<typename T>
			T& getComponent(Entity entity){
				// Get a reference to a component from the array for an entity
				return getComponentArray<T>()->get(entity);
			}

			void entityDestroyed(Entity entity){
				// Notify each component array that an entity has been destroyed
				// If it has a component for that entity, it will remove it
				for (auto const& pair : _componentArrays){
					auto const& component = pair.second;

					component->entityDestroyed(entity);
				}
			}

			template<typename T>
			bool hasComponent(Entity entity){
				auto componentArray = getComponentArray<T>();
				if (!componentArray) return false;
				return componentArray->has(entity);
			}

		private:
			// Map from type string pointer to a component type
			std::unordered_map<size_t, ComponentType> _ComponentTypes{};

			// Map from type string pointer to a component array
			std::unordered_map<size_t, std::shared_ptr<IComponentArray>> _componentArrays{};

			// The component type to be assigned to the next registered component - starting at 0
			ComponentType _NextComponentType{};

			// Convenience function to get the statically casted pointer to the ComponentArray of type T.
			template<typename T>
			std::shared_ptr<ComponentArray<T>> getComponentArray(){
				size_t id = typeid(T).hash_code();
				ENGINE_CORE_ASSERT(_ComponentTypes.find(id) != _ComponentTypes.end(), "Component not registered before use. : ", typeid(T).name());
				return std::static_pointer_cast<ComponentArray<T>>(_componentArrays[id]);
			}
	};


	class EntityManager{
		public:
			EntityManager(){
				// fill the available entity queue with ids
				for (Entity entity = 1; entity<MAX_ENTITIES; entity++)
					_availablesEntities.push(entity);
			}

			/**
			* @brief create an entity and return he's id
			* @warning need to be destroyed 
			* @return Entity id
			*/
			Entity create(){
				ENGINE_CORE_ASSERT(_livingEntityCount < MAX_ENTITIES, "Too many entites in existence.");

				// take the fisrt available id from the available entity queue
				Entity id = _availablesEntities.front();
				_availablesEntities.pop();
				_livingEntityCount++;

				entities.insert(id);

				// return the id
				return id;
			}

			/**
			* @brief destroy the given entity, it will no more be managed by the Entity Manager
			* @param entity the entity to destroy
			*/
			void destroy(Entity entity){
				ENGINE_CORE_ASSERT(entity < MAX_ENTITIES, "Entiity out of range.");

				entities.erase(entity);

				// reset the entity's components
				_signatures[entity].reset();

				// and push the new availble entity id into the queue
				_availablesEntities.push(entity);
				_livingEntityCount--;
			}

			/**
			* @brief Set the Signature of the given entity
			* @param entity the targeted entity
			* @param signature the new signature state of the entity
			*/
			void setSignature(Entity entity, Signature signature){
				ENGINE_CORE_ASSERT(entity < MAX_ENTITIES, "Entity out of range.");
				_signatures[entity] = signature;
			}

			/**
			* @brief Get the Signature of the given entity
			* @param entity the targeted entity
			* @return the signature of the entity as a bitfield with the size of EntityManager::MAX_COMPONENT
			*/
			Signature getSignature(Entity entity){
				ENGINE_CORE_ASSERT(entity < MAX_ENTITIES, "Entity out of range.");
				return _signatures[entity];
			}

			inline Entity getLivingCount() const {
				return _livingEntityCount;
			}

			std::set<Entity>::iterator begin() {return entities.begin();}
			std::set<Entity>::iterator end() {return entities.end();}

		private:

			/**
			* @brief the queue of available entities id to avoid holes in the entities array
			*/
			std::queue<Entity> _availablesEntities{};

			/**
			* @brief the array who store the entities signature
			*/
			std::array<Signature, MAX_ENTITIES> _signatures{};

			/**
			* @brief the list of all living entities
			*/
			std::set<Entity> entities;

			/**
			* @brief the count of entities handeled by the manager
			* used to keep a limit about entities count 
			*/
			Entity _livingEntityCount{};
	};

	class System{
		public:
			std::set<Entity> entities;

			virtual void init() = 0;
			virtual void OnEntityPushed(Entity entity){}
			virtual void OnEntityRemoved(Entity entity){}
	};

	class SystemManager{
		public:

			template<typename T, typename... _Args>
			std::shared_ptr<T> registerSystem(_Args&&... __args) {
				size_t id = typeid(T).hash_code();

				ENGINE_CORE_ASSERT(mSystems.find(id) == mSystems.end(), "Registering system more than once. : ", typeid(T).name());

				// Create a pointer to the system and return it so it can be used externally
				auto system = std::make_shared<T>(std::forward<_Args>(__args)...);
				mSystems[id] = system;
				system->init();
				return system;
			}

			template<typename T>
			void setSignature(Signature signature){
				size_t id = typeid(T).hash_code();

				ENGINE_CORE_ASSERT(mSystems.find(id) != mSystems.end(), "System used before registered. : ", typeid(T).name());

				// Set the signature for this system
				mSignatures[id] = signature;
			}

			void entityDestroyed(Entity entity){
				// Erase a destroyed entity from all system lists
				// mEntities is a set so no check needed
				for (auto const& pair : mSystems)
				{
					auto const& system = pair.second;

					system->OnEntityRemoved(entity);
					system->entities.erase(entity);
				}
			}

			void EntitySignatureChanged(Entity entity, Signature entitySignature){
				// Notify each system that an entity's signature changed
				for (auto const& pair : mSystems)
				{
					auto const& type = pair.first;
					auto const& system = pair.second;
					auto const& systemSignature = mSignatures[type];

					// Entity signature matches system signature - insert into set
					if ((entitySignature & systemSignature) == systemSignature){
						system->entities.insert(entity);
						system->OnEntityPushed(entity);
					}
					// Entity signature does not match system signature - erase from set
					else {
						system->OnEntityRemoved(entity);
						system->entities.erase(entity);
					}
				}
			}

		private:
			// Map from system type string pointer to a signature
			std::unordered_map<size_t, Signature> mSignatures{};

			// Map from system type string pointer to a system pointer
			std::unordered_map<size_t, std::shared_ptr<System>> mSystems{};
	};

	class Registry{
		public:
			Registry() {Init();}
			~Registry() = default;
			
			void Init(){
				// Create pointers to each manager
				_componentManager = std::make_unique<ComponentManager>();
				_entityManager = std::make_unique<EntityManager>();
				_systemManager = std::make_unique<SystemManager>();

				registerComponent<components::EntityData>();
			}

			// Entity methods
			Entity create(){
				Entity entity = _entityManager->create();
				addComponent<components::EntityData>(entity);
				return entity;
			}

			Entity create(Entity parent){
				Entity entity = _entityManager->create();
				auto &data = addComponent<components::EntityData>(entity);
				data.parent = parent;

				// push the entity in the parents childs
				auto &parentData = getComponent<components::EntityData>(parent);
				parentData.childs.push_back(entity);
				return entity;
			}

			void destroy(Entity entity){

				// delete the childs

				auto &data = getData(entity);
				for (auto &child : data.childs){
					destroy(child);
				}

				_systemManager->entityDestroyed(entity);
				_componentManager->entityDestroyed(entity);
				_entityManager->destroy(entity);

				
			}


			// Component methods
			template<typename T>
			void registerComponent(){
				_componentManager->registerComponent<T>();
			}

			template<typename T>
			T& addComponent(Entity entity, T component = {}){
				_componentManager->addComponent<T>(entity, component);

				auto signature = _entityManager->getSignature(entity);
				signature.set(_componentManager->getComponentType<T>(), true);
				_entityManager->setSignature(entity, signature);

				_systemManager->EntitySignatureChanged(entity, signature);
				return getComponent<T>(entity);
			}

			template<typename T>
			void removeComponent(Entity entity) {
				_componentManager->removeComponent<T>(entity);

				auto signature = _entityManager->getSignature(entity);
				signature.set(_componentManager->getComponentType<T>(), false);
				_entityManager->setSignature(entity, signature);

				_systemManager->EntitySignatureChanged(entity, signature);
			}

			template<typename T>
			T& getComponent(Entity entity){
				return _componentManager->getComponent<T>(entity);
			}

			template<typename T>
			ComponentType getComponentType(){
				return _componentManager->getComponentType<T>();
			}

			// System methods
			template<typename T, typename... _Args>
			std::shared_ptr<T> RegisterSystem(_Args&&... __args){
				return _systemManager->registerSystem<T>(std::forward<_Args>(__args)...);
			}

			template<typename T>
			void setSystemSignature(Signature signature){
				_systemManager->setSignature<T>(signature);
			}

			std::set<Entity>::iterator begin(){
				return _entityManager->begin();
			}

			std::set<Entity>::iterator end(){
				return _entityManager->end();
			}

			components::EntityData &getData(Entity entity){
				return getComponent<components::EntityData>(entity);
			}

			template<typename T>
			bool hasComponent(Entity entity){
				return _componentManager->hasComponent<T>(entity);
			}

		private:
			std::unique_ptr<ComponentManager> _componentManager;
			std::unique_ptr<EntityManager> _entityManager;
			std::unique_ptr<SystemManager> _systemManager;
	};
}
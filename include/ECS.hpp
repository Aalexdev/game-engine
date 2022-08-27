#pragma once
// =============================================================================
// ECS
// =============================================================================

#include <unordered_map>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <memory>
#include <bitset>
#include <array>
#include <queue>
#include <set>

namespace ECS{
    /**
     * @brief set the entity and componenent type, which also define the max physical count of entities and component
     */
    using Entity = std::uint32_t;
    using ComponentType = std::uint8_t;

    static constexpr Entity MAX_ENTITIES = 1024; // thoses values are just exemples and can be change
    static constexpr ComponentType MAX_COMPONENT = 32;

    /**
     * @brief used to define entites components, for an example, is an entity has Transfoorm (type 1) RigidBody (type 2) and Gravity (type 3) the bitset will be 0b111
     * each bits represent a component, and the MAX_COMPONENENT delclaration define the lenght of the bitset.
     * as an example a bitset of lenght 8 will be like 0b00000000 with 8 zeros
     */
    using Signature = std::bitset<MAX_COMPONENT>; 

    class ComponentArray{
        public:
			ComponentArray(size_t componentSize) : componentSize{componentSize}{}
			ComponentArray() = delete;

            void InsertData(Entity entity, void* component){
                assert(_entityToIndexMap.find(entity) == _entityToIndexMap.end() && "Component added to same entity more than once.");

                // Put new entry at end and update the maps
                size_t newIndex = _size;
                _entityToIndexMap[entity] = newIndex;
                _indexToEntityMap[newIndex] = entity;
                _componentArray[newIndex] = malloc(componentSize);
				if (component) memcpy(_componentArray[newIndex], component, componentSize);
                ++_size;
            }

            void RemoveData(Entity entity){
                assert(_entityToIndexMap.find(entity) != _entityToIndexMap.end() && "Removing non-existent component.");

                // Copy element at end into deleted element's place to maintain density
                size_t indexOfRemovedEntity = _entityToIndexMap[entity];
                size_t indexOfLastElement = _size - 1;
				free(_componentArray[indexOfRemovedEntity]);
                _componentArray[indexOfRemovedEntity] = _componentArray[indexOfLastElement];

                // Update map to point to moved spot
                Entity entityOfLastElement = _indexToEntityMap[indexOfLastElement];
                _entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
                _indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

                _entityToIndexMap.erase(entity);
                _indexToEntityMap.erase(indexOfLastElement);

                --_size;
            }

            void* GetData(Entity entity){
                assert(_entityToIndexMap.find(entity) != _entityToIndexMap.end() && "Retrieving non-existent component.");

                // Return a reference to the entity's component
                return _componentArray[_entityToIndexMap[entity]];
            }

			bool HasComponent(Entity entity){
				return _entityToIndexMap.find(entity) != _entityToIndexMap.end();
			}

            void EntityDestroyed(Entity entity){
                if (_entityToIndexMap.find(entity) != _entityToIndexMap.end()){
                    // Remove the entity's component if it existed
                    RemoveData(entity);
                }
            }
        
        private:
            /**
             * @brief the packed array (no holes) of generic type T
             * set the maximal entity count
             */
            std::array<void*, MAX_ENTITIES> _componentArray;

            /**
             * @brief map entity id to array index
             */
            std::unordered_map<Entity, std::size_t> _entityToIndexMap;

            /**
             * @brief map array index to entity id
             * the invers of the _entotyToIndexMap
             */
            std::unordered_map<std::size_t, Entity> _indexToEntityMap;

            /**
             * @brief total size of entries in the array
             */
            std::size_t _size;

			size_t componentSize;
    };

    class ComponentManager{
        public:
            void RegisterComponent(size_t typeID, size_t componentSize){
                assert(_ComponentTypes.find(typeID) == _ComponentTypes.end() && "Registering component type more than once.");

                // Add this component type to the component type map
                _ComponentTypes.insert({typeID, _NextComponentType});

                // Create a ComponentArray pointer and add it to the component arrays map
                _componentArrays.insert({typeID, std::make_shared<ComponentArray>(componentSize)});

                // Increment the value so that the next component registered will be different
                _NextComponentType++;
            }

            ComponentType GetComponentType(size_t typeID){
                assert(_ComponentTypes.find(typeID) != _ComponentTypes.end() && "Component not registered before use.");

                // Return this component's type - used for creating signatures
                return _ComponentTypes[typeID];
            }

            void AddComponent(Entity entity, void* component, size_t typeID){
                // Add a component to the array for an entity
                GetComponentArray(typeID)->InsertData(entity, component);
            }

            void RemoveComponent(Entity entity, size_t typeID){
                // Remove a component from the array for an entity
                GetComponentArray(typeID)->RemoveData(entity);
            }

            void* GetComponent(Entity entity, size_t typeID){
                // Get a reference to a component from the array for an entity
                return GetComponentArray(typeID)->GetData(entity);
            }

			bool HasComponent(Entity entity, size_t typeID){
				return GetComponentArray(typeID)->HasComponent(entity);
			}

            void EntityDestroyed(Entity entity){
                // Notify each component array that an entity has been destroyed
                // If it has a component for that entity, it will remove it
                for (auto const& pair : _componentArrays){
                    auto const& component = pair.second;

                    component->EntityDestroyed(entity);
                }
            }

        private:
            // Map from type hash code to a component type
            std::unordered_map<size_t, ComponentType> _ComponentTypes{};

            // Map from type hash code to a component array
            std::unordered_map<size_t, std::shared_ptr<ComponentArray>> _componentArrays{};

            // The component type to be assigned to the next registered component - starting at 0
            ComponentType _NextComponentType{};

            // Convenience function to get the statically casted pointer to the ComponentArray of type T.
            std::shared_ptr<ComponentArray> GetComponentArray(size_t typeID){
                assert(_ComponentTypes.find(typeID) != _ComponentTypes.end() && "Component not registered before use.");
                return std::static_pointer_cast<ComponentArray>(_componentArrays[typeID]);
            }
    };

    class EntityManager{
        public:
            EntityManager(){
                // fill the available entity queue with ids
                for (Entity entity = 0; entity<MAX_ENTITIES; entity++)
                    _availablesEntities.push(entity);
            }

            /**
             * @brief create an entity and return he's id
             * @warning need to be destroyed 
             * @return Entity id
             */
            Entity create(){
                assert(_livingEntityCount < MAX_ENTITIES && "Too many entites in existence.");

                // take the fisrt available id from the available entity queue
                Entity id = _availablesEntities.front();
                _availablesEntities.pop();
                _livingEntityCount++;

                // return the id
                return id;
            }

            /**
             * @brief destroy the given entity, it will no more be managed by the Entity Manager
             * @param entity the entity to destroy
             */
            void destroy(Entity entity){
                assert(entity < MAX_ENTITIES && "Entiity out of range.");

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
                assert(entity < MAX_ENTITIES && "Entity out of range.");
                _signatures[entity] = signature;
            }

            /**
             * @brief Get the Signature of the given entity
             * @param entity the targeted entity
             * @return the signature of the entity as a bitfield with the size of EntityManager::MAX_COMPONENT
             */
            Signature getSignature(Entity entity){
                assert(entity < MAX_ENTITIES && "Entity out of range.");
                return _signatures[entity];
            }

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
             * @brief the count of entities handeled by the manager
             * used to keep a limit about entities count 
             */
            std::uint32_t _livingEntityCount{};
    };

    class System{
        public:
            std::set<Entity> mEntities;
    };

    class SystemManager{
        public:
			~SystemManager(){
				for (auto &s : mSystems){
					delete s.second;
				}
				mSystems.clear();
			}
			
            void RegisterSystem(size_t typeID, System *system) {
                assert(mSystems.find(typeID) == mSystems.end() && "Registering system more than once.");

                // Create a pointer to the system and return it so it can be used externally
                mSystems.insert({typeID, system});
            }

            void SetSignature(size_t typeID, Signature signature){
                assert(mSystems.find(typeID) != mSystems.end() && "System used before registered.");

                // Set the signature for this system
                mSignatures.insert({typeID, signature});
            }

            void EntityDestroyed(Entity entity){
                // Erase a destroyed entity from all system lists
                // mEntities is a set so no check needed
                for (auto const& pair : mSystems)
                {
                    auto const& system = pair.second;

                    system->mEntities.erase(entity);
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
                    if ((entitySignature & systemSignature) == systemSignature)
                    {
                        system->mEntities.insert(entity);
                    }
                    // Entity signature does not match system signature - erase from set
                    else
                    {
                        system->mEntities.erase(entity);
                    }
                }
            }

        private:
            // Map from system type string pointer to a signature
            std::unordered_map<size_t, Signature> mSignatures{};

            // Map from system type string pointer to a system pointer
            std::unordered_map<size_t, System*> mSystems{};
    };

    class Coordinator{
        public:
            Coordinator() {Init();}

            void Init(){
                // Create pointers to each manager
                _componentManager = std::make_unique<ComponentManager>();
                _entityManager = std::make_unique<EntityManager>();
                _systemManager = std::make_unique<SystemManager>();
            }

            // Entity methods
            Entity CreateEntity(){
                return _entityManager->create();
            }

            void DestroyEntity(Entity entity){
                _entityManager->destroy(entity);

                _componentManager->EntityDestroyed(entity);

                _systemManager->EntityDestroyed(entity);
            }

            // Component methods
            void RegisterComponent(size_t typeID, size_t componentSize){
                _componentManager->RegisterComponent(typeID, componentSize);
            }

            void AddComponent(Entity entity, void* component, size_t typeID){
                _componentManager->AddComponent(entity, component, typeID);

                auto signature = _entityManager->getSignature(entity);
                signature.set(_componentManager->GetComponentType(typeID), true);
                _entityManager->setSignature(entity, signature);

                _systemManager->EntitySignatureChanged(entity, signature);
            }

            void RemoveComponent(Entity entity, size_t typeID){
                _componentManager->RemoveComponent(entity, typeID);

                auto signature = _entityManager->getSignature(entity);
                signature.set(_componentManager->GetComponentType(typeID), false);
                _entityManager->setSignature(entity, signature);

                _systemManager->EntitySignatureChanged(entity, signature);
            }

            void* GetComponent(Entity entity, size_t typeID){
                return _componentManager->GetComponent(entity, typeID);
            }

			bool HasComponent(Entity entity, size_t typeID){
				return _componentManager->HasComponent(entity, typeID);
			}

            ComponentType GetComponentType(size_t typeID){
                return _componentManager->GetComponentType(typeID);
            }

            // System methods
            void RegisterSystem(size_t typeID, System *system){
                return _systemManager->RegisterSystem(typeID, system);
            }

            void SetSystemSignature(size_t typeID, Signature signature){
                _systemManager->SetSignature(typeID, signature);
            }

        private:
            std::unique_ptr<ComponentManager> _componentManager;
            std::unique_ptr<EntityManager> _entityManager;
            std::unique_ptr<SystemManager> _systemManager;
    };
}
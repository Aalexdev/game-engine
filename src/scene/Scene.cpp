#include "engine/core.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/scene/Scene.hpp"
#include "engine/scene/Entity.hpp"
#include "engine/scene/BatchRenderer.hpp"

#include "engine/scene/components/TransformComponent.hpp"
#include "engine/scene/components/SpriteComponent.hpp"
#include "engine/scene/components/CameraComponent.hpp"
#include "engine/scene/components/RigidBodyComponent.hpp"
#include "engine/scene/components/BoxColliderComponent.hpp"
#include "engine/scene/components/CircleRendererComponent.hpp"
#include "engine/scene/components/TriangleRenderer.hpp"
#include "engine/scene/components/CircleColliderComponent.hpp"
#include "engine/scene/components/DistanceJointComponent.hpp"
#include "engine/scene/components/SpringJointComponent.hpp"
#include "engine/scene/components/RevoluteJoint.hpp"
#include "engine/scene/components/PrismaticJoint.hpp"
#include "engine/scene/components/FrictionJointComponent.hpp"

#include <libs/yaml-cpp/yaml.h>
#include <libs/glm/gtc/epsilon.hpp>
#include <libs/box2d/box2d.h>

template<>
struct std::hash<engine::Entity>{
    std::size_t operator()(engine::Entity const& e) const noexcept{
        return static_cast<uint32_t>(e);
    }
};

namespace engine{

	template<typename T>
	static void copyComponent(Entity dst, Entity src){
		if (src.hasComponent<T>()){
			if (dst.hasComponent<T>()){
				dst.getComponent<T>() = src.getComponent<T>();
				return;
			}

			dst.addComponent<T>(src.getComponent<T>());
		}
	}

	static void copyComponents(Entity dst, Entity src){
		copyComponent<ECS::components::EntityData>(dst, src);
		copyComponent<ECS::components::Transform>(dst, src);
		copyComponent<ECS::components::Camera>(dst, src);
		copyComponent<ECS::components::RigidBody>(dst, src);
		copyComponent<ECS::components::Sprite>(dst, src);
		copyComponent<ECS::components::BoxCollider>(dst, src);
		copyComponent<ECS::components::CircleRenderer>(dst, src);
		copyComponent<ECS::components::TriangleRenderer>(dst, src);
		copyComponent<ECS::components::CircleCollider>(dst, src);
		copyComponent<ECS::components::DistanceJoint>(dst, src);
		copyComponent<ECS::components::SpringJoint>(dst, src);
		copyComponent<ECS::components::RevoluteJoint>(dst, src);
		copyComponent<ECS::components::PrismaticJoint>(dst, src);
		copyComponent<ECS::components::FrictionJoint>(dst, src);
	}
	
	Ref<Scene> Scene::copy(const Ref<Scene> &scene){
		Ref<Scene> newScene = Scene::create(scene->renderer, scene->physicMaterials);

		newScene->gravity = scene->gravity;
		newScene->positionIteration = scene->positionIteration;
		newScene->velocityIteration = scene->velocityIteration;
		newScene->runtimeStopped = scene->runtimeStopped;
		newScene->viewportHeight = scene->viewportHeight;
		newScene->viewportWidth = scene->viewportWidth;

		newScene->physicsBodies = scene->physicsBodies;

		for (const auto &e : scene->registry){
			Entity srcEntity = {e, scene.get()};
			Entity dstEntity = newScene->createEntity(srcEntity.getUUID(), srcEntity.getTag());

			copyComponents(dstEntity, srcEntity);
		}

		return newScene;
	}

	Scene::Scene(const Ref<Renderer> &renderer, const Ref<PhysicMaterialLibrary> &physicMaterials) : renderer{renderer}, physicMaterials{physicMaterials}{
		entityIDMap = new std::unordered_map<UUID, Entity>();

		registry.registerComponent<ECS::components::Camera>();
		registry.registerComponent<ECS::components::Transform>();
		registry.registerComponent<ECS::components::Sprite>();
		registry.registerComponent<ECS::components::CircleRenderer>();
		registry.registerComponent<ECS::components::TriangleRenderer>();
		registry.registerComponent<ECS::components::RigidBody>();
		registry.registerComponent<ECS::components::BoxCollider>();
		registry.registerComponent<ECS::components::CircleCollider>();
		registry.registerComponent<ECS::components::DistanceJoint>();
		registry.registerComponent<ECS::components::SpringJoint>();
		registry.registerComponent<ECS::components::RevoluteJoint>();
		registry.registerComponent<ECS::components::PrismaticJoint>();
		registry.registerComponent<ECS::components::FrictionJoint>();

		batchRenderer = createScope<BatchRenderer>(this);

		cameraSystem = registry.RegisterSystem<ECS::systems::Camera>(this);
		transformSystem = registry.RegisterSystem<ECS::systems::Transform>(this);
		rigidBodySystem = registry.RegisterSystem<ECS::systems::RigidBody>(this);
	}

	Scene::~Scene(){
		delete entityIDMap;
	}
	
	Entity Scene::createEntity(const std::string &name){
		// create an entity
		Entity entity = {registry.create(), this};

		// set the tag and register the entity in the entity ID map
		entity.getTag() = name;
		(*entityIDMap)[entity.getUUID()] = entity;

		// add the transform component
		entity.addComponent<ECS::components::Transform>();
		return entity;
	}

	Entity Scene::createEntity(UUID uuid, const std::string &name){

		// if there is already an entity with the given id, return it
		if ((*entityIDMap).find(uuid) != entityIDMap->end()){
			return (*entityIDMap)[uuid];
		}
		// else

		// create an entity
		Entity entity = {registry.create(), this};

		// set the tag and uuid, then register the entity in the entity ID map
		entity.getTag() = name;
		entity.getUUID() = uuid;

		(*entityIDMap)[uuid] = entity;

		// add the transform component
		entity.addComponent<ECS::components::Transform>();
		return entity;
	}

	Entity Scene::createChildEntity(Entity parent, const std::string &name){
		Entity child = {registry.create(), this};

		parent.pushChild(child);
		child.data().parent = parent;

		// set the tag and register the entity in the entity ID map
		child.getTag() = name;
		(*entityIDMap)[child.getUUID()] = child;

		// add the transform component
		child.addComponent<ECS::components::Transform>();
		return child;
	}

	Entity Scene::createChildEntity(Entity parent, UUID uuid, const std::string &name){
		if ((*entityIDMap).find(uuid) != entityIDMap->end()){
			Entity child = (*entityIDMap)[uuid];
			child.data().parent = parent;
			parent.pushChild(child);
			return child;
		}

		Entity child = {registry.create(), this};

		parent.pushChild(child);
		child.data().parent = parent;

		// set the tag and register the entity in the entity ID map
		child.getTag() = name;
		(*entityIDMap)[child.getUUID()] = child;

		// add the transform component
		child.addComponent<ECS::components::Transform>();
		return child;
	}

	void Scene::destroyEntity(Entity entity){
		registry.destroy(static_cast<ECS::Entity>(entity));
	}

	void Scene::OnUpdateRuntime(Timestep dt){
		
		SceneCamera *primaryCamera = cameraSystem->update();

		if (primaryCamera){
			renderer->beginScene(primaryCamera->getCamera());
		} else {
			// a camera with zero transformation
			renderer->beginScene(Camera());
		}

		rigidBodySystem->update(dt, velocityIteration, positionIteration);
		transformSystem->update();
		batchRenderer->render();
		rigidBodySystem->renderCollisions();
		renderer->endScene();
	}

	void Scene::OnUpdateEditor(const EditorCamera &camera){
		renderer->beginScene(camera.getCamera());
		transformSystem->update();
		batchRenderer->render();
		rigidBodySystem->renderCollisions();
		renderer->endScene();
	}

	void Scene::setViewportSize(uint32_t width, uint32_t height){
		viewportWidth = width;
		viewportHeight = height;
		cameraSystem->updateViewport(width, height);
	}

	void Scene::clear(){
		// registry.each([&](auto entity){
		// 	registry.destroy(entity);
		// });
		
		if (entityIDMap) entityIDMap->clear();
	}

	Entity Scene::duplicateEntity(Entity entity){
		if (!entity) return Entity();
		Entity clone = {registry.create(), this};

		copyComponents(clone, entity);
		return clone;
	}
	
	void Scene::OnRuntimeStart(){
		physicsWorld = new b2World({gravity.x, gravity.y});
		rigidBodySystem->start();
		runtimeStopped = false;
	}

	void Scene::OnRuntimeStop(){
		delete physicsWorld;
		physicsWorld = nullptr;
		runtimeStopped = true;
	}

	void Scene::OnRuntimePause(){
		runtimeStopped = true;
	}

	void Scene::OnRuntimePlay(){
		runtimeStopped = true;
		if (physicsWorld){
			runtimeStopped = false;
		}
	}

	Entity Scene::get(UUID id){
		auto it = entityIDMap->find(id);
		if (it == entityIDMap->end()) return {ECS::null, this};
		return it->second;
	}
	
	void* Scene::getPhysicsBody(UUID id){
		auto it = physicsBodies.find(id);
		if (it == physicsBodies.end()) return nullptr;
		return it->second;
	}

	b2World* Scene::getPhysicsWorld(){
		return physicsWorld;
	}

	void Scene::setPhysicBody(UUID id, void *body){
		physicsBodies[id] = body;
	}

	Ref<PhysicMaterialLibrary> Scene::getPhysicMaterials(){
		return physicMaterials;
	}
}
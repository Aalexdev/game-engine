#include "engine/core.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/scene/Scene.hpp"
#include "engine/scene/Entity.hpp"

#include "engine/scene/components/TagComponent.hpp"
#include "engine/scene/components/TransformComponent.hpp"
#include "engine/scene/components/SpriteComponent.hpp"
#include "engine/scene/components/CameraComponent.hpp"
#include "engine/scene/components/UUIDComponent.hpp"
#include "engine/scene/components/RigidBodyComponent.hpp"
#include "engine/scene/components/BoxColliderComponent.hpp"
#include "engine/scene/components/CircleRendererComponent.hpp"
#include "engine/scene/components/TriangleRenderer.hpp"
#include "engine/scene/components/HierarchyNodeComponent.hpp"

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
			dst.getScene()->getENTTRegistry().emplace_or_replace<T>(static_cast<entt::entity>(dst), src.getComponent<T>());
		}
	}

	static void copyComponents(Entity dst, Entity src){
		copyComponent<components::Transform>(dst, src);
		copyComponent<components::Camera>(dst, src);
		copyComponent<components::RigidBody>(dst, src);
		copyComponent<components::Sprite>(dst, src);
		copyComponent<components::BoxCollider>(dst, src);
		copyComponent<components::CircleRenderer>(dst, src);
		copyComponent<components::TriangleRenderer>(dst, src);
		copyComponent<components::HierarchyNode>(dst, src);
	}
	
	static b2BodyType ToB2Type(components::RigidBody::BodyType type){
		switch (type){
			case components::RigidBody::BodyType::STATIC: return b2BodyType::b2_staticBody;
			case components::RigidBody::BodyType::DYNAMIC: return b2BodyType::b2_dynamicBody;
			case components::RigidBody::BodyType::KINEMATIC: return b2BodyType::b2_kinematicBody;
		}
		ENGINE_CORE_ASSERT(false, "unknown type");
		return b2BodyType::b2_staticBody;
	}

	Ref<Scene> Scene::copy(const Ref<Scene> &scene){
		Ref<Scene> newScene = Scene::create(scene->renderer);

		newScene->sprites = Set<UUID>::copy(scene->sprites);
		newScene->gravity = scene->gravity;
		newScene->positionIteration = scene->positionIteration;
		newScene->runtimeStopped = scene->runtimeStopped;
		newScene->velocityIteration = scene->velocityIteration;
		newScene->viewportHeight = scene->viewportHeight;
		newScene->viewportWidth = scene->viewportWidth;

		newScene->physicsBodies = scene->physicsBodies;

		auto &srcRegistry = scene->registry;
		auto &dstRegistry = newScene->registry;

		auto idView = srcRegistry.view<components::UUID>();
		for (auto e : idView){
			Entity entity{e, scene.get()};

			auto &uuid = entity.getComponent<components::UUID>();
			const auto &tag = entity.getComponent<components::Tag>().tag;

			Entity newEntity = newScene->createIDEntity(uuid, tag);
			copyComponents(newEntity, entity);
		}
		
		return newScene;
	}


	Scene::Scene(const Ref<Renderer> &renderer) : renderer{renderer}{
		entityIDMap = new std::unordered_map<UUID, Entity>();
	}

	Scene::~Scene(){
		delete entityIDMap;
	}
	
	Entity Scene::createEntity(const std::string &name){
		Entity entity = {registry.create(), this};

		entity.addComponent<components::Tag>(name);
		entity.addComponent<components::Transform>();
		entity.addComponent<components::HierarchyNode>();

		return entity;
	}

	Entity Scene::createIDEntity(const std::string &name){
		Entity entity = createEntity(name);
		auto id = entity.addComponent<components::UUID>();

		if (entityIDMap->find(id) != entityIDMap->end()){ 
			return (*entityIDMap)[id];
		}
		(*entityIDMap)[id] = entity;
		
		absolutParents.pushBack(id);
		return entity;
	}

	Entity Scene::createIDEntity(uint64_t id, const std::string &name){
		UUID uuid(id);

		if (entityIDMap->find(uuid) != entityIDMap->end()){
			return (*entityIDMap)[uuid];
		}

		Entity entity = createEntity(name);
		entity.addComponent<components::UUID>(uuid);
		(*entityIDMap)[uuid] = entity;

		absolutParents.pushBack(uuid);

		return entity;
	}
	
	Entity Scene::createEntity(Entity parent){
		Entity entity = {registry.create(), this};
		auto id = entity.addComponent<components::UUID>();

		entity.addComponent<components::Tag>("new entity");
		entity.addComponent<components::Transform>();
		auto node = entity.addComponent<components::HierarchyNode>();

		node.parent = parent.getComponent<components::UUID>();
		parent.getComponent<components::HierarchyNode>().childs.push_back(id);

		(*entityIDMap)[id] = entity;
		return entity;
	}

	Entity Scene::createEntity(Entity parent, uint64_t id, const std::string &name){
		Entity entity = {registry.create(), this};
		auto uuid = entity.addComponent<components::UUID>(id);

		entity.addComponent<components::Tag>(name);
		entity.addComponent<components::Transform>();
		auto node = entity.addComponent<components::HierarchyNode>();

		node.parent = parent.getComponent<components::UUID>();
		parent.getComponent<components::HierarchyNode>().childs.push_back(uuid);

		(*entityIDMap)[id] = entity;
		return entity;
	}

	void Scene::destroyEntity(Entity entity){

		if (entity.hasComponent<components::UUID>()){
			UUID id = entity.getComponent<components::UUID>().id;

			auto &node = entity.getComponent<components::HierarchyNode>();

			for (auto &child : node.childs){
				destroyEntity((*entityIDMap)[child]);
			}

			entityIDMap->erase(id);
			sprites.erase(id);
		}

		
		registry.destroy(static_cast<entt::entity>(entity));
	}

	void Scene::OnUpdateRuntime(Timestep dt){
		
		SceneCamera *primaryCamera = nullptr;
		
		{
			float cameraPriority = 0.f;
			auto group = registry.group<components::Camera>(entt::get<components::Transform>);
			for (auto &entity : group){
				auto [camera, transform] = group.get<components::Camera, components::Transform>(entity);

				// get the primary camera
				if (camera.priorityLevel > cameraPriority){
					primaryCamera = &camera.camera;
					cameraPriority = camera.priorityLevel;
				}

				// update camera position
				camera.camera.setPosition(transform.translation);
				camera.camera.setRotation(transform.rotation);
			}
		}

		if (primaryCamera){
			renderer->beginScene(primaryCamera->getCamera());
		} else {
			// a camera with zero transformation
			renderer->beginScene(Camera());
		}

		updatePhysics(dt);
		updateTransform();
		drawSprites();

		renderer->endScene();
	}

	void Scene::OnUpdateEditor(const EditorCamera &camera){
		renderer->beginScene(camera.getCamera());
		updateTransform();
		drawSprites();
		renderer->endScene();
	}

	void Scene::updatePhysics(Timestep ts){
		if (physicsWorld){
			physicsWorld->Step(ts, velocityIteration, positionIteration);

			auto view = registry.view<components::RigidBody>();
			for (auto e : view){
				Entity entity = {e, this};
				
				auto &transform = entity.getComponent<components::Transform>();
				auto &uuid = entity.getComponent<components::UUID>().id;

				b2Body *body = reinterpret_cast<b2Body*>(physicsBodies[uuid]);
				const auto &pos = body->GetPosition();

				transform.translation = {pos.x, pos.y};
				transform.rotation = {body->GetAngle()};
			}
		}
	}

	void Scene::drawSprites(){
		auto it = sprites.begin();

		while (it){
			Entity entity = (*entityIDMap)[*it];
			ENGINE_CORE_ASSERT(entity.hasComponent<components::Transform>(), "invalid entity components");
			auto &transform = entity.getComponent<components::Transform>();

			if (entity.hasComponent<components::Sprite>()){
				auto &sprite = entity.getComponent<components::Sprite>();
				renderer->drawQuad(transform.transform, sprite.color, static_cast<uint32_t>(entity), sprite.texture);

			} else if (entity.hasComponent<components::TriangleRenderer>()){
				auto &tri = entity.getComponent<components::TriangleRenderer>();
				renderer->drawTriangle(tri.p1, tri.p2, tri.p3, transform.transform, tri.texture, static_cast<uint32_t>(entity));
			}

			it++;
		}

		{
			auto view = registry.view<components::Transform, components::CircleRenderer>();
			for (auto &e : view){
				auto [transform, circle] = view.get(e);
				
				renderer->drawCircle(transform.transform, circle.color, static_cast<uint32_t>(e), circle.thickness, circle.fade);
			}
		}
	}

	void Scene::updateTransform(){

		auto view = registry.view<components::Transform>();

		for (auto &e : view){
			Entity entity = {e, this};
			auto &transform = entity.getComponent<components::Transform>();
			auto &node = entity.getComponent<components::HierarchyNode>();

			transform.lastTransform = transform.transform;

			// if the quad dosen't have a rotation, it useless to calculate to rotation transformation
			if (glm::epsilonEqual(transform.rotation, 0.0f, glm::epsilon<float>())){
				transform.transform = glm::translate(glm::mat4(1.f), glm::vec3(transform.translation, 0.f)) * glm::scale(glm::mat4(1.f), glm::vec3(transform.scale, 1.f));
			} else {
				transform.transform = glm::translate(glm::mat4(1.f), glm::vec3(transform.translation, 0.f)) * glm::rotate(glm::mat4(1.f), transform.rotation, {0.f, 0.f, 1.f}) * glm::scale(glm::mat4(1.f), glm::vec3(transform.scale, 1.f));
			}

			if (transform.transform != transform.lastTransform){
				for (auto &child : node.childs){
					updatetransform((*entityIDMap)[child], transform.transform);
				}
			}
		}
	}

	void Scene::updatetransform(Entity entity, const glm::mat4 &parentTransform){
		auto &node = entity.getComponent<components::HierarchyNode>();
		auto &transform = entity.getComponent<components::Transform>();

		transform.transform = parentTransform * transform.transform;
		
		for (auto &child : node.childs){
			updatetransform((*entityIDMap)[child], transform.transform);
		}
	}

	void Scene::setViewportSize(uint32_t width, uint32_t height){
		viewportWidth = width;
		viewportHeight = height;

		float aspect = static_cast<float>(width) / static_cast<float>(height);

		auto view = registry.view<components::Camera>();
		for (auto entity : view){
			auto &camera = view.get<components::Camera>(entity);
			
			if (!camera.fixedViewportAspectRatio){
				camera.camera.setAspectRatio(aspect);
				camera.viewportWidth = width;
				camera.viewportHeight = height;
			}
		}		
	}

	void Scene::clear(){
		registry.each([&](auto entity){
			registry.destroy(entity);
		});
		
		if (entityIDMap) entityIDMap->clear();
		sprites.clear();
	}

	Entity Scene::getPrimaryCamera(){
		Entity primaryCamera;
		float cameraPriority = 0.f;

		auto view = registry.view<components::Camera>();
		for (auto &entity : view){
			auto &camera = view.get<components::Camera>(entity);

			// get the primary camera
			if (camera.priorityLevel > cameraPriority)
				primaryCamera = {entity, this};

		}
		return primaryCamera;
	}

	Entity Scene::duplicateEntity(Entity entity){
		if (!entity) return Entity();
		Entity clone = {registry.create(), this};

		copyComponents(clone, entity);
		return clone;
	}

	void Scene::OnSpriteComponentAdded(Entity entity){
		ENGINE_CORE_ASSERT(entity.hasComponent<components::Sprite>(), "entity dosen't have sprite component");
		auto &sprite = entity.getComponent<components::Sprite>();
		sprites.pushBack(entity.getComponent<components::UUID>().id);
	}

	void Scene::OnSpriteComponentRemoved(Entity entity){
		if (!entity.hasComponent<components::UUID>()) return;
		auto uuid = entity.getComponent<components::UUID>();
		sprites.erase(uuid);
		ENGINE_TRACE("erase");
	}

	void Scene::moveSpriteToTop(Entity entity){
		ENGINE_CORE_ASSERT(entity.hasComponent<components::Sprite>(), "entity dosen't have sprite component");
		sprites.pushBack(entity.getComponent<components::UUID>().id);
	}

	void Scene::moveSpriteToBack(Entity entity){
		ENGINE_CORE_ASSERT(entity.hasComponent<components::Sprite>(), "entity dosen't have sprite component");
		sprites.pushFront(entity.getComponent<components::UUID>().id);
	}

	void Scene::moveSpriteForward(Entity entity){
		ENGINE_CORE_ASSERT(entity.hasComponent<components::Sprite>(), "entity dosen't have sprite component");
		sprites.swapForward(entity.getComponent<components::UUID>().id);
	}

	void Scene::moveSpriteBackward(Entity entity){
		ENGINE_CORE_ASSERT(entity.hasComponent<components::Sprite>(), "entity dosen't have sprite component");
		sprites.swapBackward(entity.getComponent<components::UUID>().id);
	}
	
	void Scene::OnRuntimeStart(){
		physicsWorld = new b2World({gravity.x, gravity.y});
		
		auto view = registry.view<components::RigidBody>();
		for (auto &e : view){
			Entity entity = {e, this};
			auto &rigidBody = entity.getComponent<components::RigidBody>();
			auto &transform = entity.getComponent<components::Transform>();
			auto &uuid = entity.getComponent<components::UUID>().id;

			// init the body definition
			b2BodyDef bodyDef;
			bodyDef.fixedRotation = rigidBody.fixedRotation;
			bodyDef.angle = transform.rotation;
			bodyDef.bullet = rigidBody.bullet;
			bodyDef.angularVelocity = rigidBody.angularVelocity;
			bodyDef.gravityScale = rigidBody.gravityScale;
			bodyDef.linearVelocity = {rigidBody.linearVelocity.x, rigidBody.linearVelocity.y};
			bodyDef.position = {transform.translation.x, transform.translation.y};
			bodyDef.type = ToB2Type(rigidBody.type);

			auto body = physicsWorld->CreateBody(&bodyDef);
			rigidBody.runtimeBody = body;
			physicsBodies[uuid] = body;

			if (entity.hasComponent<components::BoxCollider>()){
				auto &boxCollider = entity.getComponent<components::BoxCollider>();

				b2PolygonShape shape;

				if (boxCollider.scaledSize){
					shape.SetAsBox(boxCollider.size.x * transform.scale.x, boxCollider.size.y * transform.scale.y, {boxCollider.offset.x * transform.scale.x, boxCollider.offset.y * transform.scale.y}, glm::radians(transform.rotation));
				} else {
					shape.SetAsBox(boxCollider.size.x, boxCollider.size.y, {boxCollider.offset.x, boxCollider.offset.y}, glm::radians(transform.rotation));
				}

				b2FixtureDef fixtureDef;
				fixtureDef.isSensor = boxCollider.isSensor;
				fixtureDef.density = boxCollider.density;
				fixtureDef.friction = boxCollider.friction;
				fixtureDef.restitution = boxCollider.restitution;
				fixtureDef.restitutionThreshold = boxCollider.restitutionThreshold;
				fixtureDef.shape = &shape;

				body->CreateFixture(&fixtureDef);
			}
		}

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
		return (*entityIDMap)[id];
	}

}
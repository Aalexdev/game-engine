#include "engine/pch.hpp"
#include "engine/scene/Scene.hpp"
#include "engine/scene/Entity.hpp"
#include "engine/scene/systems/RigidBodySystem.hpp"
#include "engine/scene/components/RigidBodyComponent.hpp"
#include "engine/scene/components/TransformComponent.hpp"
#include "engine/scene/components/BoxColliderComponent.hpp"

#include <libs/box2d/box2d.h>

namespace engine::ECS::systems{
	
	static b2BodyType toB2Type(ECS::components::RigidBody::BodyType type){
		switch (type){
			case ECS::components::RigidBody::BodyType::STATIC: return b2BodyType::b2_staticBody;
			case ECS::components::RigidBody::BodyType::DYNAMIC: return b2BodyType::b2_dynamicBody;
			case ECS::components::RigidBody::BodyType::KINEMATIC: return b2BodyType::b2_kinematicBody;
		}
		ENGINE_CORE_ASSERT(false, "unknown type");
		return b2BodyType::b2_staticBody;
	}

	void RigidBody::init(){
		auto &registry = scene->getRegistry();
		Signature signature{};
		signature.set(registry.getComponentType<components::RigidBody>());
		signature.set(registry.getComponentType<components::Transform>());
		registry.setSystemSignature<RigidBody>(signature);
	}

	void RigidBody::update(Timestep ts, uint32_t velocityIteration, uint32_t positionIteration){
		auto physicsWorld = scene->getPhysicsWorld();
		if (!physicsWorld) return;

		physicsWorld->Step(ts, velocityIteration, positionIteration);

		for (const auto &e : entities){
			engine::Entity entity = {e, scene};

			auto &transform = entity.getComponent<components::Transform>();
			b2Body *body = reinterpret_cast<b2Body*>(scene->getPhysicsBody(entity.getUUID()));

			const auto &pos = body->GetPosition();
			transform.transform.translation = {pos.x, pos.y};
			transform.transform.rotation = body->GetAngle();
		}
	}

	void RigidBody::start(){
		auto physicsWorld = scene->getPhysicsWorld();
		for (const auto &e : entities){
			engine::Entity entity = {e, scene};

			auto &rigidBody = entity.getComponent<components::RigidBody>();
			auto &transform = entity.getComponent<components::Transform>();

			b2BodyDef bodyDef;
			bodyDef.fixedRotation = rigidBody.fixedRotation;
			bodyDef.angle = transform.transform.rotation;
			bodyDef.bullet = rigidBody.bullet;
			bodyDef.angularVelocity = rigidBody.angularVelocity;
			bodyDef.gravityScale = rigidBody.gravityScale;
			bodyDef.linearVelocity = {rigidBody.linearVelocity.x, rigidBody.linearVelocity.y};
			bodyDef.position = {transform.transform.translation.x, transform.transform.translation.y};
			bodyDef.type = toB2Type(rigidBody.type);

			auto body = physicsWorld->CreateBody(&bodyDef);
			rigidBody.runtimeBody = body;
			scene->setPhysicBody(entity.getUUID(), body);

			if (entity.hasComponent<components::BoxCollider>()){
				auto &scale = transform.transform.scale;
				auto &boxCollider = entity.getComponent<components::BoxCollider>();

				b2PolygonShape shape;
				if (boxCollider.scaledSize){
					shape.SetAsBox(boxCollider.size.x * scale.x, boxCollider.size.y * scale.y, {boxCollider.offset.x * scale.x, boxCollider.offset.y * scale.y}, glm::radians(transform.transform.rotation));
				} else {
					shape.SetAsBox(boxCollider.size.x, boxCollider.size.y, {boxCollider.offset.x, boxCollider.offset.y}, glm::radians(transform.transform.rotation));
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
	}

	void RigidBody::renderCollisions(engine::Entity entity){
		auto renderer = scene->getRenderer();
		auto &transform = entity.getComponent<components::Transform>();

		renderer->setLineThickness(5);

		if (entity.hasComponent<components::BoxCollider>()){
			renderer->drawSquare(transform.transformMat, {0.8, 0.1, 0.2, 1.0}, static_cast<uint32_t>(entity));
		}
	}

	void RigidBody::renderCollisions(){
		for (const auto &e : entities){
			if (scene->getRegistry().getComponent<components::RigidBody>(e).renderCollisions) renderCollisions({e, scene});
		}
	}
}
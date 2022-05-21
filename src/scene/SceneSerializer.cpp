#include "engine/core.hpp"
#include "engine/scene/SceneSerializer.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/scene/Entity.hpp"
#include "engine/filesystem/Filesystem.hpp"

#include "engine/scene/components/TransformComponent.hpp"
#include "engine/scene/components/CameraComponent.hpp"
#include "engine/scene/components/SpriteComponent.hpp"
#include "engine/scene/components/RigidBodyComponent.hpp"
#include "engine/scene/components/BoxColliderComponent.hpp"
#include "engine/scene/components/CircleRendererComponent.hpp"
#include "engine/scene/components/TriangleRenderer.hpp"
#include "engine/scene/components/CircleColliderComponent.hpp"
#include "engine/scene/components/DistanceJointComponent.hpp"
#include "engine/scene/components/SpringJointComponent.hpp"
#include "engine/scene/components/RevoluteJoint.hpp"
#include "engine/scene/components/PrismaticJoint.hpp"

// libs
#include <libs/yaml-cpp/yaml.h>

template<>
struct std::hash<engine::Entity>{
    std::size_t operator()(engine::Entity const& e) const noexcept{
        return static_cast<uint32_t>(e);
    }
};

namespace YAML{
	Emitter& operator<<(YAML::Emitter& out, const glm::vec2 &v){
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	Emitter& operator<<(YAML::Emitter& out, const glm::vec3 &v){
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	Emitter& operator<<(YAML::Emitter& out, const glm::vec4 &v){
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	Emitter& operator<<(YAML::Emitter& out, const glm::u32vec2 &v){
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	template<>
	struct convert<glm::vec2>{
		static Node encode(const glm::vec2 &vec){
			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			return node;
		}

		static bool decode(const Node &node, glm::vec2 &vec){
			if (!node.IsSequence() || node.size() != 2) return false;

			vec.x = node[0].as<float>();
			vec.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>{
		static Node encode(const glm::vec3 &vec){
			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			node.push_back(vec.z);
			return node;
		}

		static bool decode(const Node &node, glm::vec3 &vec){
			if (!node.IsSequence() || node.size() != 3) return false;

			vec.x = node[0].as<float>();
			vec.y = node[1].as<float>();
			vec.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>{
		static Node encode(const glm::vec4 &vec){
			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			node.push_back(vec.z);
			node.push_back(vec.w);
			return node;
		}

		static bool decode(const Node &node, glm::vec4 &vec){
			if (!node.IsSequence() || node.size() != 4) return false;

			vec.x = node[0].as<float>();
			vec.y = node[1].as<float>();
			vec.z = node[2].as<float>();
			vec.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::u32vec2>{
		static Node encode(const glm::u32vec2 &vec){
			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			return node;
		}

		static bool decode(const Node &node, glm::u32vec2 &vec){
			if (!node.IsSequence() || node.size() != 2) return false;

			vec.x = node[0].as<uint32_t>();
			vec.y = node[1].as<uint32_t>();
			return true;
		}
	};

}

namespace engine{
	SceneSerializer::SceneSerializer(const Ref<Scene> &scene, const Ref<Texture2DLibrary> &texturesLibrary) : scene{scene}, texturesLibrary{texturesLibrary}{
		ENGINE_PROFILE_FUNCTION();
	}
	
	Ref<SceneSerializer> SceneSerializer::create(const Ref<Scene> &scene, const Ref<Texture2DLibrary> &texturesLibrary){
		return createRef<SceneSerializer>(scene, texturesLibrary);
	}

	static void serializeTransformComponent(YAML::Emitter &out, Entity entity){
		if (entity.hasComponent<ECS::components::Transform>()){
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			auto &transform = entity.getComponent<ECS::components::Transform>().transform;
			out << YAML::Key << "translation" << YAML::Value << transform.translation;
			out << YAML::Key << "scale" << YAML::Value << transform.scale;
			out << YAML::Key << "rotation" << YAML::Value << transform.rotation;
			
			out << YAML::EndMap;
		}
	}

	static void serializeCameraComponent(YAML::Emitter &out, Entity entity){
		if (entity.hasComponent<ECS::components::Camera>()){
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			auto &camera = entity.getComponent<ECS::components::Camera>();

			out << YAML::Key << "viewportAspectRatio" << YAML::Value << camera.camera.getAspectRatio();
			out << YAML::Key << "priorityLevel" << YAML::Value << camera.priorityLevel;
			out << YAML::Key << "fixedAspectRatio" << YAML::Value << camera.fixedViewportAspectRatio;

			if (camera.fixedViewportAspectRatio){
				out << YAML::Key << "viewportWidth" << YAML::Value << camera.viewportWidth;
				out << YAML::Key << "viewportHeight" << YAML::Value << camera.viewportHeight;
			}

			out << YAML::Key << "zoomLevel" << YAML::Value << camera.camera.getZoomLevel();
			out << YAML::EndMap;
		}
	}

	static void serializeSpriteComponent(YAML::Emitter &out, Entity entity){
		if (entity.hasComponent<ECS::components::Sprite>()){
			out << YAML::Key << "SpriteComponent";
			out << YAML::BeginMap;

			auto &sprite = entity.getComponent<ECS::components::Sprite>();
			out << YAML::Key << "color" << YAML::Value << sprite.color;

			
			if (sprite.texture && sprite.texture->getTexture()){
				auto &texture = sprite.texture;

				out << YAML::Key << "Texture";

				out << YAML::BeginMap;
				out << YAML::Key << "id"<< YAML::Value << texture->getTexture()->getUUID();
				out << YAML::Key << "Start" << YAML::Value << texture->getRelativeStart();
				out << YAML::Key << "End" << YAML::Value << texture->getRelativeEnd();
				out << YAML::EndMap;

			}
			
			out << YAML::EndMap;
		}
	}

	static void serializeRigidBodyComponent(YAML::Emitter &out, Entity entity){
		if (entity.hasComponent<ECS::components::RigidBody>()){
			out << YAML::Key << "RigidBodyComponent";
			out << YAML::BeginMap;

			auto &rigidBody = entity.getComponent<ECS::components::RigidBody>();

			out << YAML::Key << "Type" << YAML::Value << static_cast<int>(rigidBody.type);
			out << YAML::Key << "FixedRotation" << YAML::Value << rigidBody.fixedRotation;
			out << YAML::Key << "LinearVelocity" << YAML::Value << rigidBody.linearVelocity;
			out << YAML::Key << "AngularVelocity" << YAML::Value << rigidBody.angularVelocity;
			out << YAML::Key << "Bullet" << YAML::Value << rigidBody.bullet;
			out << YAML::Key << "GravityScale" << YAML::Value << rigidBody.gravityScale;

			out << YAML::EndMap;
		}
	}

	static void serializeBoxColliderComponent(YAML::Emitter &out, Entity entity){
		if (entity.hasComponent<ECS::components::BoxCollider>()){
			out << YAML::Key << "BoxColliderComponent";
			out << YAML::BeginMap;

			auto &collider = entity.getComponent<ECS::components::BoxCollider>();

			out << YAML::Key << "Offset" << YAML::Value << collider.offset;
			out << YAML::Key << "Size" << YAML::Value << collider.size;
			out << YAML::Key << "ScaledSize" << YAML::Value << collider.scaledSize;
			out << YAML::Key << "IsSensor" << YAML::Value << collider.isSensor;
			out << YAML::Key << "MaterialUD" << YAML::Value << collider.material;

			out << YAML::EndMap;
		}
	}

	static void serializeCircleRendererComponent(YAML::Emitter &out, Entity entity){
		if (entity.hasComponent<ECS::components::CircleRenderer>()){
			out << YAML::Key << "CircleRendererComponent";
			out << YAML::BeginMap;

			auto &circle = entity.getComponent<ECS::components::CircleRenderer>();

			out << YAML::Key << "Color" << YAML::Value << circle.color;
			out << YAML::Key << "Thickness" << YAML::Value << circle.thickness;
			out << YAML::Key << "Fade" << YAML::Value << circle.fade;

			out << YAML::EndMap;
		}
	}

	static void serializeTriangleRendererComponentVertex(YAML::Emitter &out, const Renderer::Vertex &v){
		out << YAML::Key << "LocalPosition" << YAML::Value << v.position;
		out << YAML::Key << "TextureCoords" << YAML::Value << v.textCoord;
		out << YAML::Key << "Color" << YAML::Value << v.color;
		out << YAML::Key << "TilingFactor" << YAML::Value << v.tilingFactor;
	}

	static void serializeTriangleRendererComponent(YAML::Emitter &out, Entity entity){
		if (entity.hasComponent<ECS::components::TriangleRenderer>()){
			out << YAML::Key << "TriangleRendererComponent";
			out << YAML::BeginMap;

			auto &triangle = entity.getComponent<ECS::components::TriangleRenderer>();

			out << YAML::Key << "p1";
			out << YAML::BeginMap;
			serializeTriangleRendererComponentVertex(out, triangle.p1);
			out << YAML::EndMap;
			
			out << YAML::Key << "p2";
			out << YAML::BeginMap;
			serializeTriangleRendererComponentVertex(out, triangle.p2);
			out << YAML::EndMap;

			out << YAML::Key << "p3";
			out << YAML::BeginMap;
			serializeTriangleRendererComponentVertex(out, triangle.p3);
			out << YAML::EndMap;

			if (triangle.texture)
				out << YAML::Key << "TextureID" << YAML::Value << triangle.texture->getUUID();

			out << YAML::EndMap;
		}
	}

	static void serializeCircleColliderComponent(YAML::Emitter &out, Entity entity){
		if (entity.hasComponent<ECS::components::CircleCollider>()){
			out << YAML::Key << "CircleColliderComponent";
			out << YAML::BeginMap;

			auto &collider = entity.getComponent<ECS::components::CircleCollider>();

			out << YAML::Key << "Position" << YAML::Value << collider.position;
			out << YAML::Key << "Collider" << YAML::Value << collider.radius;
			out << YAML::Key << "ScaledSize" << YAML::Value << collider.scaledSize;
			out << YAML::Key << "IsSensor" << YAML::Value << collider.isSensor;
			out << YAML::Key << "MaterialUD" << YAML::Value << collider.material;

			out << YAML::EndMap;
		}
	}

	static void serializeDistanceJointComponent(YAML::Emitter &out, Entity entity){
		if (entity.hasComponent<ECS::components::DistanceJoint>()){
			auto &component = entity.getComponent<ECS::components::DistanceJoint>();

			out << YAML::Key << "DistanceJointComponent";
			out << YAML::BeginSeq;
			for (auto &joint : component.joints){
				out << YAML::BeginMap;
				out << YAML::Key << "Joint" << YAML::Value << joint.entityB;
				out << YAML::Key << "AnchorAOffset" << YAML::Value << joint.anchorOffsetA;
				out << YAML::Key << "AnchorBOffset" << YAML::Value << joint.anchorOffsetB;
				out << YAML::EndMap;
			}

			out << YAML::EndSeq;
		}
	}

	static void serializeSpringJointComponent(YAML::Emitter &out, Entity entity){
		if (entity.hasComponent<ECS::components::SpringJoint>()){
			auto &component = entity.getComponent<ECS::components::SpringJoint>();

			out << YAML::Key << "SpringJointComponent";
			out << YAML::BeginSeq;

			for (auto &joint : component.joints){
				out << YAML::BeginMap;
				out << YAML::Key << "Joint" << YAML::Value << joint.joinedEntity;
				out << YAML::Key << "AnchorA" << YAML::Value << joint.anchorA;
				out << YAML::Key << "AnchorB" << YAML::Value << joint.anchorB;
				out << YAML::Key << "DampingRatio" << YAML::Value <<joint.dampingRatio;
				out << YAML::Key << "Frequency" << YAML::Value <<joint.frequencyHertz;
				out << YAML::EndMap;
			}

			out << YAML::EndSeq;
		}
	}

	static void serializeRevoluteJointComponent(YAML::Emitter &out, Entity entity){
		if (entity.hasComponent<ECS::components::RevoluteJoint>()){
			auto &component = entity.getComponent<ECS::components::RevoluteJoint>();

			out << YAML::Key << "RevoluteJointComponent";
			out << YAML::BeginSeq;

			for (auto &joint : component.joints){
				out << YAML::BeginMap;
				out << YAML::Key << "Joint" << YAML::Value << joint.joinedEntity;
				out << YAML::Key << "Anchor" << YAML::Value << joint.anchor;
				out << YAML::Key << "Limits" << YAML::Value << joint.limits;
				out << YAML::Key << "MinAngle" << YAML::Value << joint.minAngle;
				out << YAML::Key << "MaxAngle" << YAML::Value << joint.maxAngle;
				out << YAML::Key << "Motor" << YAML::Value << joint.motor;
				out << YAML::Key << "MaxTorque" << YAML::Value << joint.maxTorque;
				out << YAML::Key << "Speed" << YAML::Value << joint.speed;
				out << YAML::EndMap;
			}

			out << YAML::EndSeq;
		}
	}

	static void serializePrismaticJointComponent(YAML::Emitter &out, Entity entity){
		if (entity.hasComponent<ECS::components::PrismaticJoint>()){
			auto &component = entity.getComponent<ECS::components::PrismaticJoint>();
			
			out << YAML::Key << "PrismaticJointComponent";
			out << YAML::BeginSeq;

			for (auto &joint : component.joints){
				out << YAML::BeginMap;
				out << YAML::Key << "Joint" << YAML::Value << joint.joinedEntity;
				out << YAML::Key << "Translation" << YAML::Value << joint.translation;
				out << YAML::Key << "Limits" << YAML::Value << joint.limits;
				out << YAML::Key << "MinTranslation" << YAML::Value << joint.minTranslation;
				out << YAML::Key << "MaxTranslation" << YAML::Value << joint.maxTranslation;
				out << YAML::Key << "Motor" << YAML::Value << joint.motor;
				out << YAML::Key << "MaxForce" << YAML::Value << joint.maxForce;
				out << YAML::Key << "Speed" << YAML::Value << joint.speed;
				out << YAML::EndMap;
			}

			out << YAML::EndSeq;
		}
	}

	static void serializeEntity(YAML::Emitter& out, Entity entity){

		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << static_cast<uint64_t>(entity.getUUID());
		out << YAML::Key << "Tag" << YAML::Value << entity.getTag();

		serializeTransformComponent(out, entity);
		serializeCameraComponent(out, entity);
		serializeSpriteComponent(out, entity);
		serializeRigidBodyComponent(out, entity);
		serializeBoxColliderComponent(out, entity);
		serializeCircleRendererComponent(out, entity);
		serializeTriangleRendererComponent(out, entity);
		serializeCircleColliderComponent(out, entity);
		serializeDistanceJointComponent(out, entity);
		serializeSpringJointComponent(out, entity);
		serializeRevoluteJointComponent(out, entity);

		out << YAML::EndMap;
	}

	static void serailizeSpriteQueue(YAML::Emitter& out, Set<UUID> &queue){
		out << YAML::Key << "SpriteQueue" << YAML::Value;
		out << YAML::BeginSeq;

		Set<UUID>::iterator it = queue.begin();
		while (it){
			out << YAML::BeginMap;
			out << YAML::Key << "Entity" << YAML::Value << *it;
			out << YAML::EndMap;
			it++;
		}

		out << YAML::EndSeq;
	}

	static void serializeTextures(YAML::Emitter& out, Texture2DLibrary &textures){
		out << YAML::Key << "Textures" << YAML::Value;
		out << YAML::BeginSeq;

		for (auto &t : textures.getList()){
			out << YAML::BeginMap;
			out << YAML::Key << "Texture" << YAML::Value << t->getUUID().get();
			out << YAML::Key << "Name" << YAML::Value << t->getName();
			out << YAML::Key << "Path" << YAML::Value << t->getPath().string();
			out << YAML::EndMap;
		}

		out << YAML::EndSeq;
	}

	static void serializePhysicMaterials(YAML::Emitter &out, Ref<PhysicMaterialLibrary> &library){
		out << YAML::Key << "PhysicMaterials" << YAML::Value;
		out << YAML::BeginSeq;

		for (auto &pair : library->get()){
			auto &material = pair.second;
			out << YAML::BeginMap;
			out << YAML::Key << "PhysicalMaterial" << YAML::Value << pair.first;
			out << YAML::Key << "Density" << YAML::Value << material->density;
			out << YAML::Key << "Friction" << YAML::Value << material->friction;
			out << YAML::Key << "Resitution" << YAML::Value << material->restitution;
			out << YAML::Key << "ResitutionThreshold" << YAML::Value << material->restitutionThreshold;
			out << YAML::EndMap;
		}

		out << YAML::EndSeq;
	}

	void SceneSerializer::serializeText(const std::string &filepath){
		ENGINE_PROFILE_FUNCTION();

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Name";
		
		serializeTextures(out, *texturesLibrary.get());

		out << YAML::Key << "Entities" << YAML::Value;
		out << YAML::BeginSeq;

		for (auto &e : scene->getRegistry()){
			Entity entity = {e, scene.get()};
			serializeEntity(out, entity);
		}


		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream file(filepath);
		ENGINE_CORE_ASSERT(file.is_open(), "failed to create or open \"", filepath, "\"");
		file << out.c_str();
		file.close();
	}

	void SceneSerializer::serializeBin(const std::string &filepath){
		ENGINE_PROFILE_FUNCTION();
	}

	static void deseralizeTextures(YAML::Node data, Texture2DLibrary &texturesLibrary){
		for (YAML::Node node : data){
			YAML::Node texture = node["Texture"];
			if (!texture) continue;

			std::string name = node["Name"].as<std::string>();
			std::filesystem::path path = Filesystem::getDataFolderPath() / node["Path"].as<std::string>();
			UUID uuid = texture.as<uint64_t>();
			
			texturesLibrary.load(path, name, uuid);
		}
	}

	static void derializePhysicMaterial(YAML::Node data, Ref<PhysicMaterialLibrary> library){
		for (YAML::Node node : data){
			YAML::Node materialNode = node["PhysicMaterial"];
			if (!materialNode) continue;

			PhysicMaterial material;
			material.name = materialNode.as<std::string>();
			material.density = node["Density"].as<float>();
			material.friction = node["Friction"].as<float>();
			material.restitution = node["Restitution"].as<float>();
			material.restitutionThreshold = node["RestitutionThreshold"].as<float>();

			library->push(material);
		}
	}

	static void deserializeTransformComponent(YAML::Node data, Entity entity){
		YAML::Node node = data["TransformComponent"];
		if (node){
			auto &transform = entity.addComponent<ECS::components::Transform>().transform;
			transform.translation = node["translation"].as<glm::vec2>();
			transform.scale = node["scale"].as<glm::vec2>();
			transform.rotation = node["rotation"].as<float>();
		}
	}

	static void deserializeCameraComponent(YAML::Node data, Entity entity){
		YAML::Node node = data["CameraComponent"];
		if (node){
			auto &camera = entity.addComponent<ECS::components::Camera>();
			camera.fixedViewportAspectRatio = node["fixedAspectRatio"].as<bool>();
			camera.priorityLevel = node["priorityLevel"].as<float>();

			if (camera.fixedViewportAspectRatio){
				camera.viewportWidth = node["viewportWidth"].as<uint32_t>();
				camera.viewportHeight = node["viewportHeight"].as<uint32_t>();
			} else {
				camera.viewportWidth = entity.getScene()->getViewportWidth();
				camera.viewportHeight = entity.getScene()->getViewportHeight();
			}

			camera.camera.setZoomLevel(node["zoomLevel"].as<float>());
			camera.camera.setAspectRatio(node["viewportAspectRatio"].as<float>());
		}
	}

	static void deserializeSpriteComponent(YAML::Node data, Entity entity, const Ref<Texture2DLibrary>& textures){
		YAML::Node node = data["SpriteComponent"];
		if (node){
			auto &sprite = entity.addComponent<ECS::components::Sprite>();
			sprite.color = node["color"].as<glm::vec4>();

			YAML::Node texture = node["Texture"];
			if (texture){
				UUID id = texture["id"].as<uint64_t>();

				glm::u32vec2 start = texture["Start"].as<glm::u32vec2>();
				glm::u32vec2 size = texture["End"].as<glm::u32vec2>() - start;
				sprite.texture = SubTexture2D::create(textures->get(id), start.x, start.y, size.x, size.y);

			} else {
				sprite.texture = nullptr;
			}
		}
	}

	static void deserializeRigidBodyComponent(YAML::Node data, Entity entity){
		YAML::Node node = data["RigidBodyComponent"];
		if (node){
			auto &rigidBody = entity.addComponent<ECS::components::RigidBody>();

			rigidBody.type = static_cast<ECS::components::RigidBody::BodyType>(node["Type"].as<int>());
			rigidBody.fixedRotation = node["FixedRotation"].as<bool>();
			rigidBody.linearVelocity = node["LinearVelocity"].as<glm::vec2>();
			rigidBody.angularVelocity = node["AngularVelocity"].as<float>();
			rigidBody.bullet = node["Bullet"].as<bool>();
			rigidBody.gravityScale = node["GravityScale"].as<float>();
		}
	}

	static void deserializeBoxColliderComponent(YAML::Node data, Entity entity){
		YAML::Node node = data["BoxColliderComponent"];
		if (node){
			auto &collider = entity.addComponent<ECS::components::BoxCollider>();

			collider.offset = node["Offset"].as<glm::vec2>();
			collider.size = node["Size"].as<glm::vec2>();
			collider.scaledSize = node["ScaledSize"].as<bool>();
			collider.isSensor = node["IsSensor"].as<bool>();
			collider.material = node["Material"].as<std::string>();
		}
	}
	
	static void deserializeCircleRendererComponent(YAML::Node data, Entity entity){
		YAML::Node node = data["CircleRendererComponent"];
		if (node){
			auto &circle = entity.addComponent<ECS::components::CircleRenderer>();

			circle.color = node["Color"].as<glm::vec4>();
			circle.thickness = node["Thickness"].as<float>();
			circle.fade = node["Fade"].as<float>();
		}
	}
	
	static void deserializeTriangleRendererComponentVertex(YAML::Node node, Renderer::Vertex &v){
		v.color = node["Color"].as<glm::vec4>();
		v.position = node["LocalPosition"].as<glm::vec2>();
		v.textCoord = node["TextureCoords"].as<glm::vec2>();
		v.tilingFactor = node["TilingFactor"].as<float>();
	}

	static void deserializeTriangleRendererComponent(YAML::Node data, Entity entity, const Ref<Texture2DLibrary>& textures){
		YAML::Node node = data["TriangleRendererComponent"];
		if (node){
			auto &triangle = entity.addComponent<ECS::components::TriangleRenderer>();
			deserializeTriangleRendererComponentVertex(node["p1"], triangle.p1);
			deserializeTriangleRendererComponentVertex(node["p2"], triangle.p2);
			deserializeTriangleRendererComponentVertex(node["p3"], triangle.p3);

			YAML::Node textureNode = node["TextureID"];
			if (textureNode && textures){
				triangle.texture = textures->get(UUID(textureNode.as<uint64_t>()));
			}
		}
	}

	static void deserializeCircleColliderComponent(YAML::Node data, Entity entity){
		YAML::Node node = data["CircleColliderComponent"];

		if (node){
			auto &collider = entity.addComponent<ECS::components::CircleCollider>();
			
			collider.position = node["Position"].as<glm::vec2>();
			collider.radius = node["Radius"].as<float>();
			collider.scaledSize = node["ScaledSize"].as<bool>();
			collider.isSensor = node["IsSensor"].as<bool>();
			collider.material = node["Material"].as<std::string>();
		}
	}

	static void deserializeJoint(YAML::Node node, Joint &joint){
		joint.entityB = node["Entity"].as<uint64_t>();
		joint.anchorOffsetA = node["AnchorAOffset"].as<glm::vec2>();
		joint.anchorOffsetB = node["AnchorBOffset"].as<glm::vec2>();
	}

	static void deserializeDistanceJointComponent(YAML::Node data, Entity entity){
		YAML::Node node = data["DistanceJointComponent"];
		if (node){
			auto &component = entity.addComponent<ECS::components::DistanceJoint>();

			for (auto jointNode : node){
				Joint joint;
				deserializeJoint(jointNode, joint);
				component.joints.push_back(joint);
			}
		}
	}

	static void deserializeSpringJointComponent(YAML::Node data, Entity entity){
		YAML::Node node = data["SpringJointComponent"];
		if (node){
			auto &component = entity.addComponent<ECS::components::SpringJoint>();

			for (auto jointNode : node){
				ECS::components::SpringJoint::Joint joint;
				joint.joinedEntity = jointNode["Joint"].as<uint64_t>();
				joint.anchorA = jointNode["AnchorA"].as<glm::vec2>();
				joint.anchorB = jointNode["AnchorB"].as<glm::vec2>();
				joint.dampingRatio = jointNode["DampingRatio"].as<float>();
				joint.frequencyHertz = jointNode["Frequency"].as<float>();

				component.joints.push_back(joint);
			}
		}
	}

	static void deserializeRevoluteJointComponent(YAML::Node data, Entity entity){
		YAML::Node node = data["RevoluteJointComponent"];
		if (node){
			auto &component = entity.addComponent<ECS::components::RevoluteJoint>();

			for (auto jointNode : node){
				ECS::components::RevoluteJoint::Joint joint;
				joint.joinedEntity = jointNode["Joint"].as<uint64_t>();
				joint.limits = jointNode["Limits"].as<bool>();
				joint.minAngle = jointNode["MinAngle"].as<float>();
				joint.maxAngle = jointNode["MaxAngle"].as<float>();
				joint.motor = jointNode["Motor"].as<bool>();
				joint.maxTorque = jointNode["MaxTorque"].as<float>();
				joint.speed = jointNode["Speed"].as<float>();

				component.joints.push_back(joint);
			}
		}
	}

	static void deserializePrismaticJointComponent(YAML::Node data, Entity entity){
		YAML::Node node = data["PrismaticJointComponent"];
		if (node){
			auto &component = entity.addComponent<ECS::components::PrismaticJoint>();

			for (auto jointNode : node){
				ECS::components::PrismaticJoint::Joint joint;
				joint.joinedEntity = node["Joint"].as<uint64_t>();
				joint.translation = node["Translation"].as<glm::vec2>();
				joint.limits = node["Limits"].as<bool>();
				joint.minTranslation = node["MinTranslation"].as<float>();
				joint.maxTranslation = node["MaxTranslation"].as<float>();
				joint.motor = node["Motor"].as<bool>();
				joint.maxForce = node["MaxForce"].as<float>();
				joint.speed = node["Speed"].as<float>();
			}
		}
	}
	
	bool SceneSerializer::deserializeText(const std::string &filepath){
		ENGINE_PROFILE_FUNCTION();

		std::ifstream file(filepath);

		if (!file.is_open()) return false;

		std::stringstream strStream;
		strStream << file.rdbuf();
		file.close();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"]) return false;
		
		std::string sceneName = data["Scene"].as<std::string>();
		ENGINE_CORE_TRACE("deserialize scene ", sceneName);

		auto textures = data["Textures"];
		if (textures){
			deseralizeTextures(textures, *texturesLibrary.get());
		}

		auto physicMaterials = data["PhysicMaterials"];
		if (physicMaterials){
			derializePhysicMaterial(data, scene->physicMaterials);
		}

		auto entities = data["Entities"];
		if (entities){
			for (auto entity : entities){
				uint64_t uuid = entity["Entity"].as<uint64_t>();
				std::string name = "";
				
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["tag"].as<std::string>();

				Entity ent = scene->createEntity(uuid, name);
				
				deserializeTransformComponent(entity, ent);
				deserializeCameraComponent(entity, ent);
				deserializeSpriteComponent(entity, ent, texturesLibrary);
				deserializeRigidBodyComponent(entity, ent);
				deserializeBoxColliderComponent(entity, ent);
				deserializeCircleRendererComponent(entity, ent);
				deserializeTriangleRendererComponent(entity, ent, texturesLibrary);
				deserializeCircleColliderComponent(entity, ent);
				deserializeDistanceJointComponent(entity, ent);
				deserializeSpringJointComponent(entity, ent);
				deserializeRevoluteJointComponent(entity, ent);
				deserializePrismaticJointComponent(entity, ent);
			}
		}

		auto spriteQueue = data["SpriteQueue"];

		return true;
	}

	bool SceneSerializer::deserializeBin(const std::string &filepath){
		ENGINE_PROFILE_FUNCTION();
		return false;
	}
}
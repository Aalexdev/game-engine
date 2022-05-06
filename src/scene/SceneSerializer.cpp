#include "engine/core.hpp"
#include "engine/scene/SceneSerializer.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/scene/Entity.hpp"
#include "engine/filesystem/Filesystem.hpp"

#include "engine/scene/components/TagComponent.hpp"
#include "engine/scene/components/TransformComponent.hpp"
#include "engine/scene/components/CameraComponent.hpp"
#include "engine/scene/components/SpriteComponent.hpp"
#include "engine/scene/components/UUIDComponent.hpp"
#include "engine/scene/components/RigidBodyComponent.hpp"
#include "engine/scene/components/BoxColliderComponent.hpp"
#include "engine/scene/components/CircleRendererComponent.hpp"
#include "engine/scene/components/TriangleRenderer.hpp"

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

	static void serializeTagComponent(YAML::Emitter &out, Entity entity){
		if (entity.hasComponent<components::Tag>()){
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;

			auto &tag = entity.getComponent<components::Tag>().tag;
			out << YAML::Key << "tag" << YAML::Value << tag;
			out << YAML::EndMap;
		}
	}

	static void serializeTransformComponent(YAML::Emitter &out, Entity entity){
		if (entity.hasComponent<components::Transform>()){
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			auto &transform = entity.getComponent<components::Transform>();
			out << YAML::Key << "translation" << YAML::Value << transform.translation;
			out << YAML::Key << "scale" << YAML::Value << transform.scale;
			out << YAML::Key << "rotation" << YAML::Value << transform.rotation;
			
			out << YAML::EndMap;
		}
	}

	static void serializeCameraComponent(YAML::Emitter &out, Entity entity){
		if (entity.hasComponent<components::Camera>()){
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			auto &camera = entity.getComponent<components::Camera>();

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
		if (entity.hasComponent<components::Sprite>()){
			out << YAML::Key << "SpriteComponent";
			out << YAML::BeginMap;

			auto &sprite = entity.getComponent<components::Sprite>();
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
		if (entity.hasComponent<components::RigidBody>()){
			out << YAML::Key << "RigidBodyComponent";
			out << YAML::BeginMap;

			auto &rigidBody = entity.getComponent<components::RigidBody>();

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
		if (entity.hasComponent<components::BoxCollider>()){
			out << YAML::Key << "BoxColliderComponent";
			out << YAML::BeginMap;

			auto &collider = entity.getComponent<components::BoxCollider>();

			out << YAML::Key << "Offset" << YAML::Value << collider.offset;
			out << YAML::Key << "Size" << YAML::Value << collider.size;
			out << YAML::Key << "ScaledSize" << YAML::Value << collider.scaledSize;
			out << YAML::Key << "IsSensor" << YAML::Value << collider.isSensor;
			out << YAML::Key << "Density" << YAML::Value << collider.density;
			out << YAML::Key << "Friction" << YAML::Value << collider.friction;
			out << YAML::Key << "Restitution" << YAML::Value << collider.restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << collider.restitutionThreshold;

			out << YAML::EndMap;
		}
	}

	static void serializeCircleRendererComponent(YAML::Emitter &out, Entity entity){
		if (entity.hasComponent<components::CircleRenderer>()){
			out << YAML::Key << "CircleRendererComponent";
			out << YAML::BeginMap;

			auto &circle = entity.getComponent<components::CircleRenderer>();

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
		if (entity.hasComponent<components::TriangleRenderer>()){
			out << YAML::Key << "TriangleRendererComponent";
			out << YAML::BeginMap;

			auto &triangle = entity.getComponent<components::TriangleRenderer>();

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

	static void serializeEntity(YAML::Emitter& out, Entity entity){
		if (!entity.hasComponent<components::UUID>()) return;

		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << static_cast<uint64_t>(entity.getComponent<components::UUID>());

		serializeTagComponent(out, entity);
		serializeTransformComponent(out, entity);
		serializeCameraComponent(out, entity);
		serializeSpriteComponent(out, entity);
		serializeRigidBodyComponent(out, entity);
		serializeBoxColliderComponent(out, entity);
		serializeCircleRendererComponent(out, entity);
		serializeTriangleRendererComponent(out, entity);

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

	void SceneSerializer::serializeText(const std::string &filepath){
		ENGINE_PROFILE_FUNCTION();

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Name";
		
		serializeTextures(out, *texturesLibrary.get());

		out << YAML::Key << "Entities" << YAML::Value;
		out << YAML::BeginSeq;

		scene->registry.each([&](auto entityID){
			Entity entity = {entityID, scene.get()};
			if (!entity) return;

			serializeEntity(out, entity);
		});

		out << YAML::EndSeq;

		serailizeSpriteQueue(out, scene->sprites);

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

	void SceneSerializer::deserializeSpriteQueue(YAML::Node data){
		for (YAML::Node node : data){
			YAML::Node entity = node["Entity"];

			if (entity){
				UUID id = entity.as<uint64_t>();
				scene->sprites.pushBack(id);
			}
		}
	}

	static void deserializeTransformComponent(YAML::Node data, Entity entity){
		YAML::Node node = data["TransformComponent"];
		if (node){
			auto &transform = entity.addComponent<components::Transform>();
			transform.translation = node["translation"].as<glm::vec2>();
			transform.scale = node["scale"].as<glm::vec2>();
			transform.rotation = node["rotation"].as<float>();
		}
	}

	static void deserializeCameraComponent(YAML::Node data, Entity entity){
		YAML::Node node = data["CameraComponent"];
		if (node){
			auto &camera = entity.addComponent<components::Camera>();
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
			auto &sprite = entity.addComponent<components::Sprite>();
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
			auto &rigidBody = entity.addComponent<components::RigidBody>();

			rigidBody.type = static_cast<components::RigidBody::BodyType>(node["Type"].as<int>());
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
			auto &collider = entity.addComponent<components::BoxCollider>();

			collider.offset = node["Offset"].as<glm::vec2>();
			collider.size = node["Size"].as<glm::vec2>();
			collider.scaledSize = node["ScaledSize"].as<bool>();
			collider.isSensor = node["IsSensor"].as<bool>();
			collider.density = node["Density"].as<float>();
			collider.friction = node["Friction"].as<float>();
			collider.restitution = node["Restitution"].as<float>();
			collider.restitutionThreshold = node["RestitutionThreshold"].as<float>();
		}
	}
	
	static void deserializeCircleRendererComponent(YAML::Node data, Entity entity){
		YAML::Node node = data["CircleRendererComponent"];
		if (node){
			auto &circle = entity.addComponent<components::CircleRenderer>();

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
			auto &triangle = entity.addComponent<components::TriangleRenderer>();
			deserializeTriangleRendererComponentVertex(node["p1"], triangle.p1);
			deserializeTriangleRendererComponentVertex(node["p2"], triangle.p2);
			deserializeTriangleRendererComponentVertex(node["p3"], triangle.p3);

			YAML::Node textureNode = node["TextureID"];
			if (textureNode && textures){
				triangle.texture = textures->get(UUID(textureNode.as<uint64_t>()));
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

		auto entities = data["Entities"];
		if (entities){
			for (auto entity : entities){
				uint64_t uuid = entity["Entity"].as<uint64_t>();
				std::string name = "";
				
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["tag"].as<std::string>();

				Entity ent = scene->createIDEntity(uuid, name);
				
				deserializeTransformComponent(entity, ent);
				deserializeCameraComponent(entity, ent);
				deserializeSpriteComponent(entity, ent, texturesLibrary);
				deserializeRigidBodyComponent(entity, ent);
				deserializeBoxColliderComponent(entity, ent);
				deserializeCircleRendererComponent(entity, ent);
				deserializeTriangleRendererComponent(entity, ent, texturesLibrary);
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
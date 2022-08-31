#include "include/RainDrop.hpp"
#include "include/glm/glm.hpp"
#include <chrono>
#include "include/glm/gtx/matrix_transform_2d.hpp"
#include <filesystem>
#include "include/glm/gtc/matrix_transform.hpp"


struct SoundComponent{
	RainDrop::SoundSource source;
};

struct Transform{
	glm::mat3 transform;
};

struct TextureComponent{
	glm::vec2 uv1, uv2;
	RainDrop::Texture texture;
};

struct EnemyComponent{
	uint32_t health;
	float cooldown;
	glm::vec2 size;
};

struct PlayerComponent{
	uint32_t health;
	uint32_t maxHealth;
	glm::vec2 size;
	float speed;
};

struct MissileComponent{
	int32_t damages;
	float speed;
	uint8_t team;
};

class MissileSystem : public RainDrop::ECSSystem{
	friend class PlayerSystem;
	friend class EnemySystem;

	public:
		void update(float dt);
		void render();
		void init();
		void setSignature();
};

class EnemySystem : public RainDrop::ECSSystem{
	public:
		void update(float dt);
		void render();
		void init(MissileSystem* missiles);
		void setSignature();
	
	private:
		MissileSystem* missiles;
};

class PlayerSystem : public RainDrop::ECSSystem{
	public:
		void update(float dt);
		void render();
		void init(MissileSystem* missiles);
		void setSignature();
		
	private:
		MissileSystem* missiles;
};

#define ENEMY_TEAM 0
#define PLAYER_TEAM 1

#define ENEMY_SPEED 3.f
#define ENEMY_COOLDOWN 3.5f

struct DefaultShaderVertex{
	glm::vec2 pos;
	glm::vec3 color;
	glm::vec2 uv;
	uint32_t textureID;
};

void EnemySystem::init(MissileSystem* missiles){
	this->missiles = missiles;
	setSignature();
}

void EnemySystem::update(float dt){
	static RainDrop::EventID missileLaunched = RainDrop::getEventID("launch missile");
	static RainDrop::EventID missileContact = RainDrop::getEventID("missile contact");
	static RainDrop::EventID deleteEnemy = RainDrop::getEventID("delete enemy");

	for (auto &id : entities){
		RainDrop::Entity entity = id;
		auto& transform = entity.getComponent<Transform>().transform;
		auto& enemy = entity.getComponent<EnemyComponent>();

		transform = glm::translate(transform, {0.f, dt * ENEMY_SPEED});
		glm::vec2 pos = glm::vec2(transform[2].x, transform[2].y);

		enemy.cooldown -= dt;
		if (enemy.cooldown <= 0.f){
			enemy.cooldown = ENEMY_COOLDOWN;
			RainDrop::triggerEvent(missileLaunched, ENEMY_TEAM, pos);
		}

		glm::vec4 box;
		box.x = pos.x - enemy.size.x / 2.f;
		box.y = pos.y - enemy.size.y / 2.f;
		box.z = pos.x + enemy.size.x / 2.f;
		box.w = pos.y + enemy.size.y / 2.f;

		printf("%f\n", box.y);
		if (box.y > 2000){
			RainDrop::triggerEvent(deleteEnemy, id);
			break;
		}


		// check collisions
		for (auto &missileID : missiles->entities){
			RainDrop::Entity missile = missileID;

			auto &missileTransform = missile.getComponent<Transform>().transform;
			glm::vec2 missilePos = glm::vec2(missileTransform[0].z, missileTransform[1].z);

			// check bound box
			if (missilePos.x >= box.x && missilePos.x <= box.z && missilePos.y >= box.y && missilePos.y <= box.w){
				RainDrop::triggerEvent(missileContact, missileID, id);
			}
		}
	}
}

void EnemySystem::render(){
	for (auto &id : entities){
		RainDrop::Entity entity = id;
		auto& transform = entity.getComponent<Transform>().transform;
		auto& texture = entity.getComponent<TextureComponent>();

		DefaultShaderVertex v[4];

		v[0].color = {1.f, 1.f, 1.f};
		v[0].pos = transform * glm::vec3{-0.5, -0.5, 1.f};
		v[0].textureID = 0;
		v[0].uv = texture.uv1;

		v[1].color = {1.f, 1.f, 1.f};
		v[1].pos = transform * glm::vec3{-0.5, 0.5, 1.f};
		v[1].textureID = 0;
		v[1].uv = {texture.uv1.x, texture.uv2.y};

		v[2].color = {1.f, 1.f, 1.f};
		v[2].pos = transform * glm::vec3{0.5, 0.5, 1.f};
		v[2].textureID = 0;
		v[2].uv = texture.uv2;

		v[3].color = {1.f, 0.f, 0.f};
		v[3].pos = transform * glm::vec3{0.5, -0.5, 1.f};
		v[3].textureID = 0;
		v[3].uv = {texture.uv2.x, texture.uv1.y};

		RainDrop::renderSceneQuad(&v[0], &v[1], &v[2], &v[3]);
	}
}

void EnemySystem::setSignature(){
	RainDrop::ECSSignature signature;
	signature.set(RainDrop::getComponentID<Transform>());
	signature.set(RainDrop::getComponentID<EnemyComponent>());
	signature.set(RainDrop::getComponentID<RainDrop::Sound>());
	signature.set(RainDrop::getComponentID<TextureComponent>());
	RainDrop::setECSSystemSignature<EnemySystem>(signature);
}

void PlayerSystem::init(MissileSystem* missiles){
	this->missiles = missiles;
	setSignature();
}

void PlayerSystem::update(float dt){
	static RainDrop::EventID missileLaunched = RainDrop::getEventID("launch missile");
	static RainDrop::EventID missileContact = RainDrop::getEventID("missile contact");

	
	for (auto &id : entities){
		RainDrop::Entity entity = id;
		RainDrop::vec2<float> mousePos = RainDrop::getMousePosition();

		auto& transform = entity.getComponent<Transform>().transform;
		auto& player = entity.getComponent<PlayerComponent>();

		// printf("%f %f\n", mousePos.x, mousePos.y);

		transform = glm::scale(glm::translate(glm::mat3(1.f), {mousePos.x, mousePos.y}), player.size);
		glm::vec2 pos = glm::vec2(transform[2].x, transform[2].y);

		glm::vec4 box;
		box.x = pos.x - player.size.x / 2.f;
		box.y = pos.y - player.size.y / 2.f;
		box.z = pos.x + player.size.x / 2.f;
		box.w = pos.y + player.size.y / 2.f;


		// check collisions
		for (auto &missileID : missiles->entities){
			RainDrop::Entity missile = missileID;

			auto &missileTransform = missile.getComponent<Transform>().transform;
			glm::vec2 missilePos = glm::vec2(missileTransform[0].z, missileTransform[1].z);

			// check bound box
			if (missilePos.x >= box.x && missilePos.x <= box.z && missilePos.y >= box.y && missilePos.y <= box.w){
				RainDrop::triggerEvent(missileContact, missileID, id);
			}
		}
	}
}

void PlayerSystem::render(){
	for (auto &id : entities){
		RainDrop::Entity entity = id;
		auto& transform = entity.getComponent<Transform>().transform;
		auto& texture = entity.getComponent<TextureComponent>();

		DefaultShaderVertex v[4];

		v[0].color = {1.f, 1.f, 1.f};
		v[0].pos = transform * glm::vec3{-0.5, -0.5, 1.f};
		v[0].textureID = 0;
		v[0].uv = texture.uv1;

		v[1].color = {1.f, 1.f, 1.f};
		v[1].pos = transform * glm::vec3{-0.5, 0.5, 1.f};
		v[1].textureID = 0;
		v[1].uv = {texture.uv1.x, texture.uv2.y};

		v[2].color = {1.f, 1.f, 1.f};
		v[2].pos = transform * glm::vec3{0.5, 0.5, 1.f};
		v[2].textureID = 0;
		v[2].uv = texture.uv2;

		v[3].color = {1.f, 0.f, 0.f};
		v[3].pos = transform * glm::vec3{0.5, -0.5, 1.f};
		v[3].textureID = 0;
		v[3].uv = {texture.uv2.x, texture.uv1.y};

		RainDrop::renderSceneQuad(&v[0], &v[1], &v[2], &v[3]);
	}
}

void PlayerSystem::setSignature(){
	RainDrop::ECSSignature signature;
	signature.set(RainDrop::getComponentID<Transform>());
	signature.set(RainDrop::getComponentID<PlayerComponent>());
	signature.set(RainDrop::getComponentID<RainDrop::Sound>());
	signature.set(RainDrop::getComponentID<TextureComponent>());
	RainDrop::setECSSystemSignature<PlayerSystem>(signature);
}

void MissileSystem::init(){
	setSignature();
}

void MissileSystem::update(float dt){

}

void MissileSystem::render(){

}

void MissileSystem::setSignature(){
	RainDrop::ECSSignature signature;
	signature.set(RainDrop::getComponentID<Transform>());
	signature.set(RainDrop::getComponentID<MissileComponent>());
	signature.set(RainDrop::getComponentID<RainDrop::Sound>());
	signature.set(RainDrop::getComponentID<TextureComponent>());
	RainDrop::setECSSystemSignature<MissileSystem>(signature);
}


static bool launched = false;

bool onWindowClosed(void*){
	launched = false;
	return false;
}

void spawnEnemy(){
	int x = rand() % 720;
	RainDrop::Entity enemy = RainDrop::createEntity();

	auto& transform = enemy.addComponent<Transform>().transform;
	auto& enemyComponent = enemy.addComponent<EnemyComponent>();
	auto& sound = enemy.addComponent<RainDrop::Sound>();
	auto& texture = enemy.addComponent<TextureComponent>();

	transform = glm::translate(glm::mat3(1.f), {static_cast<float>(x), -150.f});
	enemyComponent.health = 150;
	enemyComponent.size = {100.f, 100.f};

	transform *= glm::scale(glm::mat3(1.f), enemyComponent.size);

	texture.texture = 0;
	texture.uv1 = {22, 35};
	texture.uv2 = {145, 169};
}

void spawnPlayer(){
	int x = 360;
	RainDrop::Entity player = RainDrop::createEntity();

	auto& transform = player.addComponent<Transform>().transform;
	auto& playerComponent = player.addComponent<PlayerComponent>();
	auto& sound = player.addComponent<RainDrop::Sound>();
	auto& texture = player.addComponent<TextureComponent>();

	transform = glm::translate(glm::mat3(1.f), {static_cast<float>(x), 1500});
	playerComponent.health = 150;
	playerComponent.size = {100.f, 100.f};

	transform *= glm::scale(glm::mat3(1.f), playerComponent.size);

	texture.texture = 0;
	texture.uv1 = {22, 35};
	texture.uv2 = {145, 169};
}

struct PushConstant{
	glm::mat4 cameraTransform;
};

RainDrop::Texture loadSpaceShips(const std::filesystem::path &path){
	RainDrop::TextureCreateInfo createInfo;
	createInfo.normalizeCoord(false);
	return RainDrop::createTextureFromPath((path / ".data/images/spaceShips.png").string().c_str(), createInfo);
}

RainDrop::DescriptorSet createDefaultDescriptorSet(RainDrop::TextureID* textures, uint32_t textureCount){
	RainDrop::DescriptorSetCreateInfo createInfo;

	RainDrop::DescriptorSetCreateInfo::DescriptorInfo descriptor;
	descriptor.binding = 0;
	descriptor.stage = static_cast<int>(RainDrop::ShaderStage::Fragment) | static_cast<int>(RainDrop::ShaderStage::Vertex);
	descriptor.textureCount = textureCount;
	descriptor.textures = textures;

	createInfo.setSetCount(2);
	return RainDrop::createDescriptorSet(createInfo);
}

RainDrop::Shader loadDefaultShader(const std::filesystem::path &path, RainDrop::DescriptorSet set){
	RainDrop::ShaderCreateInfo createInfo;
	createInfo.setPushConstant<PushConstant>();
	createInfo.setRenderTarget(-1);
	createInfo.setFragmentFilepath((path / ".data/shaders/defaultShader/defaultShader.frag.spv").string().c_str());
	createInfo.setVertexFilepath((path / ".data/shaders/defaultShader/defaultShader.vert.spv").string().c_str());
	createInfo.setVertexInputSize(sizeof(DefaultShaderVertex));

	RainDrop::ShaderCreateInfo::VertexAttribute attributes[4];
	attributes[0].format = RainDrop::TypeFormat::Float_vec2;
	attributes[0].offset = offsetof(DefaultShaderVertex, pos);

	attributes[1].format = RainDrop::TypeFormat::Float_vec3;
	attributes[1].offset = offsetof(DefaultShaderVertex, color);

	attributes[2].format = RainDrop::TypeFormat::Float_vec2;
	attributes[2].offset = offsetof(DefaultShaderVertex, uv);

	attributes[3].format = RainDrop::TypeFormat::Uint;
	attributes[3].offset = offsetof(DefaultShaderVertex, textureID);

	createInfo.setVertexAttributes(attributes, 4);
	createInfo.setDescriptorSets(&set, 1);
	
	return RainDrop::createShader(createInfo);
}

bool onWindowResized(void* ptr, void* data){
	PushConstant* push = static_cast<PushConstant*>(ptr);
	RainDrop::vec2<uint32_t>* size = static_cast<RainDrop::vec2<uint32_t>*>(data);
	
	push->cameraTransform = glm::ortho(0.f, static_cast<float>(size->x), 0.f, static_cast<float>(size->y), -1.0f, 1.0f);
	return false;
}

bool onEntityDeleted(void* data){
	RainDrop::EntityID* id = static_cast<RainDrop::EntityID*>(data);
	RainDrop::destroyEntity(*id);
	printf("aaaa %d\n", id);
	return false;
}

int main(int argc, char** argv){
	std::filesystem::path gamePath = std::string(argv[0]);
	gamePath = gamePath.parent_path();

	printf("%s\n", gamePath.string().c_str());
	PushConstant pushConstant;

	RainDrop::initialize();
	RainDrop::setWindowSize(RainDrop::vec2<uint32_t>{720, 1820});
	RainDrop::setWindowPosition(RainDrop::vec2<uint32_t>{500, 50});
	RainDrop::setWindowResizable(true);

	RainDrop::registerEvent("launch missile", sizeof(int) + sizeof(glm::vec2));
	RainDrop::registerEvent("missile contact", sizeof(RainDrop::EntityID) * 2);
	RainDrop::registerEvent("delete enemy", sizeof(RainDrop::EntityID));

	RainDrop::subscribeEvent("window closed", &onWindowClosed);
	RainDrop::subscribeEvent("window resized", &pushConstant, &onWindowResized);
	RainDrop::subscribeEvent("delete enemy", onEntityDeleted);
	
	RainDrop::registerEntityComponent<Transform>();
	RainDrop::registerEntityComponent<RainDrop::Sound>();
	RainDrop::registerEntityComponent<TextureComponent>();
	RainDrop::registerEntityComponent<EnemyComponent>();
	RainDrop::registerEntityComponent<PlayerComponent>();
	RainDrop::registerEntityComponent<MissileComponent>();

	MissileSystem* missileSystem = RainDrop::registerECSSystem<MissileSystem>();
	PlayerSystem* playerSystem = RainDrop::registerECSSystem<PlayerSystem>(missileSystem);
	EnemySystem* enemySystem = RainDrop::registerECSSystem<EnemySystem>(missileSystem);

	RainDrop::Texture spaceShips = loadSpaceShips(gamePath);
	RainDrop::DescriptorSet descriptor = createDefaultDescriptorSet(&spaceShips, 1);
	RainDrop::Shader defaultShader = loadDefaultShader(gamePath, descriptor);

	launched = true;
	auto currentTime = std::chrono::high_resolution_clock::now();

	pushConstant.cameraTransform = glm::ortho(0.f, 720.f, 0.f, 1820.f, -1.0f, 1.0f);
	RainDrop::setSceneVertexSize(sizeof(DefaultShaderVertex));

	spawnPlayer();

	float t = 0;
	
	while (launched){
		auto newTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
		currentTime = std::chrono::high_resolution_clock::now();
		t += dt;

		if (t > 0.1){
			spawnEnemy();
			t = 0;
		}

		RainDrop::updateEvents();

		missileSystem->update(dt);
		playerSystem->update(dt);
		enemySystem->update(dt);

		RainDrop::beginFrame();
		RainDrop::beginSwapChainRenderPass();

		uint32_t index = 0;
		RainDrop::useShader(defaultShader, &index);
		RainDrop::setShaderPushConstant(defaultShader, &pushConstant);

		missileSystem->render();
		playerSystem->render();
		enemySystem->render();

		RainDrop::renderScene();
	
		RainDrop::endSwapChainRenderPass();
		RainDrop::endFrame();

		// printf("%f\n", dt);
	}

	RainDrop::shutdown();
	return EXIT_SUCCESS;
}
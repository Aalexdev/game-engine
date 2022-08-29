#include <iostream>
#include <filesystem>

#include "include/RainDrop.hpp"
#include "include/glm/glm.hpp"

bool launched = true;

bool onWindowClosed(void*){
	launched = false;
	return false;
}

struct Vertex{
	glm::vec3 pos;
	glm::vec3 color;
};

struct PushConstant{
	glm::mat4 mat;
};

int main(int argc, char** argv){
	std::filesystem::path path = argv[0];
	path = path.parent_path();

	RainDrop::initialize();
	RainDrop::subscribeEvent("window closed", &onWindowClosed);

	RainDrop::Texture texture;
	{
		RainDrop::TextureCreateInfo info;
		texture = RainDrop::createTextureFromPath((path / ".raindrop_data/textures/Capture.2PNG.PNG").string().c_str(), info);
	}

	RainDrop::DescriptorSet set;
	{
		RainDrop::DescriptorSetCreateInfo info;
		RainDrop::DescriptorSetCreateInfo::DescriptorInfo infos[1];

		infos[0].binding = 0;
		infos[0].type = RainDrop::DescriptorType::TextureID;
		infos[0].textureCount = 1;
		infos[0].textures = &texture;

		info.setSetCount(2);
		info.setDescriptors(infos, 1);

		set = RainDrop::createDescriptorSet(info);
	}
	
	RainDrop::Shader shader;
	{	
		RainDrop::ShaderCreateInfo info;
		RainDrop::ShaderCreateInfo::VertexAttribute attributes[2];

		attributes[0].format = RainDrop::TypeFormat::Float_vec3;
		attributes[0].offset = offsetof(Vertex, pos);
		attributes[1].format = RainDrop::TypeFormat::Float_vec3;
		attributes[1].offset = offsetof(Vertex, color);

		info.setFragmentFilepath((path / ".raindrop_data/shaders/defaultShader.frag.spv").string().c_str());
		info.setVertexFilepath((path / ".raindrop_data/shaders/defaultShader.vert.spv").string().c_str());
		info.setPushConstant<PushConstant>();
		info.setVertexInputSize(sizeof(Vertex));
		info.setVertexAttributes(attributes, 2);
		info.setDescriptorSets(&set, 1);
		shader = RainDrop::createShader(info);
	}

	RainDrop::setSceneVertexSize(sizeof(Vertex));

	Vertex v0;
	Vertex v1;
	Vertex v2;

	v0.pos = {-0.5, -0.5, 0.0};
	v0.color = {1.f, 1.f, 1.f};
	
	v1.pos = {0.5, -0.5, 0.0};
	v1.color = {1.f, 0.f, 1.f};

	v2.pos = {0.5, 0.5, 0.0};
	v2.color = {0.f, 0.f, 1.f};

	PushConstant push;
	uint32_t frame = 0;
	while (launched){
		RainDrop::updateEvents();

		RainDrop::beginFrame();
		RainDrop::beginSwapChainRenderPass();
	
		RainDrop::renderSceneTrigone(&v0, &v1, &v2);
		
		shader.use(&frame);
		shader.setPushConstant(push);
		RainDrop::renderScene();

		RainDrop::endSwapChainRenderPass();
		RainDrop::endFrame();

		frame = (frame + 1) % 2;
	}

	RainDrop::shutdown();

	return 0;
}

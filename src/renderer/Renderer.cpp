#include "engine/renderer/Renderer.hpp"
#include "engine/Exception.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/filesystem/Filesystem.hpp"

#include "engine/renderer/SubTexture2D.hpp"
#include "engine/OpenGL_AL/OpenGL_Renderer.hpp"

static void checkDefinition(const engine::Renderer::Definition &def){
	ENGINE_PROFILE_FUNCTION();
	if (def.display == nullptr) throw engine::exceptions::BadParameter("cannot create a context from a null display");
}

namespace engine{

	Ref<Renderer> Renderer::create(const Definition &def){
		ENGINE_PROFILE_FUNCTION();
		Ref<Renderer> renderer = nullptr;
		switch (def.renderAPI){
			case RenderAPI::OPENGL: renderer = OpenGL::Renderer::create(def);
		}

		if (renderer){

			renderer->loadShader("default", "shaders/shader.glsl");

			renderer->renderAPI = def.renderAPI;
			
			for (int i=0; i<2; i++){
				renderer->dataBuffers[i].reserve(def.bufferSize);
				renderer->commandQueues[i].reserve(def.commandCount);
			}

			auto &pos = renderer->vertexPosition;

			pos[0] = {-0.5f, -0.5f, 1.f};
			pos[1] = { 0.5f, -0.5f, 1.f};
			pos[2] = { 0.5f,  0.5f, 1.f};
			pos[3] = {-0.5f,  0.5f, 1.f};

			return renderer;
		}

		ENGINE_CORE_ASSERT(false, "unknown RenderAPI");
		return nullptr;
	}

	void Renderer::drawQuad(const glm::mat3 &transform, uint32_t entityIndex){
		ENGINE_PROFILE_FUNCTION();
		auto &data = dataBuffers[currentQueue];
		RenderCommand command;
		command.category = RenderCommand::Category::DRAW_QUAD;

		command.data = pushMat3(transform);
		pushFloat(static_cast<float>(entityIndex));
		commandQueues[currentQueue].push_back(command);
	}

	void Renderer::drawQuad(const glm::mat3 &transform, const glm::vec4 &color, uint32_t entityIndex){
		ENGINE_PROFILE_FUNCTION();
		RenderCommand command;
		command.category = RenderCommand::Category::DRAW_QUAD_COLORED;
		
		command.data = pushMat3(transform);
		pushVec4(color);
		pushFloat(static_cast<float>(entityIndex));
		commandQueues[currentQueue].push_back(command);
	}

	void Renderer::drawTexturedQuad(const glm::mat3 &transform, const Ref<SubTexture2D> &texture, uint32_t entityIndex){
		ENGINE_PROFILE_FUNCTION();
		auto &data = dataBuffers[currentQueue];
		RenderCommand command;
		command.category = RenderCommand::Category::DRAW_TEXTURED_QUAD;
		
		command.data = pushMat3(transform);
		pushTexture(texture);
		pushFloat(static_cast<float>(entityIndex));
		commandQueues[currentQueue].push_back(command);
	}

	void Renderer::drawTexturedQuad(const glm::mat3 &transform, const Ref<SubTexture2D> &texture, const glm::vec4 &color, uint32_t entityIndex){
		ENGINE_PROFILE_FUNCTION();
		auto &data = dataBuffers[currentQueue];
		RenderCommand command;
		command.category = RenderCommand::Category::DRAW_TEXTURED_QUAD_COLORED;

		command.data = pushMat3(transform);
		pushTexture(texture);
		pushVec4(color);
		pushFloat(static_cast<float>(entityIndex));
		commandQueues[currentQueue].push_back(command);
	}

	void Renderer::setRefreshRate(uint16_t refreshRate){
		ENGINE_PROFILE_FUNCTION();
		auto &data = dataBuffers[currentQueue];
		RenderCommand command;
		command.category = RenderCommand::Category::SET_REFRESH_RATE;
		command.data = pushFloat(static_cast<float>(refreshRate));
		commandQueues[currentQueue].push_back(command);
	}

	void Renderer::setClearColor(const glm::vec3 &color){
		ENGINE_PROFILE_FUNCTION();
		auto &data = dataBuffers[currentQueue];
		RenderCommand command;
		command.category = RenderCommand::Category::SET_CLEAR_COLOR;
		command.data = pushVec3(color);
		commandQueues[currentQueue].push_back(command);
	}

	void Renderer::setViewport(const glm::u32vec2 &viewport){
		ENGINE_PROFILE_FUNCTION();
		auto &data = dataBuffers[currentQueue];
		RenderCommand command;
		command.category = RenderCommand::Category::SET_VIEWPORT;
		command.data = pushVec2(glm::vec2(viewport));
		commandQueues[currentQueue].push_back(command);
	}

	void Renderer::resize(const glm::u32vec2 &resolution){
		ENGINE_PROFILE_FUNCTION();
		auto &data = dataBuffers[currentQueue];
		RenderCommand command;
		command.category = RenderCommand::Category::SET_RESOLUTION;
		command.data = pushVec2(glm::vec2(resolution));
		commandQueues[currentQueue].push_back(command);
	}

	void Renderer::enableVSync(bool enabled){
		ENGINE_PROFILE_FUNCTION();
		auto &data = dataBuffers[currentQueue];
		RenderCommand command;
		command.category = RenderCommand::Category::ENABLE_VSYNC;
		command.data = pushFloat(static_cast<float>(enabled));
		commandQueues[currentQueue].push_back(command);
	}

	void Renderer::clear(){
		ENGINE_PROFILE_FUNCTION();
		RenderCommand command;
		command.category = RenderCommand::Category::CLEAR;
		commandQueues[currentQueue].push_back(command);
	}

	void Renderer::useShader(const std::string &name){
		ENGINE_PROFILE_FUNCTION();
		RenderCommand command;
		command.category = RenderCommand::Category::USE_SHADER;
		command.data = pushStr(name);
		commandQueues[currentQueue].push_back(command);
	}

	void Renderer::setViewProjectionMat(const glm::mat3 &mat){
		ENGINE_PROFILE_FUNCTION();
		RenderCommand command;
		command.category = RenderCommand::Category::SET_VIEW_PROJECTION_MAT;
		command.data = pushMat3(mat);
		commandQueues[currentQueue].push_back(command);
	}

	float* Renderer::pushMat3(const glm::mat3 &mat){
		ENGINE_PROFILE_FUNCTION();
		auto &data = dataBuffers[currentQueue];
		
		data.push_back(mat[0][0]);
		float *ptr = &data.back();

		data.push_back(mat[0][1]);
		data.push_back(mat[0][2]);

		data.push_back(mat[1][0]);
		data.push_back(mat[1][1]);
		data.push_back(mat[1][2]);

		data.push_back(mat[2][0]);
		data.push_back(mat[2][1]);
		data.push_back(mat[2][2]);
		return ptr;
	}

	float* Renderer::pushVec4(const glm::vec4 &vec){
		ENGINE_PROFILE_FUNCTION();
		auto &data = dataBuffers[currentQueue];
		data.push_back(vec.x);
		float *ptr = &data.back();
		data.push_back(vec.y);
		data.push_back(vec.z);
		data.push_back(vec.w);
		return ptr;
	}

	float* Renderer::pushVec3(const glm::vec3 &vec){
		ENGINE_PROFILE_FUNCTION();
		auto &data = dataBuffers[currentQueue];
		data.push_back(vec.x);
		float *ptr = &data.back();
		data.push_back(vec.y);
		data.push_back(vec.z);
		return ptr;
	}

	float* Renderer::pushVec2(const glm::vec2 &vec){
		ENGINE_PROFILE_FUNCTION();
		auto &data = dataBuffers[currentQueue];
		data.push_back(vec.x);
		float *ptr = &data.back();
		data.push_back(vec.y);
		return ptr;
	}

	float* Renderer::pushFloat(float value){
		ENGINE_PROFILE_FUNCTION();
		auto &data = dataBuffers[currentQueue];
		data.push_back(value);
		return &data.back();
	}

	float* Renderer::pushTexture(const Ref<SubTexture2D> &texture){
		ENGINE_PROFILE_FUNCTION();
		float *ptr = pushFloat(static_cast<float>(pushTexture(texture->getTexture())));
		pushVec2(texture->getStart());
		pushVec2(texture->getEnd());
		return ptr;
	}

	float* Renderer::pushStr(const std::string &str){
		ENGINE_PROFILE_FUNCTION();
		float *ptr = pushFloat(static_cast<float>(str.size()));
		for (const char &c : str){
			pushFloat(static_cast<float>(c));
		}
		return ptr;
	}
	
	void Renderer::popMat3(glm::mat3 &mat, float *data, uint32_t &i){
		for (int x=0; x<3; x++){
			for (int y=0; y<3; y++){
				mat[x][y] = data[i++];
			}
		}
	}

	void Renderer::popVec4(glm::vec4 &vec, float *data, uint32_t &i){
		for (int j=0; j<4; j++){
			vec[j] = data[i++];
		}
	}

	void Renderer::popVec3(glm::vec3 &vec, float *data, uint32_t &i){
		for (int j=0; j<3; j++){
			vec[j] = data[i++];
		}
	}

	void Renderer::popVec2(glm::vec2 &vec, float *data, uint32_t &i){
		for (int j=0; j<2; j++){
			vec[j] = data[i++];
		}
	}

	void Renderer::popFloat(float &value, float *data, uint32_t &i){
		value = data[i++];
	}

	void Renderer::popTexture(SubTexture &texture, float *data, uint32_t &i){
		static float index;
		popFloat(index, data, i);
		texture.textureIndex  = static_cast<uint32_t>(index);
		popVec2(texture.start, data, i);
		popVec2(texture.end, data, i);
	}

	void Renderer::popStr(std::string &str, float *data, uint32_t &i){
		static float size;
		popFloat(size, data, i);
		str.resize(static_cast<size_t>(size));
		for (int j=0; j<str.size(); j++){
			static float c;
			popFloat(c, data, i);
			str[j] = static_cast<char>(c);
		}
	}

	void Renderer::popBool(bool &value, float *data, uint32_t &i){
		float v;
		popFloat(v, data, i);
		value = v == 1.f;
	}
	
	void Renderer::render(){
		ENGINE_PROFILE_FUNCTION();
		auto &queue = commandQueues[(currentQueue + 1) % 2];
		auto &data = dataBuffers[(currentQueue + 1) % 2];

		bindDefaultShader();

		for (auto &command : queue){
			switch (command.category){
				case RenderCommand::Category::DRAW_QUAD: renderQuad(command.data); break;
				case RenderCommand::Category::DRAW_QUAD_COLORED: renderQuadColored(command.data); break;
				case RenderCommand::Category::DRAW_TEXTURED_QUAD: renderTexturedQuad(command.data); break;
				case RenderCommand::Category::DRAW_TEXTURED_QUAD_COLORED: renderTexturedQuadColored(command.data); break;
				case RenderCommand::Category::SET_REFRESH_RATE: setRefreshRate(command.data); break;
				case RenderCommand::Category::SET_CLEAR_COLOR: setClearColor(command.data); break;
				case RenderCommand::Category::SET_VIEWPORT: setViewport(command.data); break;
				case RenderCommand::Category::RESIZED: resized(command.data); break;
				case RenderCommand::Category::ENABLE_VSYNC: enableVSync(command.data); break;
				case RenderCommand::Category::CLEAR: clear(command.data); break;
				case RenderCommand::Category::USE_SHADER: useShader(command.data); break;
				case RenderCommand::Category::SET_VIEW_PROJECTION_MAT: setViewProjectionMat(command.data); break;
			}
		}
	}
	
	void Renderer::renderQuad(float *data){
		ENGINE_PROFILE_FUNCTION();
		static glm::mat3 transform;
		static float entityIndex;

		uint32_t i=0;

		popMat3(transform, data, i);
		popFloat(entityIndex, data, i);

		drawQuad_impl_(transform, glm::vec4(1.f), {}, static_cast<uint32_t>(entityIndex));
	}

	void Renderer::renderQuadColored(float *data){
		ENGINE_PROFILE_FUNCTION();
		static glm::mat3 transform;
		static glm::vec4 color;
		static float entityIndex;

		uint32_t i=0;

		popMat3(transform, data, i);
		popVec4(color, data, i);
		popFloat(entityIndex, data, i);

		drawQuad_impl_(transform, color, {}, static_cast<uint32_t>(entityIndex));

	}

	void Renderer::renderTexturedQuad(float *data){
		ENGINE_PROFILE_FUNCTION();
		static glm::mat3 transform;
		static SubTexture texture;
		static float entityIndex;

		uint32_t i=0;
		popMat3(transform, data, i);
		popTexture(texture, data, i);
		popFloat(entityIndex, data, i);

		drawQuad_impl_(transform, glm::vec4(1.f), texture, static_cast<uint32_t>(entityIndex));
	}

	void Renderer::renderTexturedQuadColored(float *data){
		ENGINE_PROFILE_FUNCTION();
		static glm::mat3 transform;
		static SubTexture texture;
		static glm::vec4 color;
		static float entityIndex;

		uint32_t i=0;
		popMat3(transform, data, i);
		popTexture(texture, data, i);
		popVec4(color, data, i);
		popFloat(entityIndex, data, i);

		drawQuad_impl_(transform, color, texture, static_cast<uint32_t>(entityIndex));
	}
	
	void Renderer::setRefreshRate(float *data){
		ENGINE_PROFILE_FUNCTION();
		static float fps;
		uint32_t i=0;
		popFloat(fps, data, i);
		setRefreshRate(fps);
	}

	void Renderer::setClearColor(float *data){
		ENGINE_PROFILE_FUNCTION();
		static glm::vec3 color;
		uint32_t i=0;
		popVec3(color, data, i);
		setClearColor_impl_(color);
	}

	void Renderer::setViewport(float *data){
		ENGINE_PROFILE_FUNCTION();
		static glm::vec2 viewport;
		uint32_t i=0;
		popVec2(viewport, data, i);
		setViewport_impl_(viewport);
	}

	void Renderer::resized(float *data){
		ENGINE_PROFILE_FUNCTION();
		static glm::vec2 size;
		uint32_t i=0;
		popVec2(size, data, i);
		resized_impl_(size);
	}

	void Renderer::enableVSync(float *data){
		ENGINE_PROFILE_FUNCTION();
		static bool enabled;
		uint32_t i=0;
		popBool(enabled, data, i);
		enableVSync_impl_(enabled);
	}

	void Renderer::clear(float *data){
		ENGINE_PROFILE_FUNCTION();
		clear_impl_();
	}

	void Renderer::useShader(float *data){
		ENGINE_PROFILE_FUNCTION();
		static std::string str;
		uint32_t i=0;
		popStr(str, data, i);
		useShader_impl_(str);
	}

	void Renderer::setViewProjectionMat(float *data){
		ENGINE_PROFILE_FUNCTION();
		static glm::mat3 mat;
		uint32_t i=0;
		popMat3(mat, data, i);
		setViewProjectionMat_impl_(mat);
	}

	void Renderer::loadShader(const std::string &name, std::filesystem::path path){
		ENGINE_PROFILE_FUNCTION();
		loadShader_impl_(name, path);
	}

	RenderAPI Renderer::getRenderAPI() const{
		return renderAPI;
	}
}
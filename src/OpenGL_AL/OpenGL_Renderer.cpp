#include "engine/OpenGL_AL/OpenGL_Renderer.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/Exception.hpp"
#include "engine/displays/Display.hpp"
#include "engine/filesystem/Filesystem.hpp"

#include <libs/glad/glad.h>
#include <libs/SDL2/SDL.h>


namespace engine::OpenGL{
	Ref<Renderer> Renderer::create(const Definition &def){
		return createRef<Renderer>(def);
	}

	Renderer::Renderer(const Definition &def) : VSyncEnabled{def.VSync}{
		ENGINE_PROFILE_FUNCTION();
		renderAPI = RenderAPI::OPENGL;
		properties = def;

		context = SDL_GL_CreateContext(reinterpret_cast<SDL_Window*>(def.display->getNativeWindow()));
		if (context == nullptr) throw engine::exceptions::API(std::string("SDL_GL_CreateContext : ") + SDL_GetError());
		
		if (gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress) == 0) throw engine::exceptions::API("gladLoadGLLoader : failed to initialize GLAD");

		initializeVertexBuffer();

		if (def.refreshRate == REFRESH_RATE_UNLIMITED){
			minSecPerFrame = 0.f;
		} else {
			minSecPerFrame = 1.f / static_cast<float>(def.refreshRate);
		}

		FramebufferLayout layout;
		layout.width = def.resolution.x;
		layout.height = def.resolution.y;
		layout.attachments = def.attachments;
		framebuffer = engine::Framebuffer::create(this, layout);

		for (uint8_t i=0; i<2; i++){
			textureSlots[i] = createScope<Ref<Texture2D>[]>(getMaxTextureSlots());
		}
	}

	Renderer::~Renderer(){
		ENGINE_PROFILE_FUNCTION();
		SDL_GL_DeleteContext(context);
	}

	void Renderer::swap(){
		ENGINE_PROFILE_FUNCTION();
		SDL_GL_SwapWindow(reinterpret_cast<SDL_Window*>(properties.display->getNativeWindow()));

		currentQueue = (currentQueue + 1) % 2;
		dataBuffers[currentQueue].clear();
		commandQueues[currentQueue].clear();
		textureSlotIndex[currentQueue] = 0;

		render();
	}

	void Renderer::reloadScene(){
		ENGINE_PROFILE_FUNCTION();
		
		vertexBufferPtr = vertexBufferBase;
		indexCount = 0;
		textureSlotIndex[(currentQueue + 1) % 2] = 1;
	}

	void Renderer::flush(){
		ENGINE_PROFILE_FUNCTION();
		
		if (indexCount == 0) return;
	
		uint32_t size = reinterpret_cast<uint32_t*>(vertexBufferPtr) - reinterpret_cast<uint32_t*>(vertexBufferBase);
		vertexBuffer->setData(vertexBufferBase, size);

		for (int i=static_cast<int>(0); i<textureSlotIndex[currentQueue]; i++){
			textureSlots[(currentQueue + 1) % 2][i]->bind(i);
		}

		vertexArray->bind();
		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, nullptr);
	}
	
	void Renderer::endScene(){
		ENGINE_PROFILE_FUNCTION();
		flush();
		reloadScene();
	}

	uint8_t Renderer::getMaxTextureSlots(){
		ENGINE_PROFILE_FUNCTION();
		int units;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &units);
		return static_cast<uint8_t>(units);
	}

	void Renderer::initializeVertexBufferLayout(BufferLayout &layout){
		ENGINE_PROFILE_FUNCTION();
		layout = {
			{DataType::Float2, "position"},
			{DataType::Float2, "UV"},
			{DataType::Float4, "color"},
			{DataType::UInt, "textureIndex"},
			{DataType::UInt, "entityIndex"},
			{DataType::Float, "tilingFactor"},
		};
	}

	void Renderer::initializeVertexBuffer(){
		vertexArray = VertexArray::create(this);
		vertexBuffer = VertexBuffer::create(this, sizeof(Vertex) * maxVertices);

		BufferLayout bufferLayout;
		initializeVertexBufferLayout(bufferLayout);

		vertexBuffer->setLayout(bufferLayout);
		vertexArray->addVertexBuffer(vertexBuffer);

		vertexBufferBase = new Vertex[maxVertices];
		uint32_t *indices = new uint32_t[maxIndices];

		uint32_t offset = 0;
		for (uint32_t i=0; i<maxIndices; i+=6){
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;
			offset += 4;
		}

		Ref<IndexBuffer> indexBuffer = IndexBuffer::create(this, indices, maxIndices);
		vertexArray->setIndexBuffer(indexBuffer);

		delete[] indices;
	}

	void Renderer::drawQuad_impl_(const glm::mat3 &transform, const glm::vec4 &color, const SubTexture &texture, uint32_t entityIndex){

		if (indexCount >= maxIndices){
			flush();
			reloadScene();
		}

		glm::vec2 textureCoords[] = {
			{texture.start.x, texture.end.y},
			texture.end,
			{texture.end.x, texture.start.y},
			texture.start
		};

		for (uint8_t i=0; i<4; i++){
			vertexBufferPtr->position = transform * vertexPosition[i];
			vertexBufferPtr->color = color;
			vertexBufferPtr->UV + textureCoords[i];
			vertexBufferPtr->textureIndex = texture.textureIndex;
			vertexBufferPtr->tilingFactor = 1.f;
			vertexBufferPtr->entityIndex = entityIndex;
			vertexBufferPtr++;
		}

		indexCount += 6;
	}

	void Renderer::setRefreshRate_impl_(float fps){
		minSecPerFrame = 1.f / fps;
	}

	void Renderer::setClearColor_impl_(const glm::vec3 &color){
		glClearColor(color.r, color.g, color.b, 1.f);
	}

	void Renderer::setViewport_impl_(const glm::vec2 &viewport){
		glViewport(0, 0, static_cast<int>(viewport.x), static_cast<int>(viewport.y));
	}

	void Renderer::resized_impl_(const glm::vec2 &size){
		framebuffer->resize(static_cast<uint32_t>(size.x), static_cast<uint32_t>(size.y));
	}

	void Renderer::enableVSync_impl_(bool enabled){
		VSyncEnabled = enabled;
		if (SDL_GL_SetSwapInterval(static_cast<int>(enabled)) == -1){
			throw exceptions::API(std::string("SDL_GL_SetSwapInterval : ") + SDL_GetError());
		}
	}

	void Renderer::clear_impl_(){
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::useShader_impl_(const std::string &name){
		auto it = shaders.find(name);
		if (it == shaders.end()) return;

		currentShader = it->second;
		currentShader->bind();
		currentShader->setMat3(viewProjectionMat, "u_viewProjection");
	}

	void Renderer::setViewProjectionMat_impl_(const glm::mat3 &mat){
		viewProjectionMat = mat;
		if (currentShader) currentShader->setMat3(viewProjectionMat, "u_viewProjection");
	}

	void Renderer::loadShader_impl_(const std::string &name, std::filesystem::path path){
		if (path.is_relative()){
			path = Filesystem::getDataFolderPath() / path;
		}

		ENGINE_CORE_ASSERT(std::filesystem::exists(path), "cannot load a non existant shader file : ", path);
		Ref<Shader> shader = Shader::create(this, path.string());

		ENGINE_CORE_ASSERT(shader != nullptr, "failed to load shader : ", path);
		shaders[name] = shader;

		
		const uint8_t size = getMaxTextureSlots();
		int samplers[size];
		for (uint8_t i=0; i<size; i++){
			samplers[i] = i;
		}

		shader->setIntArray(samplers, size, "u_textures");
	}

	uint8_t Renderer::texturePushed(const Ref<Texture2D> &texture){
		for (uint8_t i=0; i<textureSlotIndex[currentQueue]; i++){
			if (textureSlots[currentQueue][i] == texture) return i;
		}
		return static_cast<uint8_t>(-1);
	}

	uint8_t Renderer::pushTexture(const Ref<Texture2D> &texture){
		if (!texture) return 0;

		uint8_t pushed = texturePushed(texture);

		if (pushed != static_cast<uint8_t>(-1)) return 0;

		if (textureSlotIndex[currentQueue] == getMaxTextureSlots()){
			flush();
			reloadScene();
		}

		textureSlots[currentQueue][textureSlotIndex[currentQueue]] = texture;
		textureSlotIndex[currentQueue]++;
		return textureSlotIndex[currentQueue]-1;
	}

	void Renderer::bindDefaultShader(){
		if (shaders.empty()) throw exceptions::BadParameter("no shaders initialized");
		shaders.begin()->second->bind();
	}
}
#include "engine/pch.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/renderer/Renderer.hpp"
#include "engine/filesystem/Filesystem.hpp"

#include <libs/glm/glm.hpp>
#include <libs/glm/gtc/matrix_transform.hpp>

namespace engine{
	Renderer::RenderAPI Renderer::renderAPI = Renderer::RenderAPI::OpenGL;

	void Renderer::init(){
		ENGINE_PROFILE_FUNCTION();
		rendererAPI = RendererAPI::create();
		rendererAPI->init();

		quadVertexPositions[0] = {-0.5f, -0.5f, 0.f, 1.f};
		quadVertexPositions[1] = { 0.5f, -0.5f, 0.f, 1.f};
		quadVertexPositions[2] = { 0.5f,  0.5f, 0.f, 1.f};
		quadVertexPositions[3] = {-0.5f,  0.5f, 0.f, 1.f};

		loadShaders();
		reloadScene();

		for (auto &buffer : renderCommandBuffers){
			buffer.reserve(350);
		}
	}

	void Renderer::loadShaders(){
		loadCircleShader();
		loadQuadShader();
		loadLineShader();
	}

	void Renderer::loadQuadShader(){
		quadData = createScope<QuadData>(1000);

		// setup the quad datas
		quadData->quadVertexArray = VertexArray::create();
		quadData->quadVertexBuffer = VertexBuffer::create(sizeof(QuadVertex) * quadData->maxVertices);

		BufferLayout quadBufferLayout = {
			{DataType::Float3, "position"},
			{DataType::Float2, "UV"},
			{DataType::Float4, "color"},
			{DataType::UInt, "textureIndex"},
			{DataType::Float, "tilingFactor"},
			{DataType::UInt, "entityIndex"}
		};

		quadData->quadVertexBuffer->setLayout(quadBufferLayout);
		quadData->quadVertexArray->addVertexBuffer(quadData->quadVertexBuffer);

		quadData->quadVertexBufferBase = new QuadVertex[quadData->maxVertices];
		uint32_t *indices = new uint32_t[quadData->maxIndices];

		uint32_t offset = 0;
		for (uint32_t i=0; i<quadData->maxIndices; i+=6){
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;
			offset += 4;
		}

		Ref<IndexBuffer> indexBuffer = IndexBuffer::create(indices, quadData->maxIndices);
		quadData->quadVertexArray->setIndexBuffer(indexBuffer);

		// will not work if the indices are send to the gpu separalty from another thread
		delete[] indices;

		// textures
		int samplers[quadData->maxTextureSlots];
		for (uint32_t i=0; i<quadData->maxTextureSlots; i++){
			samplers[i] = i;
		}

		quadShader = Shader::create((Filesystem::getDataFolderPath() / "shaders/quadShader.glsl").string());
		quadShader->bind();

		quadData->textureSlots = createScope<Ref<Texture2D>[]>(quadData->maxTextureSlots);
		quadShader->setIntArray(samplers, quadData->maxTextureSlots, "u_textures");

		uint32_t color = 0xFFFFFF;
		Ref<Texture2D> whiteTexture = Texture2D::create(1, 1, TextureFormat::RGB, &color);

		quadData->textureSlots[0] = whiteTexture;
	}

	void Renderer::loadCircleShader(){
		circleData = createScope<CircleData>(1000);

		circleData->circleVertexArray = VertexArray::create();
		circleData->circleVertexBuffer = VertexBuffer::create(sizeof(CircleVertex) * circleData->maxVertices);

		BufferLayout circleBufferLayout = {
			{DataType::Float3, "position"},
			{DataType::Float2, "UV"},
			{DataType::Float4, "color"},
			{DataType::Float, "thickness"},
			{DataType::Float, "fade"},
			{DataType::UInt, "entityIndex"}
		};

		circleData->circleVertexBuffer->setLayout(circleBufferLayout);
		circleData->circleVertexArray->addVertexBuffer(circleData->circleVertexBuffer);

		circleData->circleVertexBufferBase = new CircleVertex[circleData->maxVertices];
		uint32_t *indices = new uint32_t[circleData->maxIndices];

		uint32_t offset = 0;
		for (uint32_t i=0; i<circleData->maxIndices; i+=6){
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;
			offset += 4;
		}

		Ref<IndexBuffer> indexBuffer = IndexBuffer::create(indices, circleData->maxIndices);
		circleData->circleVertexArray->setIndexBuffer(indexBuffer);

		delete[] indices;

		circleShader = Shader::create((Filesystem::getDataFolderPath() / "shaders/circleShader.glsl").string());
	}

	void Renderer::loadLineShader(){
		lineData = createScope<LineData>(1000);

		lineData->lineVertexArray = VertexArray::create();
		lineData->lineVertexBuffer = VertexBuffer::create(sizeof(LineData) * lineData->maxVertices);

		BufferLayout lineBufferLayout = {
			{DataType::Float3, "position"},
			{DataType::Float4, "color"},
			{DataType::UInt, "entityIndex"}
		};

		lineData->lineVertexBuffer->setLayout(lineBufferLayout);
		lineData->lineVertexArray->addVertexBuffer(lineData->lineVertexBuffer);

		lineData->lineVertexBufferBase = new LineVertex[lineData->maxVertices];
		lineShader = Shader::create((Filesystem::getDataFolderPath() / "shaders/lineShader.glsl").string());
	}

	void Renderer::shutdown(){
		ENGINE_PROFILE_FUNCTION();
		rendererAPI = nullptr;
	}

	void Renderer::OnWindowResized(uint32_t width, uint32_t height){
		rendererAPI->setViewport(0, 0, width, height);
	}

	void Renderer::beginScene(const Camera &camera){
		ENGINE_PROFILE_FUNCTION();
		ENGINE_CORE_ASSERT(quadShader, "cannot begin the scene with a null quadShader");

		quadShader->bind();
		quadShader->setMat4(camera.getViewProjection(), "u_viewProjectionMatrix");
		
		circleShader->bind();
		circleShader->setMat4(camera.getViewProjection(), "u_viewProjectionMatrix");
		
		lineShader->bind();
		lineShader->setMat4(camera.getViewProjection(), "u_viewProjectionMatrix");

		reloadScene();
	}

	void Renderer::endScene(){
		ENGINE_PROFILE_FUNCTION();
		flush();
		reloadScene();
	}

	void Renderer::reloadScene(){
		ENGINE_PROFILE_FUNCTION();
		
		quadData->quadVertexBufferPtr = quadData->quadVertexBufferBase;
		quadData->quadIndexCount = 0;

		circleData->circleVertexBufferPtr = circleData->circleVertexBufferBase;
		circleData->circleIndexCount = 0;

		lineData->lineVertexBufferPtr = lineData->lineVertexBufferBase;
		lineData->lineCount = 0;

		quadData->textureSlotIndex = 1;
	}


	void Renderer::flush(){
		ENGINE_PROFILE_FUNCTION();

		if (quadData->quadIndexCount > 0){
			uint32_t size = reinterpret_cast<uint8_t*>(quadData->quadVertexBufferPtr) - reinterpret_cast<uint8_t*>(quadData->quadVertexBufferBase);
			quadData->quadVertexBuffer->setData(quadData->quadVertexBufferBase, size);

			for (int i=static_cast<int>(0); i<quadData->textureSlotIndex; i++){
				quadData->textureSlots[i]->bind(i);
			}

			quadShader->bind();
			rendererAPI->drawIndexed(quadData->quadVertexArray, quadData->quadIndexCount);
		}

		if (circleData->circleIndexCount > 0){
			uint32_t size = reinterpret_cast<uint8_t*>(circleData->circleVertexBufferPtr) - reinterpret_cast<uint8_t*>(circleData->circleVertexBufferBase);
			circleData->circleVertexBuffer->setData(circleData->circleVertexBufferBase, size);

			circleShader->bind();
			rendererAPI->drawIndexed(circleData->circleVertexArray, circleData->circleIndexCount);
		}

		if (lineData->lineCount > 0){
			uint32_t size = reinterpret_cast<uint8_t*>(lineData->lineVertexBufferPtr) - reinterpret_cast<uint8_t*>(lineData->lineVertexBufferBase);
			lineData->lineVertexBuffer->setData(lineData->lineVertexBufferBase, size);

			lineShader->bind();
			rendererAPI->drawLines(lineData->lineVertexArray, lineData->lineCount);
		}
	}

	static void initializeCommand(RenderCommand &command, RenderCommand::DataSpecs data, RenderCommand::Type type, RenderCommand::ColorSpec color){
		command.specs = data;
		command.type = type;
		command.colorSpecs = color;
	}
	
	void Renderer::drawQuad(const glm::mat4 &mat, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSFORM, RenderCommand::TYPE_QUAD, RenderCommand::COLOR_WHITE);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushMat4(mat);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawQuad(const glm::mat4 &mat, const glm::vec3 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSFORM, RenderCommand::TYPE_QUAD, RenderCommand::COLOR_RGB);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushMat4(mat);
		commandBufferPushVec3(color);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawQuad(const glm::mat4 &mat, const glm::vec4 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSFORM, RenderCommand::TYPE_QUAD, RenderCommand::COLOR_RGBA);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushMat4(mat);
		commandBufferPushVec4(color);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawQuad(const glm::vec2 &translation, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE, RenderCommand::TYPE_QUAD, RenderCommand::COLOR_WHITE);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawQuad(const glm::vec2 &translation, const glm::vec3 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE, RenderCommand::TYPE_QUAD, RenderCommand::COLOR_RGB);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushVec3(color);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawQuad(const glm::vec2 &translation, const glm::vec4 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE, RenderCommand::TYPE_QUAD, RenderCommand::COLOR_RGBA);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushVec4(color);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawQuad(const glm::vec2 &translation, float scale, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_SCALE1, RenderCommand::TYPE_QUAD, RenderCommand::COLOR_WHITE);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushFloat(scale);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawQuad(const glm::vec2 &translation, float scale, const glm::vec3 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_SCALE1, RenderCommand::TYPE_QUAD, RenderCommand::COLOR_RGB);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushFloat(scale);
		commandBufferPushVec3(color);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawQuad(const glm::vec2 &translation, float scale, const glm::vec4 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_SCALE1, RenderCommand::TYPE_QUAD, RenderCommand::COLOR_RGBA);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushFloat(scale);
		commandBufferPushVec4(color);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawQuad(const glm::vec2 &translation, glm::vec2 &scale, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_SCALE2, RenderCommand::TYPE_QUAD, RenderCommand::COLOR_WHITE);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushVec2(scale);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawQuad(const glm::vec2 &translation, glm::vec2 &scale, const glm::vec3 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_SCALE2, RenderCommand::TYPE_QUAD, RenderCommand::COLOR_RGB);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushVec2(scale);
		commandBufferPushVec3(color);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawQuad(const glm::vec2 &translation, glm::vec2 &scale, const glm::vec4 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_SCALE2, RenderCommand::TYPE_QUAD, RenderCommand::COLOR_RGBA);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushVec2(scale);
		commandBufferPushVec4(color);
		getCurrentRenderQueue().push_back(command);
	}
	
	void Renderer::drawRotatedQuad(const glm::vec2 &translation, float rotation, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_ROTATE, RenderCommand::TYPE_QUAD, RenderCommand::COLOR_WHITE);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushFloat(rotation);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawRotatedQuad(const glm::vec2 &translation, float rotation, const glm::vec3 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_ROTATE, RenderCommand::TYPE_QUAD, RenderCommand::COLOR_RGB);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushFloat(rotation);
		commandBufferPushVec3(color);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawRotatedQuad(const glm::vec2 &translation, float rotation, const glm::vec4 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_ROTATE, RenderCommand::TYPE_QUAD, RenderCommand::COLOR_RGBA);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushFloat(rotation);
		commandBufferPushVec4(color);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawRotatedQuad(const glm::vec2 &translation, float rotation, float scale, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_SCALE1_ROTATE, RenderCommand::TYPE_QUAD, RenderCommand::COLOR_WHITE);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushFloat(scale);
		commandBufferPushFloat(rotation);
		getCurrentRenderQueue().push_back(command);

	}

	void Renderer::drawRotatedQuad(const glm::vec2 &translation, float rotation, float scale, const glm::vec3 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_SCALE1_ROTATE, RenderCommand::TYPE_QUAD, RenderCommand::COLOR_RGB);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushFloat(scale);
		commandBufferPushFloat(rotation);
		commandBufferPushVec3(color);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawRotatedQuad(const glm::vec2 &translation, float rotation, float scale, const glm::vec4 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_SCALE1_ROTATE, RenderCommand::TYPE_QUAD, RenderCommand::COLOR_RGBA);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushFloat(scale);
		commandBufferPushFloat(rotation);
		commandBufferPushVec4(color);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawRotatedQuad(const glm::vec2 &translation, float rotation, glm::vec2 &scale, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_SCALE2_ROTATE, RenderCommand::TYPE_QUAD, RenderCommand::COLOR_WHITE);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushVec2(scale);
		commandBufferPushFloat(rotation);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawRotatedQuad(const glm::vec2 &translation, float rotation, glm::vec2 &scale, const glm::vec3 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_SCALE2_ROTATE, RenderCommand::TYPE_QUAD, RenderCommand::COLOR_RGB);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushVec2(scale);
		commandBufferPushFloat(rotation);
		commandBufferPushVec3(color);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawRotatedQuad(const glm::vec2 &translation, float rotation, glm::vec2 &scale, const glm::vec4 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_SCALE2_ROTATE, RenderCommand::TYPE_QUAD, RenderCommand::COLOR_RGBA);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushVec2(scale);
		commandBufferPushFloat(rotation);
		commandBufferPushVec4(color);
		getCurrentRenderQueue().push_back(command);
	}

	
	void Renderer::drawTexturedQuad(const Ref<SubTexture2D> &subTexture, const glm::mat4 &mat, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSFORM, RenderCommand::TYPE_TEXTURED_QUAD, RenderCommand::COLOR_WHITE);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushMat4(mat);
		commandBufferPushSubTexture(subTexture);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawTexturedQuad(const Ref<SubTexture2D> &subTexture, const glm::mat4 &mat, const glm::vec3 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSFORM, RenderCommand::TYPE_TEXTURED_QUAD, RenderCommand::COLOR_RGB);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushMat4(mat);
		commandBufferPushVec3(color);
		commandBufferPushSubTexture(subTexture);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawTexturedQuad(const Ref<SubTexture2D> &subTexture, const glm::mat4 &mat, const glm::vec4 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSFORM, RenderCommand::TYPE_TEXTURED_QUAD, RenderCommand::COLOR_RGBA);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushMat4(mat);
		commandBufferPushVec4(color);
		commandBufferPushSubTexture(subTexture);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawTexturedQuad(const Ref<SubTexture2D> &subTexture, const glm::vec2 &translation, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE, RenderCommand::TYPE_TEXTURED_QUAD, RenderCommand::COLOR_WHITE);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushSubTexture(subTexture);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawTexturedQuad(const Ref<SubTexture2D> &subTexture, const glm::vec2 &translation, const glm::vec3 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE, RenderCommand::TYPE_TEXTURED_QUAD, RenderCommand::COLOR_RGB);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushVec3(color);
		commandBufferPushSubTexture(subTexture);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawTexturedQuad(const Ref<SubTexture2D> &subTexture, const glm::vec2 &translation, const glm::vec4 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE, RenderCommand::TYPE_TEXTURED_QUAD, RenderCommand::COLOR_RGBA);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushVec4(color);
		commandBufferPushSubTexture(subTexture);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawTexturedQuad(const Ref<SubTexture2D> &subTexture, const glm::vec2 &translation, float scale, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_SCALE1, RenderCommand::TYPE_TEXTURED_QUAD, RenderCommand::COLOR_WHITE);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushFloat(scale);
		commandBufferPushSubTexture(subTexture);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawTexturedQuad(const Ref<SubTexture2D> &subTexture, const glm::vec2 &translation, float scale, const glm::vec3 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_SCALE1, RenderCommand::TYPE_TEXTURED_QUAD, RenderCommand::COLOR_RGB);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushFloat(scale);
		commandBufferPushVec3(color);
		commandBufferPushSubTexture(subTexture);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawTexturedQuad(const Ref<SubTexture2D> &subTexture, const glm::vec2 &translation, float scale, const glm::vec4 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_SCALE1, RenderCommand::TYPE_TEXTURED_QUAD, RenderCommand::COLOR_RGBA);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushFloat(scale);
		commandBufferPushVec4(color);
		commandBufferPushSubTexture(subTexture);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawTexturedQuad(const Ref<SubTexture2D> &subTexture, const glm::vec2 &translation, glm::vec2 &scale, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_SCALE2, RenderCommand::TYPE_TEXTURED_QUAD, RenderCommand::COLOR_WHITE);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushVec2(scale);
		commandBufferPushSubTexture(subTexture);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawTexturedQuad(const Ref<SubTexture2D> &subTexture, const glm::vec2 &translation, glm::vec2 &scale, const glm::vec3 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_SCALE2, RenderCommand::TYPE_TEXTURED_QUAD, RenderCommand::COLOR_RGB);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushVec2(scale);
		commandBufferPushVec3(color);
		commandBufferPushSubTexture(subTexture);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawTexturedQuad(const Ref<SubTexture2D> &subTexture, const glm::vec2 &translation, glm::vec2 &scale, const glm::vec4 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_SCALE2, RenderCommand::TYPE_TEXTURED_QUAD, RenderCommand::COLOR_RGBA);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushVec2(scale);
		commandBufferPushVec4(color);
		commandBufferPushSubTexture(subTexture);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawTexturedRotatedQuad(const Ref<SubTexture2D> &subTexture, const glm::vec2 &translation, float rotation, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_ROTATE, RenderCommand::TYPE_TEXTURED_QUAD, RenderCommand::COLOR_WHITE);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushFloat(rotation);
		commandBufferPushSubTexture(subTexture);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawTexturedRotatedQuad(const Ref<SubTexture2D> &subTexture, const glm::vec2 &translation, float rotation, const glm::vec3 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_ROTATE, RenderCommand::TYPE_TEXTURED_QUAD, RenderCommand::COLOR_RGB);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushFloat(rotation);
		commandBufferPushVec3(color);
		commandBufferPushSubTexture(subTexture);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawTexturedRotatedQuad(const Ref<SubTexture2D> &subTexture, const glm::vec2 &translation, float rotation, const glm::vec4 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_ROTATE, RenderCommand::TYPE_TEXTURED_QUAD, RenderCommand::COLOR_RGBA);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushFloat(rotation);
		commandBufferPushVec4(color);
		commandBufferPushSubTexture(subTexture);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawTexturedRotatedQuad(const Ref<SubTexture2D> &subTexture, const glm::vec2 &translation, float rotation, float scale, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_SCALE1_ROTATE, RenderCommand::TYPE_TEXTURED_QUAD, RenderCommand::COLOR_WHITE);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushFloat(scale);
		commandBufferPushFloat(rotation);
		commandBufferPushSubTexture(subTexture);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawTexturedRotatedQuad(const Ref<SubTexture2D> &subTexture, const glm::vec2 &translation, float rotation, float scale, const glm::vec3 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_SCALE1_ROTATE, RenderCommand::TYPE_TEXTURED_QUAD, RenderCommand::COLOR_RGB);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushFloat(scale);
		commandBufferPushFloat(rotation);
		commandBufferPushVec3(color);
		commandBufferPushSubTexture(subTexture);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawTexturedRotatedQuad(const Ref<SubTexture2D> &subTexture, const glm::vec2 &translation, float rotation, float scale, const glm::vec4 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_SCALE1_ROTATE, RenderCommand::TYPE_TEXTURED_QUAD, RenderCommand::COLOR_RGBA);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushFloat(scale);
		commandBufferPushFloat(rotation);
		commandBufferPushVec4(color);
		commandBufferPushSubTexture(subTexture);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawTexturedRotatedQuad(const Ref<SubTexture2D> &subTexture, const glm::vec2 &translation, float rotation, glm::vec2 &scale, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_SCALE2_ROTATE, RenderCommand::TYPE_TEXTURED_QUAD, RenderCommand::COLOR_WHITE);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushVec2(scale);
		commandBufferPushFloat(rotation);
		commandBufferPushSubTexture(subTexture);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawTexturedRotatedQuad(const Ref<SubTexture2D> &subTexture, const glm::vec2 &translation, float rotation, glm::vec2 &scale, const glm::vec3 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_SCALE2_ROTATE, RenderCommand::TYPE_TEXTURED_QUAD, RenderCommand::COLOR_RGB);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushVec2(scale);
		commandBufferPushFloat(rotation);
		commandBufferPushVec3(color);
		commandBufferPushSubTexture(subTexture);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::drawTexturedRotatedQuad(const Ref<SubTexture2D> &subTexture, const glm::vec2 &translation, float rotation, glm::vec2 &scale, const glm::vec4 &color, uint32_t entityIndex){
		RenderCommand command;
		initializeCommand(command, RenderCommand::DATA_TRANSLATE_SCALE2_ROTATE, RenderCommand::TYPE_TEXTURED_QUAD, RenderCommand::COLOR_RGBA);
		command.data = commandBufferPushUint32_t(entityIndex);
		commandBufferPushVec2(translation);
		commandBufferPushVec2(scale);
		commandBufferPushFloat(rotation);
		commandBufferPushVec4(color);
		commandBufferPushSubTexture(subTexture);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::clear(){
		RenderCommand command;
		command.type = RenderCommand::TYPE_CLEAR;
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::setClearColor(const glm::vec3 &color){
		RenderCommand command;
		command.type = RenderCommand::TYPE_CLEAR;
		command.data = commandBufferPushVec3(color);
		getCurrentRenderQueue().push_back(command);
	}

	void Renderer::swap(){
		currentRenderQueue = (currentRenderQueue+1)%2;
		getCurrentRenderQueue().clear();
		getCurrentRenderBuffer().clear();
	}

	void Renderer::draw(){
		batchDrawQueue();

		for (auto &command : batchRenderCommand){
			if (!command || command->type == RenderCommand::TYPE_NONE) continue;

			bool reload = false;
			switch (command->type){
				case RenderCommand::TYPE_CLEAR: commandClear(command); break;
				case RenderCommand::TYPE_SET_CLEAR_COLOR: commandSetClearColor(command); break;
				case RenderCommand::TYPE_QUAD: commandDrawQuad(command); reload = true; break;
				case RenderCommand::TYPE_TEXTURED_QUAD: reload = true; break;
			}

			if (reload){
				endScene();
				reloadScene();
			}
		}
	}

	void Renderer::batchDrawQueue(){
		RenderCommand *lastCommand = nullptr;
		std::deque<RenderCommand>& deque = renderCommands[(currentRenderQueue+1)%2];
		for (auto &command : deque){
			if (lastCommand){
				if (lastCommand->type == command.type){
					lastCommand->count++;
					continue;
				}
			}

			lastCommand = &command;
			batchRenderCommand.push_back(lastCommand);
		}
	}

	void Renderer::commandClear(RenderCommand* command){
		ENGINE_ASSERT(command != nullptr, "cannot execute a null render command");
		rendererAPI->clear();
	}

	void Renderer::commandSetClearColor(RenderCommand* command){
		ENGINE_ASSERT(command != nullptr, "cannot execute a null render command");

		float* data = command->data;
		float r = data[0];
		float g = data[1];
		float b = data[2];

		rendererAPI->setClearColor({r, g, b, 1});
	}

	void Renderer::commandDrawQuad(RenderCommand* command){
		ENGINE_ASSERT(command != nullptr, "cannot execute a null render command");


		for (int i=0; i<command->count; i++){
			switch (command->specs){
				case RenderCommand::DATA_TRANSFORM: commandDrawQuadTransformation(command); break;
				case RenderCommand::DATA_TRANSLATE: commandDrawQuadTranslate(command); break;
				case RenderCommand::DATA_TRANSLATE_ROTATE: commandDrawQuadTranslateRotate(command); break;
				case RenderCommand::DATA_TRANSLATE_SCALE1: commandDrawQuadTranslateScale1(command); break;
				case RenderCommand::DATA_TRANSLATE_SCALE2: commandDrawQuadTranslateScale2(command); break;
				case RenderCommand::DATA_TRANSLATE_SCALE1_ROTATE: commandDrawQuadTranslateScale1Rotate(command); break;
				case RenderCommand::DATA_TRANSLATE_SCALE2_ROTATE: commandDrawQuadTranslateScale2Rotate(command); break;
			}
		}
	}

	#define GET_COLOR() switch(command->colorSpecs){case RenderCommand::COLOR_WHITE:{color={1.0f,1.0f,1.0f,1.0f};break;}case RenderCommand::COLOR_RGB:{color.r=data[i++];color.g=data[i++];color.b=data[i++];color.a=1.f;break;}case RenderCommand::COLOR_RGBA:{color.r=data[i++];color.g=data[i++];color.b=data[i++];color.a=data[i++];}}

	#define GET_SUBTEXTURE() uv1.x=data[i++];uv1.y=data[i++];uv2.x=data[i++];uv2.y=data[i++];

	void Renderer::commandDrawQuadTransformation(RenderCommand* command){
		ENGINE_ASSERT(command != nullptr, "cannot execute a null render command");
		float* data = command->data;
		uint8_t i=0;

		float entityIndex = data[i++];
		glm::mat4 mat;
		for (int x=0; x<4; x++){
			for (int y=0; y<4; y++){
				mat[x][y] = data[i++];
			}
		}

		glm::vec4 color;
		GET_COLOR();
		

		drawQuad(mat, color, 1, 0, {0, 0}, {1, 1}, static_cast<uint32_t>(entityIndex));
	}

	void Renderer::commandDrawQuadTranslate(RenderCommand* command){
		ENGINE_ASSERT(command != nullptr, "cannot execute a null render command");
		float* data = command->data;
		uint8_t i=0;

		float entityIndex = data[i++];
		glm::vec2 translation;
		translation.x = data[i++];
		translation.y = data[i++];

		glm::vec4 color;
		GET_COLOR();

		glm::mat4 mat = glm::translate(glm::mat4(1.f), glm::vec3(translation, 0.f));
		drawQuad(mat, color, 1, 0, {0, 0}, {1, 1}, static_cast<uint32_t>(entityIndex));
	}

	void Renderer::commandDrawQuadTranslateRotate(RenderCommand* command){
		ENGINE_ASSERT(command != nullptr, "cannot execute a null render command");
		float* data = command->data;
		uint8_t i=0;

		float entityIndex = data[i++];
		glm::vec2 translation;
		translation.x = data[i++];
		translation.y = data[i++];

		float rotation = data[i++];

		glm::vec4 color;
		GET_COLOR();

		glm::mat4 mat = glm::translate(glm::mat4(1.f), glm::vec3(translation, 0.f)) * glm::rotate(glm::mat4(1.f), rotation, {0.f, 0.f, 1.f});
		drawQuad(mat, color, 1, 0, {0, 0}, {1, 1}, static_cast<uint32_t>(entityIndex));

	}

	void Renderer::commandDrawQuadTranslateScale1(RenderCommand* command){
		ENGINE_ASSERT(command != nullptr, "cannot execute a null render command");
		float* data = command->data;
		uint8_t i=0;

		float entityIndex = data[i++];
		glm::vec2 translation;
		translation.x = data[i++];
		translation.y = data[i++];

		float scale = data[i++];

		glm::vec4 color;
		GET_COLOR();

		glm::mat4 mat = glm::translate(glm::mat4(1.f), glm::vec3(translation, 0.f)) * glm::scale(glm::mat4(1.f), {scale, scale, 1.f});
		drawQuad(mat, color, 1, 0, {0, 0}, {1, 1}, static_cast<uint32_t>(entityIndex));
	}
	
	void Renderer::commandDrawQuadTranslateScale2(RenderCommand* command){
		ENGINE_ASSERT(command != nullptr, "cannot execute a null render command");
		float* data = command->data;
		uint8_t i=0;

		float entityIndex = data[i++];
		glm::vec2 translation;
		translation.x = data[i++];
		translation.y = data[i++];

		glm::vec2 scale;
		scale.x = data[i++];
		scale.x = data[i++];

		glm::vec4 color;
		GET_COLOR();

		glm::mat4 mat = glm::translate(glm::mat4(1.f), glm::vec3(translation, 0.f)) * glm::scale(glm::mat4(1.f), {scale.x, scale.y, 1.f});
		drawQuad(mat, color, 1, 0, {0, 0}, {1, 1}, static_cast<uint32_t>(entityIndex));

	}

	void Renderer::commandDrawQuadTranslateScale1Rotate(RenderCommand* command){
		ENGINE_ASSERT(command != nullptr, "cannot execute a null render command");
		float* data = command->data;
		uint8_t i=0;

		float entityIndex = data[i++];
		glm::vec2 translation;
		translation.x = data[i++];
		translation.y = data[i++];

		float scale = data[i++];
		float rotation = data[i++];

		glm::vec4 color;
		GET_COLOR();

		glm::mat4 mat = glm::translate(glm::mat4(1.f), glm::vec3(translation, 0.f)) * glm::rotate(glm::mat4(1.f), rotation, {0.f, 0.f, 1.f}) * glm::scale(glm::mat4(1.f), {scale, scale, 1.f});
		drawQuad(mat, color, 1, 0, {0, 0}, {1, 1}, static_cast<uint32_t>(entityIndex));

	}

	void Renderer::commandDrawQuadTranslateScale2Rotate(RenderCommand* command){
		ENGINE_ASSERT(command != nullptr, "cannot execute a null render command");
		float* data = command->data;
		uint8_t i=0;

		float entityIndex = data[i++];
		glm::vec2 translation;
		translation.x = data[i++];
		translation.y = data[i++];

		glm::vec2 scale;
		scale.x = data[i++];
		scale.y = data[i++];
		float rotation = data[i++];

		glm::vec4 color;
		GET_COLOR();

		glm::mat4 mat = glm::translate(glm::mat4(1.f), glm::vec3(translation, 0.f)) * glm::rotate(glm::mat4(1.f), rotation, {0.f, 0.f, 1.f}) * glm::scale(glm::mat4(1.f), {scale.x, scale.y, 1.f});
		drawQuad(mat, color, 1, 0, {0, 0}, {1, 1}, static_cast<uint32_t>(entityIndex));
	}
	
	void Renderer::commandTexturedDrawQuad(RenderCommand* command){
		ENGINE_ASSERT(command != nullptr, "cannot execute a null render command");


		for (int i=0; i<command->count; i++){
			switch (command->specs){
				case RenderCommand::DATA_TRANSFORM: commandTexturedDrawQuadTransformation(command); break;
				case RenderCommand::DATA_TRANSLATE: commandTexturedDrawQuadTranslate(command); break;
				case RenderCommand::DATA_TRANSLATE_ROTATE: commandTexturedDrawQuadTranslateRotate(command); break;
				case RenderCommand::DATA_TRANSLATE_SCALE1: commandTexturedDrawQuadTranslateScale1(command); break;
				case RenderCommand::DATA_TRANSLATE_SCALE2: commandTexturedDrawQuadTranslateScale2(command); break;
				case RenderCommand::DATA_TRANSLATE_SCALE1_ROTATE: commandTexturedDrawQuadTranslateScale1Rotate(command); break;
				case RenderCommand::DATA_TRANSLATE_SCALE2_ROTATE: commandTexturedDrawQuadTranslateScale2Rotate(command); break;
			}
		}
	}

	void Renderer::commandTexturedDrawQuadTransformation(RenderCommand* command){
		ENGINE_ASSERT(command != nullptr, "cannot execute a null render command");
		float* data = command->data;
		uint8_t i=0;

		float entityIndex = data[i++];
		glm::mat4 mat;
		for (int x=0; x<4; x++){
			for (int y=0; y<4; y++){
				mat[x][y] = data[i++];
			}
		}

		glm::vec4 color;
		GET_COLOR();
		
		float textureIndex = data[i++];
		glm::vec2 uv1, uv2;
		GET_SUBTEXTURE();

		drawQuad(mat, color, 1, static_cast<uint32_t>(textureIndex), uv1, uv2, static_cast<uint32_t>(entityIndex));
	}

	void Renderer::commandTexturedDrawQuadTranslate(RenderCommand* command){
		ENGINE_ASSERT(command != nullptr, "cannot execute a null render command");
		float* data = command->data;
		uint8_t i=0;

		float entityIndex = data[i++];
		glm::vec2 translation;
		translation.x = data[i++];
		translation.y = data[i++];

		glm::vec4 color;
		GET_COLOR();

		float textureIndex = data[i++];
		glm::vec2 uv1, uv2;
		GET_SUBTEXTURE();

		glm::mat4 mat = glm::translate(glm::mat4(1.f), glm::vec3(translation, 0.f));
		drawQuad(mat, color, 1, static_cast<uint32_t>(textureIndex), uv1, uv2, static_cast<uint32_t>(entityIndex));
	}

	void Renderer::commandTexturedDrawQuadTranslateRotate(RenderCommand* command){
		ENGINE_ASSERT(command != nullptr, "cannot execute a null render command");
		float* data = command->data;
		uint8_t i=0;

		float entityIndex = data[i++];
		glm::vec2 translation;
		translation.x = data[i++];
		translation.y = data[i++];
		
		float rotation = data[i++];

		glm::vec4 color;
		GET_COLOR();

		float textureIndex = data[i++];
		glm::vec2 uv1, uv2;
		GET_SUBTEXTURE();

		glm::mat4 mat = glm::translate(glm::mat4(1.f), glm::vec3(translation, 0.f)) * glm::rotate(glm::mat4(1.f), rotation, {0.f, 0.f, 1.f});
		drawQuad(mat, color, 1, static_cast<uint32_t>(textureIndex), uv1, uv2, static_cast<uint32_t>(entityIndex));
	}

	void Renderer::commandTexturedDrawQuadTranslateScale1(RenderCommand* command){
		ENGINE_ASSERT(command != nullptr, "cannot execute a null render command");
		float* data = command->data;
		uint8_t i=0;

		float entityIndex = data[i++];
		glm::vec2 translation;
		translation.x = data[i++];
		translation.y = data[i++];
		
		float scale = data[i++];

		glm::vec4 color;
		GET_COLOR();

		float textureIndex = data[i++];
		glm::vec2 uv1, uv2;
		GET_SUBTEXTURE();

		glm::mat4 mat = glm::translate(glm::mat4(1.f), glm::vec3(translation, 0.f)) * glm::scale(glm::mat4(1.f), {scale, scale, 1.f});
		drawQuad(mat, color, 1, static_cast<uint32_t>(textureIndex), uv1, uv2, static_cast<uint32_t>(entityIndex));
	}

	void Renderer::commandTexturedDrawQuadTranslateScale2(RenderCommand* command){
		ENGINE_ASSERT(command != nullptr, "cannot execute a null render command");
		float* data = command->data;
		uint8_t i=0;

		float entityIndex = data[i++];
		glm::vec2 translation;
		translation.x = data[i++];
		translation.y = data[i++];
		
		glm::vec2 scale;
		scale.x = data[i++];
		scale.y = data[i++];

		glm::vec4 color;
		GET_COLOR();

		float textureIndex = data[i++];
		glm::vec2 uv1, uv2;
		GET_SUBTEXTURE();

		glm::mat4 mat = glm::translate(glm::mat4(1.f), glm::vec3(translation, 0.f)) * glm::scale(glm::mat4(1.f), glm::vec3(scale, 1.f));
		drawQuad(mat, color, 1, static_cast<uint32_t>(textureIndex), uv1, uv2, static_cast<uint32_t>(entityIndex));
	}

	void Renderer::commandTexturedDrawQuadTranslateScale1Rotate(RenderCommand* command){
		ENGINE_ASSERT(command != nullptr, "cannot execute a null render command");
		float* data = command->data;
		uint8_t i=0;

		float entityIndex = data[i++];
		glm::vec2 translation;
		translation.x = data[i++];
		translation.y = data[i++];

		float scale = data[i++];
		float rotation = data[i++];

		glm::vec4 color;
		GET_COLOR();
		
		float textureIndex = data[i++];
		glm::vec2 uv1, uv2;
		GET_SUBTEXTURE();

		glm::mat4 mat = glm::translate(glm::mat4(1.f), glm::vec3(translation, 0.f)) * glm::rotate(glm::mat4(1.f), rotation, {0.f, 0.f, 1.f}) * glm::scale(glm::mat4(1.f), {scale, scale, 1.f});
		drawQuad(mat, color, 1, static_cast<uint32_t>(textureIndex), uv1, uv2, static_cast<uint32_t>(entityIndex));
	}

	void Renderer::commandTexturedDrawQuadTranslateScale2Rotate(RenderCommand* command){
		ENGINE_ASSERT(command != nullptr, "cannot execute a null render command");
		float* data = command->data;
		uint8_t i=0;

		float entityIndex = data[i++];
		glm::vec2 translation;
		translation.x = data[i++];
		translation.y = data[i++];

		glm::vec2 scale;
		scale.x = data[i++];
		scale.y = data[i++];
		float rotation = data[i++];

		glm::vec4 color;
		GET_COLOR();
		
		float textureIndex = data[i++];
		glm::vec2 uv1, uv2;
		GET_SUBTEXTURE();

		glm::mat4 mat = glm::translate(glm::mat4(1.f), glm::vec3(translation, 0.f)) * glm::rotate(glm::mat4(1.f), rotation, {0.f, 0.f, 1.f}) * glm::scale(glm::mat4(1.f), {scale.x, scale.y, 1.f});
		drawQuad(mat, color, 1, static_cast<uint32_t>(textureIndex), uv1, uv2, static_cast<uint32_t>(entityIndex));

	}
	
	void Renderer::drawQuad(const glm::mat4 &transform, const glm::vec4& color, float tilingFactor, uint32_t textureIndex, const glm::vec2 &TextureCoordsStart, const glm::vec2 &TextureCoordsEnd, uint32_t entityIndex){
		if (quadData->quadIndexCount + 6 > quadData->maxIndices){
			endScene();
			reloadScene();
		}

		glm::vec2 textureCoords[] = {
			{TextureCoordsStart.x, TextureCoordsEnd.y},
			TextureCoordsEnd,
			{TextureCoordsEnd.x, TextureCoordsStart.y},
			TextureCoordsStart
		};

		for (uint8_t i=0; i<4; i++){
			quadData->quadVertexBufferPtr->position = transform * quadVertexPositions[i];
			quadData->quadVertexBufferPtr->color = color;
			quadData->quadVertexBufferPtr->TextCoord = textureCoords[i];
			quadData->quadVertexBufferPtr->textureIndex = textureIndex;
			quadData->quadVertexBufferPtr->tilingFactor = tilingFactor;
			quadData->quadVertexBufferPtr->entityIndex = entityIndex;
			quadData->quadVertexBufferPtr++;
		}

		quadData->quadIndexCount += 6;
	}

	std::deque<RenderCommand>& Renderer::getCurrentRenderQueue(){
		return renderCommands[currentRenderQueue];
	}

	std::vector<float>& Renderer::getCurrentRenderBuffer(){
		return renderCommandBuffers[currentRenderQueue];
	}

	float* Renderer::commandBufferPushFloat(const float &value){
		std::vector<float>& buffer = getCurrentRenderBuffer();
		buffer.push_back(value);
		return &buffer.back();
	}


	float* Renderer::commandBufferPushVec2(const glm::vec2 &vec){
		std::vector<float>& buffer = getCurrentRenderBuffer();
		buffer.push_back(vec.x);
		float* data = &buffer.back();
		buffer.push_back(vec.y);
		return data;
	}

	float* Renderer::commandBufferPushVec3(const glm::vec3 &vec){
		std::vector<float>& buffer = getCurrentRenderBuffer();
		buffer.push_back(vec.x);
		float* data = &buffer.back();
		buffer.push_back(vec.y);
		buffer.push_back(vec.z);
		return data;
	}

	float* Renderer::commandBufferPushVec4(const glm::vec4 &vec){
		std::vector<float>& buffer = getCurrentRenderBuffer();
		buffer.push_back(vec.x);
		float* data = &buffer.back();
		buffer.push_back(vec.y);
		buffer.push_back(vec.z);
		buffer.push_back(vec.w);
		return data;
	}

	float* Renderer::commandBufferPushMat4(const glm::mat4 &mat){
		std::vector<float>& buffer = getCurrentRenderBuffer();
		float* data = nullptr;
		for (int x=0; x<4; x++){
			for (int y=0; y<4; y++){
				buffer.push_back(mat[x][y]);

				if (x==0 && y==0){
					data = &buffer.back();
				}
			}
		}
		return data;
	}

	float* Renderer::commandBufferPushUint32_t(const uint32_t &value){
		std::vector<float>& buffer = getCurrentRenderBuffer();
		buffer.push_back(static_cast<float>(value));
		return &buffer.back();
	}

	float* Renderer::commandBufferPushSubTexture(const Ref<SubTexture2D> &texture){
		std::vector<float>& buffer = getCurrentRenderBuffer();
		buffer.push_back(static_cast<float>(pushTexture(texture->getTexture())));
		float* data = &buffer.back();
		commandBufferPushVec2(texture->getStart());
		commandBufferPushVec2(texture->getEnd());
		return data;
	}

	uint16_t Renderer::texturePushed(const Ref<Texture2D> &texture){
		for (size_t i=0; i<quadData->textureSlotIndex; i++){
			if (quadData->textureSlots[i] == texture){
				return i;
			}
		}
		return static_cast<uint16_t>(-1);
	}

	uint16_t Renderer::pushTexture(const Ref<Texture2D> &texture){

		if (!texture) return 0;

		uint16_t pushed = texturePushed(texture);

		// if the texture is already pushed, we return the texture index
		if (pushed != static_cast<uint16_t>(-1)) return 0;

		// if the max amount of texture pushed has been reached
		// this part might be uptated for better batching
		// TODO
		if (quadData->textureSlotIndex == quadData->maxTextureSlots){
			endScene();
			reloadScene();
		}

		quadData->textureSlots[quadData->textureSlotIndex] = texture;
		quadData->textureSlotIndex++;
		return quadData->textureSlotIndex-1;
	}
}
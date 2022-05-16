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
		renderCommand.init();

		quadVertexPositions[0] = {-0.5f, -0.5f, 0.f, 1.f};
		quadVertexPositions[1] = { 0.5f, -0.5f, 0.f, 1.f};
		quadVertexPositions[2] = { 0.5f,  0.5f, 0.f, 1.f};
		quadVertexPositions[3] = {-0.5f,  0.5f, 0.f, 1.f};

		loadShaders();
		reloadScene();
	}

	void Renderer::loadShaders(){
		loadCircleShader();
		loadQuadShader();
		loadLineShader();
	}

	void Renderer::loadQuadShader(){
		quadData = createScope<QuadData>();

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
		circleData = createScope<CircleData>();

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
		lineData = createScope<LineData>(10000);

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
		renderCommand.shutdown();
	}

	void Renderer::OnWindowResized(uint32_t width, uint32_t height){
		renderCommand.setViewport(0, 0, width, height);
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
			renderCommand.drawIndexed(quadData->quadVertexArray, quadData->quadIndexCount);
		}

		if (circleData->circleIndexCount > 0){
			uint32_t size = reinterpret_cast<uint8_t*>(circleData->circleVertexBufferPtr) - reinterpret_cast<uint8_t*>(circleData->circleVertexBufferBase);
			circleData->circleVertexBuffer->setData(circleData->circleVertexBufferBase, size);

			circleShader->bind();
			renderCommand.drawIndexed(circleData->circleVertexArray, circleData->circleIndexCount);
		}

		if (lineData->lineCount > 0){
			uint32_t size = reinterpret_cast<uint8_t*>(lineData->lineVertexBufferPtr) - reinterpret_cast<uint8_t*>(lineData->lineVertexBufferBase);
			lineData->lineVertexBuffer->setData(lineData->lineVertexBufferBase, size);

			lineShader->bind();
			renderCommand.drawLines(lineData->lineVertexArray, lineData->lineCount);
		}
	}

	void Renderer::drawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4& color, float tilingFactor, uint32_t textureIndex, const glm::vec2 &TextureCoordsStart, const glm::vec2 &TextureCoordsEnd, uint32_t entityIndex){
		ENGINE_PROFILE_FUNCTION();

		// add one to the entity index, so the index 0 is never used and count a no entity, instead of using a non unsigned int value
		entityIndex++;

		if (quadData->quadIndexCount + 6 > quadData->maxIndices){
			endScene();
			reloadScene();
		}

		glm::mat4 transform = glm::scale(glm::translate(glm::mat4(1.f), position), {size.x, size.y, 1.f});

		quadData->quadVertexBufferPtr->position = transform * quadVertexPositions[0];
		quadData->quadVertexBufferPtr->color = color;
		quadData->quadVertexBufferPtr->TextCoord = {TextureCoordsStart.x, TextureCoordsEnd.y};
		quadData->quadVertexBufferPtr->textureIndex = textureIndex;
		quadData->quadVertexBufferPtr->tilingFactor = tilingFactor;
		quadData->quadVertexBufferPtr->entityIndex = entityIndex;
		quadData->quadVertexBufferPtr++;

		quadData->quadVertexBufferPtr->position = transform * quadVertexPositions[1];
		quadData->quadVertexBufferPtr->color = color;
		quadData->quadVertexBufferPtr->TextCoord = TextureCoordsEnd;
		quadData->quadVertexBufferPtr->textureIndex = textureIndex;
		quadData->quadVertexBufferPtr->tilingFactor = tilingFactor;
		quadData->quadVertexBufferPtr->entityIndex = entityIndex;
		quadData->quadVertexBufferPtr++;

		quadData->quadVertexBufferPtr->position = transform * quadVertexPositions[2];
		quadData->quadVertexBufferPtr->color = color;
		quadData->quadVertexBufferPtr->TextCoord = {TextureCoordsEnd.x, TextureCoordsStart.y};
		quadData->quadVertexBufferPtr->textureIndex = textureIndex;
		quadData->quadVertexBufferPtr->tilingFactor = tilingFactor;
		quadData->quadVertexBufferPtr->entityIndex = entityIndex;
		quadData->quadVertexBufferPtr++;

		quadData->quadVertexBufferPtr->position = transform * quadVertexPositions[3];
		quadData->quadVertexBufferPtr->color = color;
		quadData->quadVertexBufferPtr->TextCoord = TextureCoordsStart;
		quadData->quadVertexBufferPtr->textureIndex = textureIndex;
		quadData->quadVertexBufferPtr->tilingFactor = tilingFactor;
		quadData->quadVertexBufferPtr->entityIndex = entityIndex;
		quadData->quadVertexBufferPtr++;

		quadData->quadIndexCount += 6;
	}

	void Renderer::drawQuad(const glm::mat4 &transform, const glm::vec4 &color, uint32_t entityIndex, const Ref<SubTexture2D> &texture){

		entityIndex++;

		if (quadData->quadIndexCount + 6 > quadData->maxIndices){
			endScene();
			reloadScene();
		}
		
		glm::vec2 TextureCoordsStart = texture ? texture->getStart() : glm::vec2(0.f);
		glm::vec2 TextureCoordsEnd = texture ? texture->getEnd() : glm::vec2(1.f);
		
		uint16_t textureIndex = texture ? pushTexture(texture->getTexture()) : 0;

		quadData->quadVertexBufferPtr->position = transform * quadVertexPositions[0];
		quadData->quadVertexBufferPtr->color = color;
		quadData->quadVertexBufferPtr->TextCoord = {TextureCoordsStart.x, TextureCoordsEnd.y};
		quadData->quadVertexBufferPtr->textureIndex = textureIndex;
		quadData->quadVertexBufferPtr->tilingFactor = 1.f;
		quadData->quadVertexBufferPtr->entityIndex = entityIndex;
		quadData->quadVertexBufferPtr++;

		quadData->quadVertexBufferPtr->position = transform * quadVertexPositions[1];
		quadData->quadVertexBufferPtr->color = color;
		quadData->quadVertexBufferPtr->TextCoord = TextureCoordsEnd;
		quadData->quadVertexBufferPtr->textureIndex = textureIndex;
		quadData->quadVertexBufferPtr->tilingFactor = 1.f;
		quadData->quadVertexBufferPtr->entityIndex = entityIndex;
		quadData->quadVertexBufferPtr++;

		quadData->quadVertexBufferPtr->position = transform * quadVertexPositions[2];
		quadData->quadVertexBufferPtr->color = color;
		quadData->quadVertexBufferPtr->TextCoord = {TextureCoordsEnd.x, TextureCoordsStart.y};
		quadData->quadVertexBufferPtr->textureIndex = textureIndex;
		quadData->quadVertexBufferPtr->tilingFactor = 1.f;
		quadData->quadVertexBufferPtr->entityIndex = entityIndex;
		quadData->quadVertexBufferPtr++;

		quadData->quadVertexBufferPtr->position = transform * quadVertexPositions[3];
		quadData->quadVertexBufferPtr->color = color;
		quadData->quadVertexBufferPtr->TextCoord = TextureCoordsStart;
		quadData->quadVertexBufferPtr->textureIndex = textureIndex;
		quadData->quadVertexBufferPtr->tilingFactor = 1.f;
		quadData->quadVertexBufferPtr->entityIndex = entityIndex;
		quadData->quadVertexBufferPtr++;

		quadData->quadIndexCount += 6;
	}

	void Renderer::drawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4& color, float tilingFactor, uint32_t textureIndex, const glm::vec2 &TextureCoordsStart, const glm::vec2 &TextureCoordsEnd, float angle, uint32_t entityIndex){
		ENGINE_PROFILE_FUNCTION();

		// add one to the entity index, so the index 0 is never used and count a no entity, instead of using a non unsigned int value
		entityIndex++;

		if (quadData->quadIndexCount + 6 > quadData->maxIndices){
			endScene();
			reloadScene();
		}

		glm::mat4 rotation = glm::rotate(glm::mat4(1.f), angle, {0.f, 0.f, 1.f});

		glm::mat4 transform = glm::translate(glm::mat4(1.f), position) * rotation * glm::scale(glm::mat4(1.f), glm::vec3(size, 1.f));

		quadData->quadVertexBufferPtr->position = transform * quadVertexPositions[0];
		quadData->quadVertexBufferPtr->color = color;
		quadData->quadVertexBufferPtr->TextCoord = {TextureCoordsStart.x, TextureCoordsEnd.y};
		quadData->quadVertexBufferPtr->textureIndex = textureIndex;
		quadData->quadVertexBufferPtr->tilingFactor = tilingFactor;
		quadData->quadVertexBufferPtr->entityIndex = entityIndex;
		quadData->quadVertexBufferPtr++;

		quadData->quadVertexBufferPtr->position = transform * quadVertexPositions[1];
		quadData->quadVertexBufferPtr->color = color;
		quadData->quadVertexBufferPtr->TextCoord = TextureCoordsEnd;
		quadData->quadVertexBufferPtr->textureIndex = textureIndex;
		quadData->quadVertexBufferPtr->tilingFactor = tilingFactor;
		quadData->quadVertexBufferPtr->entityIndex = entityIndex;
		quadData->quadVertexBufferPtr++;

		quadData->quadVertexBufferPtr->position = transform * quadVertexPositions[2];
		quadData->quadVertexBufferPtr->color = color;
		quadData->quadVertexBufferPtr->TextCoord = {TextureCoordsEnd.x, TextureCoordsStart.y};
		quadData->quadVertexBufferPtr->textureIndex = textureIndex;
		quadData->quadVertexBufferPtr->tilingFactor = tilingFactor;
		quadData->quadVertexBufferPtr->entityIndex = entityIndex;
		quadData->quadVertexBufferPtr++;

		quadData->quadVertexBufferPtr->position = transform * quadVertexPositions[3];
		quadData->quadVertexBufferPtr->color = color;
		quadData->quadVertexBufferPtr->TextCoord = TextureCoordsStart;
		quadData->quadVertexBufferPtr->textureIndex = textureIndex;
		quadData->quadVertexBufferPtr->tilingFactor = tilingFactor;
		quadData->quadVertexBufferPtr->entityIndex = entityIndex;
		quadData->quadVertexBufferPtr++;

		quadData->quadIndexCount += 6;
	}

	void Renderer::drawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, uint32_t entityIndex, const Ref<SubTexture2D> &texture){
		uint16_t textureIndex = texture ? pushTexture(texture->getTexture()) : 0;
		glm::vec2 UVstart = texture ? texture->getStart() : glm::vec2(0.f);
		glm::vec2 UVend = texture ? texture->getEnd() : glm::vec2(1.f);

		drawQuad(position, size, color, 1, textureIndex, UVstart, UVend, entityIndex);
	}

	void Renderer::drawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, uint32_t entityIndex, const Ref<Texture2D> &texture){
		uint16_t textureIndex = texture ? pushTexture(texture) : 0;
		drawQuad(position, size, color, 1, textureIndex, {0, 0}, {1, 1}, entityIndex);
	}

	void Renderer::drawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, float rotation, uint32_t entityIndex, const Ref<SubTexture2D> &texture){
		uint16_t textureIndex = texture ? pushTexture(texture->getTexture()) : 0;
		glm::vec2 UVstart = texture ? texture->getStart() : glm::vec2(0.f);
		glm::vec2 UVend = texture ? texture->getEnd() : glm::vec2(1.f);

		drawRotatedQuad(position, size, color, 1, textureIndex, UVstart, UVend, rotation, entityIndex);
	}

	void Renderer::drawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, float rotation, uint32_t entityIndex, const Ref<Texture2D> &texture){
		uint16_t textureIndex = texture ? pushTexture(texture) : 0;
		drawRotatedQuad(position, size, color, 1, textureIndex, {0, 0}, {1, 1}, rotation, entityIndex);
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

	void Renderer::drawCircle(const glm::mat4 &transform, glm::vec4 &color, uint32_t entityIndex, float thickness, float fade){
		
		ENGINE_PROFILE_FUNCTION();

		// add one to the entity index, so the index 0 is never used and count a no entity, instead of using a non unsigned int value
		entityIndex++;

		if (circleData->circleIndexCount + 6 > circleData->maxIndices){
			endScene();
			reloadScene();
		}

		circleData->circleVertexBufferPtr->position = transform * quadVertexPositions[0];
		circleData->circleVertexBufferPtr->color = color;
		circleData->circleVertexBufferPtr->localPos = glm::vec2(quadVertexPositions[0]) * glm::vec2(2);
		circleData->circleVertexBufferPtr->entityIndex = entityIndex;
		circleData->circleVertexBufferPtr->fade = fade;
		circleData->circleVertexBufferPtr->thickness = thickness;
		circleData->circleVertexBufferPtr++;

		circleData->circleVertexBufferPtr->position = transform * quadVertexPositions[1];
		circleData->circleVertexBufferPtr->color = color;
		circleData->circleVertexBufferPtr->localPos = glm::vec2(quadVertexPositions[1]) * glm::vec2(2);
		circleData->circleVertexBufferPtr->entityIndex = entityIndex;
		circleData->circleVertexBufferPtr->fade = fade;
		circleData->circleVertexBufferPtr->thickness = thickness;
		circleData->circleVertexBufferPtr++;

		circleData->circleVertexBufferPtr->position = transform * quadVertexPositions[2];
		circleData->circleVertexBufferPtr->color = color;
		circleData->circleVertexBufferPtr->localPos = glm::vec2(quadVertexPositions[2]) * glm::vec2(2);
		circleData->circleVertexBufferPtr->entityIndex = entityIndex;
		circleData->circleVertexBufferPtr->fade = fade;
		circleData->circleVertexBufferPtr->thickness = thickness;
		circleData->circleVertexBufferPtr++;

		circleData->circleVertexBufferPtr->position = transform * quadVertexPositions[3];
		circleData->circleVertexBufferPtr->color = color;
		circleData->circleVertexBufferPtr->localPos = glm::vec2(quadVertexPositions[3]) * glm::vec2(2);
		circleData->circleVertexBufferPtr->entityIndex = entityIndex;
		circleData->circleVertexBufferPtr->fade = fade;
		circleData->circleVertexBufferPtr->thickness = thickness;
		circleData->circleVertexBufferPtr++;

		circleData->circleIndexCount += 6;
	}

	void Renderer::drawTriangle(const Vertex &p1, const Vertex &p2, const Vertex &p3, const Ref<Texture2D> &texture, uint32_t entityIndex){
		ENGINE_PROFILE_FUNCTION();

		if (entityIndex == static_cast<uint32_t>(-1)){
			entityIndex = 0;
		} else {
			entityIndex++;
		}

		if (quadData->quadIndexCount + 6 > quadData->maxIndices){
			endScene();
			reloadScene();
		}

		uint16_t textureIndex = texture ? pushTexture(texture) : 0;

		quadData->quadVertexBufferPtr->position = glm::vec3(p1.position, 0.f);
		quadData->quadVertexBufferPtr->color = p1.color;
		quadData->quadVertexBufferPtr->TextCoord = p1.textCoord;
		quadData->quadVertexBufferPtr->textureIndex = textureIndex;
		quadData->quadVertexBufferPtr->tilingFactor = p1.tilingFactor;
		quadData->quadVertexBufferPtr->entityIndex = entityIndex;
		quadData->quadVertexBufferPtr++;

		quadData->quadVertexBufferPtr->position = glm::vec3(p2.position, 0.f);
		quadData->quadVertexBufferPtr->color = p2.color;
		quadData->quadVertexBufferPtr->TextCoord = p2.textCoord;
		quadData->quadVertexBufferPtr->textureIndex = textureIndex;
		quadData->quadVertexBufferPtr->tilingFactor = p2.tilingFactor;
		quadData->quadVertexBufferPtr->entityIndex = entityIndex;
		quadData->quadVertexBufferPtr++;

		quadData->quadVertexBufferPtr->position = glm::vec3(p3.position, 0.f);
		quadData->quadVertexBufferPtr->color = p3.color;
		quadData->quadVertexBufferPtr->TextCoord = p3.textCoord;
		quadData->quadVertexBufferPtr->textureIndex = textureIndex;
		quadData->quadVertexBufferPtr->tilingFactor = p3.tilingFactor;
		quadData->quadVertexBufferPtr->entityIndex = entityIndex;
		quadData->quadVertexBufferPtr++;

		quadData->quadIndexCount += 3;
	}

	void Renderer::drawTriangle(const Vertex &p1, const Vertex &p2, const Vertex &p3, const glm::mat4 &transform, const Ref<Texture2D> &texture, uint32_t entityIndex){
		ENGINE_PROFILE_FUNCTION();

		if (entityIndex == static_cast<uint32_t>(-1)){
			entityIndex = 0;
		} else {
			entityIndex++;
		}

		if (quadData->quadIndexCount + 3 > quadData->maxIndices){
			endScene();
			reloadScene();
		}

		uint16_t textureIndex = texture ? pushTexture(texture) : 0;

		quadData->quadVertexBufferPtr->position = transform * glm::vec4(p1.position, 0.f, 1.f);
		quadData->quadVertexBufferPtr->color = p1.color;
		quadData->quadVertexBufferPtr->TextCoord = p1.textCoord;
		quadData->quadVertexBufferPtr->textureIndex = textureIndex;
		quadData->quadVertexBufferPtr->tilingFactor = p1.tilingFactor;
		quadData->quadVertexBufferPtr->entityIndex = entityIndex;
		quadData->quadVertexBufferPtr++;

		quadData->quadVertexBufferPtr->position = transform * glm::vec4(p2.position, 0.f, 1.f);
		quadData->quadVertexBufferPtr->color = p2.color;
		quadData->quadVertexBufferPtr->TextCoord = p2.textCoord;
		quadData->quadVertexBufferPtr->textureIndex = textureIndex;
		quadData->quadVertexBufferPtr->tilingFactor = p2.tilingFactor;
		quadData->quadVertexBufferPtr->entityIndex = entityIndex;
		quadData->quadVertexBufferPtr++;

		quadData->quadVertexBufferPtr->position = transform * glm::vec4(p3.position, 0.f, 1.f);
		quadData->quadVertexBufferPtr->color = p3.color;
		quadData->quadVertexBufferPtr->TextCoord = p3.textCoord;
		quadData->quadVertexBufferPtr->textureIndex = textureIndex;
		quadData->quadVertexBufferPtr->tilingFactor = p3.tilingFactor;
		quadData->quadVertexBufferPtr->entityIndex = entityIndex;
		quadData->quadVertexBufferPtr++;

		quadData->quadIndexCount += 3;
	}


	void Renderer::drawTriangle(const Vertex &p1, const Vertex &p2, const Vertex &p3, const glm::vec2 &translation, const glm::vec2 &scale, float angle, const Ref<Texture2D> &texture, uint32_t entityIndex){
		ENGINE_PROFILE_FUNCTION();

		glm::mat4 rotation = glm::rotate(glm::mat4(1.f), angle, {0.f, 0.f, 1.f});
		glm::mat4 transform = glm::translate(glm::mat4(1.f), glm::vec3(translation, 0.f)) * rotation * glm::scale(glm::mat4(1.f), glm::vec3(scale, 1.f));

		drawTriangle(p1, p2, p3, transform, texture, entityIndex);
	}

	void Renderer::drawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, float rotation, uint32_t entityIndex){
		drawRotatedQuad(position, size, color, 1.f, 0, {0.f, 0.f}, {1.f, 1.f}, rotation, entityIndex);
	}

	void Renderer::drawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, uint32_t entityIndex){
		drawQuad(position, size, color, 1.f, 0, {0.f, 0.f}, {1.f, 1.f}, entityIndex);
	}

	void Renderer::drawLine(const glm::vec2 &start, const glm::vec2 &end, uint32_t entityIndex, glm::vec4 color){
		ENGINE_PROFILE_FUNCTION();

		if (entityIndex == static_cast<uint32_t>(-1)){
			entityIndex = 0;
		} else {
			entityIndex++;
		}

		if (lineData->lineCount + 2 > lineData->maxVertices){
			endScene();
			reloadScene();
		}

		lineData->lineVertexBufferPtr->position = glm::vec3(start, 0);
		lineData->lineVertexBufferPtr->color = color;
		lineData->lineVertexBufferPtr->entityIndex = entityIndex;
		lineData->lineVertexBufferPtr++;
		
		lineData->lineVertexBufferPtr->position = glm::vec3(end, 0);
		lineData->lineVertexBufferPtr->color = color;
		lineData->lineVertexBufferPtr->entityIndex = entityIndex;
		lineData->lineVertexBufferPtr++;

		lineData->lineCount += 2;
	}

	void Renderer::setLineThickness(float thickness){
		renderCommand.setLineThickness(thickness);
	}

	void Renderer::drawSquare(const glm::mat4 &transform, const glm::vec4 &color, uint32_t entityIndex){
		glm::vec2 p0 = transform * quadVertexPositions[0];
		glm::vec2 p1 = transform * quadVertexPositions[1];
		glm::vec2 p2 = transform * quadVertexPositions[2];
		glm::vec2 p3 = transform * quadVertexPositions[3];

		drawLine(p0, p1, entityIndex, color);
		drawLine(p1, p2, entityIndex, color);
		drawLine(p2, p3, entityIndex, color);
		drawLine(p3, p0, entityIndex, color);
	}

}
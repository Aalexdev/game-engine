#pragma once

#include "../core.hpp"
#include "RenderCommand.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Texture2D.hpp"
#include "SubTexture2D.hpp"
#include "Texture2DLibrary.hpp"
#include "../UUID.hpp"

namespace engine{
	class ENGINE_API Renderer{
		public:
			enum class RenderAPI{
				OpenGL,
				Vulkan,
				Metal,
				DirectX
			};

			static inline RenderAPI getRenderAPI() {return renderAPI;} 
			static inline void setRenderAPI(RenderAPI API) {renderAPI = API;}

			struct Vertex{
				glm::vec2 position;
				glm::vec2 textCoord;
				glm::vec4 color;
				float tilingFactor;
			};

			Renderer(){}
			~Renderer(){}

			void init();
			void shutdown();

			void OnWindowResized(uint32_t width, uint32_t height);

			void beginScene(const Camera &camera);
			void endScene();

			inline void clear() {renderCommand.clear();}
			inline void setClearColor(const glm::vec4& clearColor) {renderCommand.setClearColor(clearColor);}

			void setShader(const Ref<Shader> &shader);

			void drawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, uint32_t entityIndex);
			void drawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, uint32_t entityIndex, const Ref<SubTexture2D> &texture);
			void drawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, uint32_t entityIndex, const Ref<Texture2D> &texture);
			void drawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, float rotation, uint32_t entityIndex);
			void drawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, float rotation, uint32_t entityIndex, const Ref<SubTexture2D> &texture);
			void drawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, float rotation, uint32_t entityIndex, const Ref<Texture2D> &texture);
			void drawQuad(const glm::mat4 &transform, const glm::vec4 &color, uint32_t entityIndex, const Ref<SubTexture2D> &texture);

			void drawCircle(const glm::mat4 &transform, glm::vec4 &color, uint32_t entityIndex, float thickness = 1.f, float fade = 0.005);

			void drawTriangle(const Vertex &p1, const Vertex &p2, const Vertex &p3, const Ref<Texture2D> &texture = nullptr, uint32_t entityIndex = -1);
			void drawTriangle(const Vertex &p1, const Vertex &p2, const Vertex &p3, const glm::vec2 &translation, const glm::vec2 &scale, float angle, const Ref<Texture2D> &texture = nullptr, uint32_t entityIndex = -1);
			void drawTriangle(const Vertex &p1, const Vertex &p2, const Vertex &p3, const glm::mat4 &transform, const Ref<Texture2D> &texture = nullptr, uint32_t entityIndex = -1);

		private:
			static RenderAPI renderAPI;

			struct QuadVertex{
				glm::vec3 position;
				glm::vec2 TextCoord;
				glm::vec4 color;
				uint32_t textureIndex;
				float tilingFactor;

				uint32_t entityIndex;
			};

			struct CircleVertex{
				glm::vec3 position;
				glm::vec2 localPos;
				glm::vec4 color;
				float thickness;
				float fade;

				uint32_t entityIndex;
			};

			struct QuadData{
				uint32_t maxQuads = 10000;
				uint32_t maxVertices;
				uint32_t maxIndices;
				uint32_t maxTextureSlots = 32;

				Ref<VertexArray> quadVertexArray;
				Ref<VertexBuffer> quadVertexBuffer;

				uint32_t quadIndexCount = 0;
				QuadVertex* quadVertexBufferBase = nullptr;
				QuadVertex* quadVertexBufferPtr = nullptr;

				Scope<Ref<Texture2D>[]> textureSlots;
				uint32_t textureSlotIndex = 1;


				QuadData(uint32_t maxQuads = 10000) : maxQuads{maxQuads}{
					maxVertices = maxQuads * 4;
					maxIndices = maxQuads * 6;
				}
			};

			struct CircleData{
				uint32_t maxCircle = 10000;
				uint32_t maxVertices;
				uint32_t maxIndices;
				
				Ref<VertexArray> circleVertexArray;
				Ref<VertexBuffer> circleVertexBuffer;

				uint32_t circleIndexCount = 0;
				CircleVertex* circleVertexBufferBase = nullptr;
				CircleVertex* circleVertexBufferPtr = nullptr;

				CircleData(uint32_t maxCircle = 10000) : maxCircle{maxCircle}{
					maxVertices = maxCircle * 4;
					maxIndices = maxCircle * 6;
				}
			};
			
			glm::vec4 quadVertexPositions[4];

			Ref<Shader> quadShader;
			Ref<Shader> circleShader;

			void flush();
			void reloadScene();

			RenderCommand renderCommand;
			Scope<QuadData> quadData;
			Scope<CircleData> circleData;

			uint16_t texturePushed(const Ref<Texture2D> &texture);
			uint16_t pushTexture(const Ref<Texture2D> &texture);

			void drawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4& color, float tilingFactor, uint32_t textureIndex, const glm::vec2 &TextureCoordsStart, const glm::vec2 &TextureCoordsEnd, uint32_t entityIndex);
			void drawRotatedQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4& color, float tilingFactor, uint32_t textureIndex, const glm::vec2 &TextureCoordsStart, const glm::vec2 &TextureCoordsEnd, float angle, uint32_t entityIndex);
			void drawCircle(const glm::vec3 &center, const glm::vec2 &size, const glm::vec4 &color, float thickness, float fade, uint32_t entityIndex);

			void loadShaders();
			void loadQuadShader();
			void loadCircleShader();

	};
}
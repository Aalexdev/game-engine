#pragma once

#include "../core.hpp"
#include "../pch.hpp"
#include "../renderer/SubTexture2D.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/VertexArray.hpp"
#include "../renderer/VertexBuffer.hpp"
#include "../renderer/Framebuffer.hpp"
#include "../renderer/Texture2D.hpp"
#include "../renderer/Shader.hpp"


namespace engine::OpenGL{
	class ENGINE_API Renderer : public engine::Renderer{
		public:
			static Ref<Renderer> create(const Definition &def);
			Renderer(const Definition &def);
			~Renderer() override;
			
			virtual void swap() override;
		
		protected:
			virtual void drawQuad_impl_(const glm::mat3 &transform, const glm::vec4 &color, const SubTexture &texture, uint32_t entityIndex) override;
			virtual void setRefreshRate_impl_(float fps) override;
			virtual void setClearColor_impl_(const glm::vec3 &color) override;
			virtual void setViewport_impl_(const glm::vec2 &viewport) override;
			virtual void resized_impl_(const glm::vec2 &size) override;
			virtual void enableVSync_impl_(bool enabled) override;
			virtual void clear_impl_() override;
			virtual void useShader_impl_(const std::string &name) override;
			virtual void setViewProjectionMat_impl_(const glm::mat3 &mat) override;

			virtual uint8_t getMaxTextureSlots() override;
			
			virtual void reloadScene() override;
			virtual void flush() override;
			virtual void endScene() override;

			virtual uint8_t texturePushed(const Ref<Texture2D> &texture) override;
			virtual uint8_t pushTexture(const Ref<Texture2D> &texture) override;

			virtual void loadShader_impl_(const std::string &name, std::filesystem::path path) override;

			virtual void bindDefaultShader() override;
		
		private:

			struct Vertex{
				glm::vec2 position;
				glm::vec2 UV;
				glm::vec4 color;
				uint32_t textureIndex;
				uint32_t entityIndex;
				float tilingFactor;
			};

			Definition properties;
			void* context = nullptr;
			bool VSyncEnabled = true;
			float minSecPerFrame = 0.0016f;

			uint32_t indexCount = 0;
			uint32_t maxIndices = 0;
			uint32_t maxVertices = 0;

			Ref<VertexArray> vertexArray = nullptr;
			Ref<VertexBuffer> vertexBuffer = nullptr;

			Vertex* vertexBufferBase = nullptr;
			Vertex* vertexBufferPtr = nullptr;

			Ref<Framebuffer> framebuffer = nullptr;

			std::unordered_map<std::string, Ref<Shader>> shaders;
			Ref<Shader> currentShader = nullptr;
			
			uint16_t textureSlotIndex[2];
			Scope<Ref<Texture2D>[]> textureSlots[2];

			void initializeVertexBufferLayout(BufferLayout &layout);
			void initializeVertexBuffer();
	};
}
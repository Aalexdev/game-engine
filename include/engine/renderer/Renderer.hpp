#pragma once

#include "../core.hpp"
#include "../pch.hpp"
#include "../displays/Display.hpp"
#include "FramebufferLayout.hpp"
#include "RenderCommand.hpp"
#include "RenderAPI.hpp"

#include <libs/glm/glm.hpp>

namespace engine{
	class SubTexture2D;
	class Texture2D;
	class ENGINE_API Renderer{
		public:
			static constexpr uint16_t REFRESH_RATE_UNLIMITED = static_cast<uint16_t>(-1);

			struct ENGINE_API Definition{
				Ref<Display> display = nullptr;
				glm::vec3 clearColor = {0.f, 0.f, 0.f};
				bool VSync = true;
				uint16_t refreshRate = 60; // used when VSync is not enabled
				glm::u32vec2 resolution = {1080, 720};
				FramebufferAttachmentSpecification attachments = {FramebufferTextureFormat::RGB8};
				uint16_t bufferSize = 2000;
				uint16_t commandCount = 500;
				RenderAPI renderAPI = RenderAPI::OPENGL;
			};

			static Ref<Renderer> create(const Definition &def);
			virtual ~Renderer() = default;

			void drawQuad(const glm::mat3 &transform, uint32_t entityIndex = 0);
			void drawQuad(const glm::mat3 &transform, const glm::vec4 &color, uint32_t entityIndex = 0);
			void drawTexturedQuad(const glm::mat3 &transform, const Ref<SubTexture2D> &texture, uint32_t entityIndex = 0);
			void drawTexturedQuad(const glm::mat3 &transform, const Ref<SubTexture2D> &texture, const glm::vec4 &color, uint32_t entityIndex = 0);

			void setRefreshRate(uint16_t refreshRate);
			void setClearColor(const glm::vec3 &color);
			void setViewport(const glm::u32vec2 &viewport);
			void resize(const glm::u32vec2 &resolution);
			void enableVSync(bool enabled = true);
			void clear();
			void useShader(const std::string &name);
			void setViewProjectionMat(const glm::mat3 &mat);

			virtual void swap() = 0;

			void loadShader(const std::string &name, std::filesystem::path path);
			RenderAPI getRenderAPI() const;
		
		protected:
			std::vector<float> dataBuffers[2];
			std::vector<RenderCommand> commandQueues[2];
			uint8_t currentQueue = 0;
			glm::mat3 viewProjectionMat = glm::mat3(1.f);
			glm::vec3 vertexPosition[4];
			
			void render();

			struct SubTexture{
				glm::vec2 start = {0.f, 0.f};
				glm::vec2 end = {1.f, 1.f};
				uint32_t textureIndex = 0;
			};

			virtual void drawQuad_impl_(const glm::mat3 &transform, const glm::vec4 &color, const SubTexture &texture, uint32_t entityIndex) = 0;
			virtual void setRefreshRate_impl_(float fps) = 0;
			virtual void setClearColor_impl_(const glm::vec3 &color) = 0;
			virtual void setViewport_impl_(const glm::vec2 &viewport) = 0;
			virtual void resized_impl_(const glm::vec2 &size) = 0;
			virtual void enableVSync_impl_(bool enabled) = 0;
			virtual void clear_impl_() = 0;
			virtual void useShader_impl_(const std::string &name) = 0;
			virtual void setViewProjectionMat_impl_(const glm::mat3 &mat) = 0;

			virtual uint8_t getMaxTextureSlots() = 0;

			virtual uint8_t texturePushed(const Ref<Texture2D> &texture) = 0;
			virtual uint8_t pushTexture(const Ref<Texture2D> &texture) = 0;

			virtual void loadShader_impl_(const std::string &name, std::filesystem::path path) = 0;

			virtual void reloadScene() = 0;
			virtual void flush() = 0;
			virtual void endScene() = 0;

			virtual void bindDefaultShader() = 0;

			
			RenderAPI renderAPI;
			
		private:

			float* pushMat3(const glm::mat3 &mat);
			float* pushVec4(const glm::vec4 &vec);
			float* pushVec3(const glm::vec3 &vec);
			float* pushVec2(const glm::vec2 &vec);
			float* pushFloat(float value);
			float* pushTexture(const Ref<SubTexture2D> &texture);
			float* pushStr(const std::string &str);

			void popMat3(glm::mat3 &mat, float *data, uint32_t &i);
			void popVec4(glm::vec4 &vec, float *data, uint32_t &i);
			void popVec3(glm::vec3 &vec, float *data, uint32_t &i);
			void popVec2(glm::vec2 &vec, float *data, uint32_t &i);
			void popFloat(float &value, float *data, uint32_t &i);
			void popTexture(SubTexture &texture, float *data, uint32_t &i);
			void popStr(std::string &str, float *data, uint32_t &i);
			void popBool(bool &value, float *data, uint32_t &i);

			void renderQuad(float *data);
			void renderQuadColored(float *data);
			void renderTexturedQuad(float *data);
			void renderTexturedQuadColored(float *data);
			void setRefreshRate(float *data);
			void setClearColor(float *data);
			void setViewport(float *data);
			void resized(float *data);
			void enableVSync(float *data);
			void clear(float *data);
			void useShader(float *data);
			void setViewProjectionMat(float *data);
	};
}
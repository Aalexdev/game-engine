#include "engine/pch.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/renderer/Texture2D.hpp"
#include "engine/renderer/Renderer.hpp"

#include "engine/OpenGlAL/OpenGlTexture2D.hpp"

namespace engine{
	Ref<Texture2D> Texture2D::create(const std::string &filepath, const Ref<Texture2DLibrary> &library){
		switch (Renderer::getRenderAPI()){
			case Renderer::RenderAPI::OpenGL: return createRef<OpenGl::OpenGlTexture2D>(filepath, library);
		}
		ENGINE_CORE_ASSERT(false, "unknown render API");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::create(uint32_t width, uint32_t height, TextureFormat format, void* data, const Ref<Texture2DLibrary> &library){
		switch (Renderer::getRenderAPI()){
			case Renderer::RenderAPI::OpenGL: return createRef<OpenGl::OpenGlTexture2D>(width, height, format, data, library);
		}

		ENGINE_CORE_ASSERT(false, "unknown render API");
		return nullptr;
	}

	glm::vec2 Texture2D::topCorner(){
		if (Renderer::getRenderAPI() == Renderer::RenderAPI::OpenGL){
			return {0, 1};
		}
		return {0, 0};
	}

	glm::vec2 Texture2D::bottomCorner(){
		if (Renderer::getRenderAPI() == Renderer::RenderAPI::OpenGL){
			return {1, 0};
		}
		return {1, 1};
	}
}
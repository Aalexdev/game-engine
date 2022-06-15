#include "engine/pch.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/renderer/Texture2D.hpp"
#include "engine/renderer/Renderer.hpp"

#include "engine/OpenGL_AL/OpenGL_Texture2D.hpp"

namespace engine{
	Ref<Texture2D> Texture2D::create(Renderer *renderer, uint32_t width, uint32_t height, TextureFormat format, void* data){
		switch (renderer->getRenderAPI()){
			case RenderAPI::OPENGL: return OpenGL::Texture2D::create(renderer, width, height, format, data);
		}

		ENGINE_CORE_ASSERT(false, "unknown render API");
		return nullptr;
	}
}
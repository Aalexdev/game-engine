#include "engine/pch.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/renderer/RendererAPI.hpp"
#include "engine/renderer/Renderer.hpp"

#include "engine/OpenGlAL/OpenGlRendererAPI.hpp"

namespace engine{
	Ref<RendererAPI> RendererAPI::create(){
		ENGINE_PROFILE_FUNCTION();
		switch (Renderer::getRenderAPI()){
			case Renderer::RenderAPI::OpenGL: return createRef<OpenGl::OpenGlRendererAPI>();
		}
		ENGINE_CORE_ASSERT(false, "unknown render API");
		return nullptr;
	}
}
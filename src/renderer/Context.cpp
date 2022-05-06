#include "engine/renderer/Context.hpp"
#include "engine/renderer/Renderer.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"

#include "engine/OpenGlAL/OpenGlContext.hpp"

namespace engine{
	Ref<Context> Context::create(Window *window){
		ENGINE_PROFILE_FUNCTION();
		switch (Renderer::getRenderAPI()){
			case Renderer::RenderAPI::OpenGL: return OpenGl::OpenGLContext::create(window);
		}
		ENGINE_CORE_INFO(false, "unsuported context");
		return nullptr;
	}
}
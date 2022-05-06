#include "engine/pch.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/renderer/Framebuffer.hpp"
#include "engine/renderer/Renderer.hpp"

#include "engine/OpenGlAL/OpenGlFramebuffer.hpp"

namespace engine{
	Ref<Framebuffer> Framebuffer::create(const FramebufferLayout &layout){
		switch (Renderer::getRenderAPI()){
			case Renderer::RenderAPI::OpenGL: return createRef<OpenGl::OpenGlFramebuffer>(layout);
		}
		ENGINE_CORE_ASSERT(false, "unknown render API");
		return nullptr;
	}
}
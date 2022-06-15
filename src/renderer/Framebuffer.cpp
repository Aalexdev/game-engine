#include "engine/pch.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/renderer/Framebuffer.hpp"
#include "engine/renderer/Renderer.hpp"

#include "engine/OpenGL_AL/OpenGL_Framebuffer.hpp"

namespace engine{
	Ref<Framebuffer> Framebuffer::create(Renderer *renderer, const FramebufferLayout &layout){
		switch (renderer->getRenderAPI()){
			case RenderAPI::OPENGL: return OpenGL::Framebuffer::create(renderer, layout);
		}
		ENGINE_CORE_ASSERT(false, "unknown render API");
		return nullptr;
	}
}
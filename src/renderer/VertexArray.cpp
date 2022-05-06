#include "engine/pch.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/renderer/VertexArray.hpp"
#include "engine/renderer/Renderer.hpp"

#include "engine/OpenGlAL/OpenGlVertexArray.hpp"

namespace engine{
	Ref<VertexArray> VertexArray::create(){
		switch (Renderer::getRenderAPI()){
			case Renderer::RenderAPI::OpenGL: return createRef<OpenGl::OpenGlVertexArray>();
		}
		ENGINE_CORE_ASSERT(false, "unknown render API");
		return nullptr;
	}
}
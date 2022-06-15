#include "engine/pch.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/renderer/VertexArray.hpp"
#include "engine/renderer/Renderer.hpp"

#include "engine/OpenGL_AL/OpenGL_VertexArray.hpp"

namespace engine{
	Ref<VertexArray> VertexArray::create(Renderer *renderer){
		switch (renderer->getRenderAPI()){
			case RenderAPI::OPENGL: return createRef<OpenGL::VertexArray>(renderer);
		}
		ENGINE_CORE_ASSERT(false, "unknown render API");
		return nullptr;
	}
}
#include "engine/pch.hpp"
#include "engine/renderer/IndexBuffer.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/renderer/Renderer.hpp"

#include "engine/OpenGL_AL/OpenGL_IndexBuffer.hpp"

namespace engine{
	Ref<IndexBuffer> IndexBuffer::create(Renderer *renderer, uint32_t *indices, uint32_t count){
		switch (renderer->getRenderAPI()){
			case RenderAPI::OPENGL: return OpenGL::IndexBuffer::create(renderer, indices, count);
		}
		ENGINE_CORE_ASSERT(false, "unknown render API");
		return nullptr;
	}
}
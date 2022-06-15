#include "engine/pch.hpp"
#include "engine/renderer/VertexBuffer.hpp"
#include "engine/renderer/Renderer.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"

#include "engine/OpenGL_AL/OpenGL_VertexBuffer.hpp"

namespace engine{
	Ref<VertexBuffer> VertexBuffer::create(Renderer *renderer,float *vertices, uint32_t size){
		switch (renderer->getRenderAPI()){
			case RenderAPI::OPENGL: return OpenGL::VertexBuffer::create(renderer, vertices, size);
		}
		ENGINE_CORE_ASSERT(false, "unknown render API");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::create(Renderer *renderer,uint32_t size){
		switch (renderer->getRenderAPI()){
			case RenderAPI::OPENGL: return OpenGL::VertexBuffer::create(renderer, size);
		}
		ENGINE_CORE_ASSERT(false, "unknown render API");
		return nullptr;
	}
}
#include "engine/pch.hpp"
#include "engine/renderer/VertexBuffer.hpp"
#include "engine/renderer/Renderer.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"

#include "engine/OpenGlAL/OpenGlVertexBuffer.hpp"

namespace engine{
	Ref<VertexBuffer> VertexBuffer::create(float *vertices, uint32_t size){
		switch (Renderer::getRenderAPI()){
			case Renderer::RenderAPI::OpenGL: return createRef<OpenGl::OpenGlVertexBuffer>(vertices, size);
		}
		ENGINE_CORE_ASSERT(false, "unknown render API");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::create(uint32_t size){
		switch (Renderer::getRenderAPI()){
			case Renderer::RenderAPI::OpenGL: return createRef<OpenGl::OpenGlVertexBuffer>(size);
		}
		ENGINE_CORE_ASSERT(false, "unknown render API");
		return nullptr;
	}
}
#include "engine/pch.hpp"
#include "engine/renderer/IndexBuffer.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/renderer/Renderer.hpp"

#include "engine/OpenGlAL/OpenGlIndexBuffer.hpp"

namespace engine{
	Ref<IndexBuffer> IndexBuffer::create(uint32_t *indices, uint32_t count){
		switch (Renderer::getRenderAPI()){
			case Renderer::RenderAPI::OpenGL: return createRef<OpenGl::OpenGlIndexBuffer>(indices, count);
		}
		ENGINE_CORE_ASSERT(false, "unknown render API");
		return nullptr;
	}
}
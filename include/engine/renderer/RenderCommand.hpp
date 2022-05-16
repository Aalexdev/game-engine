#pragma once

#include "../core.hpp"
#include "RendererAPI.hpp"

namespace engine{
	class ENGINE_API RenderCommand{
		public:
			RenderCommand(){
				rendererAPI = RendererAPI::create();
			}

			~RenderCommand(){}

			inline void init(){
				rendererAPI->init();
			}

			inline void shutdown(){
				rendererAPI = nullptr;
			}

			inline void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height){
				rendererAPI->setViewport(x, y, width, height);
			}

			inline void clear(){
				rendererAPI->clear();
			}

			inline void setClearColor(const glm::vec4 &clearColor){
				rendererAPI->setClearColor(clearColor);
			}

			inline void drawIndexed(const Ref<VertexArray> &vertexArray, uint32_t indexCount = 0){
				rendererAPI->drawIndexed(vertexArray, indexCount);
			}

			inline void drawLines(const Ref<VertexArray> &vertexArray, uint32_t indexCount = 0){
				rendererAPI->drawLines(vertexArray, indexCount);
			}

			inline void setLineThickness(float thickness){
				rendererAPI->setLineThickness(thickness);
			}

			inline const Ref<RendererAPI>& getRendererAPI() const {return rendererAPI;}

		private:
			Ref<RendererAPI> rendererAPI;		
	};
}
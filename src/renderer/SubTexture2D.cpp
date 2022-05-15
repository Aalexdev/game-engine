#include "engine/pch.hpp"
#include "engine/renderer/SubTexture2D.hpp"
#include "engine/renderer/Renderer.hpp"

namespace engine{

	Ref<SubTexture2D> SubTexture2D::create(const Ref<Texture2D> &texture, uint32_t x, uint32_t y, uint32_t width, uint32_t height){
		return createRef<SubTexture2D>(texture, x, y, width, height);
	}

	Ref<SubTexture2D> SubTexture2D::create(const Ref<Texture2D> &texture){
		return createRef<SubTexture2D>(texture);
	}

	static void checkY(uint32_t &y, uint32_t height){
		// opengl invert the y axis
		if (Renderer::getRenderAPI() == Renderer::RenderAPI::OpenGL) y = height - y;
	}

	SubTexture2D::SubTexture2D(const Ref<Texture2D> &texture, uint32_t x, uint32_t y, uint32_t width, uint32_t height) : texture{texture}{

		// normalize the positions
		updateStart(x, y);
		updateSize(width, height);
	}

	SubTexture2D::SubTexture2D(const Ref<Texture2D> &texture) : texture{texture}{
		updateStart(0, 0);
		updateSize(texture->getWidth(), texture->getHeight());
	}


	void SubTexture2D::updateStart(uint32_t x, uint32_t y){
		if (x > end.x){
			x = end.x -1;
		}

		if (y > end.y){
			y = end.y - 1;
		}

		
		start = {x, y};

		checkY(y, texture->getHeight());

		normalizedStart.x = static_cast<float>(x) / static_cast<float>(texture->getWidth());
		normalizedStart.y = static_cast<float>(y) / static_cast<float>(texture->getHeight());
	}

	void SubTexture2D::updateEnd(uint32_t x, uint32_t y){

		if (x < start.x){
			x = start.x + 1;
		}

		if (y < start.y){
			y = start.y + 1;
		}

		end = {x, y};
		
		checkY(y, texture->getHeight());

		normalizedEnd.x = static_cast<float>(x) / static_cast<float>(texture->getWidth());
		normalizedEnd.y = static_cast<float>(y) / static_cast<float>(texture->getHeight());
	}

	void SubTexture2D::updateSize(uint32_t width, uint32_t height){
		updateEnd(start.x + width, start.y + height);
	}
}
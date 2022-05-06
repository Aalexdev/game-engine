#pragma once

#include "../core.hpp"
#include "Texture2D.hpp"
#include <libs/glm/glm.hpp>

namespace engine{
	class ENGINE_API SubTexture2D{
		public:
			SubTexture2D() = default;
			SubTexture2D(const Ref<Texture2D> &texture, uint32_t x, uint32_t y, uint32_t width, uint32_t height);
			SubTexture2D(const Ref<Texture2D> &texture);

			inline const Ref<Texture2D>& getTexture() const {return texture;}

			inline const glm::vec2& getStart() const {return normalizedStart;}
			inline const glm::vec2& getEnd() const {return normalizedEnd;}

			inline const glm::u32vec2& getRelativeStart() const {return start;}
			inline const glm::u32vec2& getRelativeEnd() const {return end;}

			void setStart(uint32_t x, uint32_t y) {updateStart(x, y);}
			void setEnd(uint32_t x, uint32_t y) {updateEnd(x, y);}
			void setSize(uint32_t width, uint32_t height) {updateSize(width, height);}

			static Ref<SubTexture2D> create(const Ref<Texture2D> &texture, uint32_t x, uint32_t y, uint32_t width, uint32_t height);
			static Ref<SubTexture2D> create(const Ref<Texture2D> &texture);

		private:
			void updateStart(uint32_t x, uint32_t y);
			void updateEnd(uint32_t x, uint32_t y);
			void updateSize(uint32_t width, uint32_t height);

			Ref<Texture2D> texture;

			glm::u32vec2 start;
			glm::u32vec2 end;

			glm::vec2 normalizedStart = {0.f, 0.f};
			glm::vec2 normalizedEnd = {1.f, 1.f};
	};
}
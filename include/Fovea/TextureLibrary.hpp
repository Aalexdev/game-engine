#pragma once

#include "Texture.hpp"
#include <vector>

namespace Fovea{
	class TextureLibrary{
		public:
			using ID = size_t;
			~TextureLibrary();

			ID push(Texture* texture);
			Texture* get(ID id);
			void erase(ID id);

			void clear();

		private:
			std::vector<Texture*> textures;
			uint32_t holes = 0;
	};
}
#include "Fovea/TextureLibrary.hpp"

namespace Fovea{
	TextureLibrary::~TextureLibrary(){
		clear();
	}

	TextureLibrary::ID TextureLibrary::push(Texture* texture){
		if (holes == 0){
			ID id = textures.size();
			textures.push_back(texture);
			return id;
		}
		ID id = 0;
		for (; id<textures.size(); id++){
			if (textures[id] == nullptr){
				textures[id] = texture;
			}
		}

		holes--;
		return id;
	}

	Texture* TextureLibrary::get(ID id){
		return textures[id];
	}

	void TextureLibrary::erase(ID id){
		delete textures[id];
		textures[id] = nullptr;
		holes++;
	}

	void TextureLibrary::clear(){
		for (auto t : textures){
			delete t;
		}
		textures.clear();
		holes = 0;
	}
}
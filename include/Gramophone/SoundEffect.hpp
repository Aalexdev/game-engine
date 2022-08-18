#pragma once

#include "OpenAl/al.h"

namespace Gramophone{
	class SoundEffect{
		friend class SoundSource;
		public:
			~SoundEffect();
			SoundEffect() = default;
			SoundEffect(const char* filepath){
				initialize(filepath);
			}

			void initialize(const char* filepath);

		private:
			void load(const char* filepath);
			ALuint soundEffect;
	};
}
#pragma once

#include "OpenAl/al.h"
#include "SoundEffect.hpp"
#include "glm/glm.hpp"

namespace Gramophone{
	class SoundSource{
		friend class EffectSlot;
		public:
			SoundSource();

			~SoundSource();

			void play(SoundEffect *sound);

			void setPitch(float pitch);
			void setGain(float gain);
			void setPosition(const glm::vec3 &position);
			void setVelocity(const glm::vec3 &velocity);
			void setLoop(bool looping);


		private:
			float pitch = 1.f;
			float gain = 1.f;
			glm::vec3 position = glm::vec3(0.f);
			glm::vec3 velocity = glm::vec3(0.f);
			bool loop = false; 

			ALuint source;
			SoundEffect *effect;
	};
}
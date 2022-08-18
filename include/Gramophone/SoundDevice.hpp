#pragma once

#include "OpenAl/alc.h"
#include "glm/glm.hpp"

namespace Gramophone{
	class SoundDevice{
		public:
			~SoundDevice();
			static void initialize();

			void setListenerPosition(const glm::vec3 &position);

			void setListenerOrientation(const glm::vec3 &at, const glm::vec3 &up);

			void setListenerGain(float gain);

			void setListenerPitch(float pitch);

		private:
			static SoundDevice& getInstance();

			ALCdevice* device = nullptr;
			ALCcontext* context = nullptr;

	};
}
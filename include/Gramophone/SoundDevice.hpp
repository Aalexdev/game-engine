#pragma once

#include "OpenAl/alc.h"

namespace Gramophone{
	class SoundDevice{
		public:
			~SoundDevice();
			static void initialize();

		private:
			static SoundDevice& getInstance();

			ALCdevice* device = nullptr;
			ALCcontext* context = nullptr;

	};
}
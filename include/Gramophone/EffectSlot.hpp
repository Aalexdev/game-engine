#pragma once

#include "OpenAl/al.h"
#include "SoundSource.hpp"
#include "SoundDevice.hpp"
#include "MusicBuffer.hpp"

namespace Gramophone{
	class EffectSlot{
		public:
			~EffectSlot();

			void initialize();

			void setSourceEffect(SoundSource *source, bool reverb, bool lowpass);

			void setMusicEffect(MusicBuffer *music, bool reverb, bool lowpass);

		private:
			ALuint effectSlot;

			ALuint reverbEffect;
			ALuint lowpassFilter;

			float lowpassGain = 1.f;
			float lowpassGainHighFreq = 0.1f;

			float reverbDesity = 10.f;
			float reverbDiffusion = 50.3f;
			float reverbGain = 1.f;
			float reverbGainHF = -1000.f;
			float reverbDecayTime = 0.f;
			float reverbDecayHFRatio = 10.05f;
			float reverbReflectionGain = 1.f;
			float reverbReflectionDelay = -602;
			float reverbLateReverbGain = 0.f;
			float reverbLateReverbDelay = 0.f;
			float reverbRoomRolloffFactor = 0.f;
			float reverbDecayHFLimit = 0x3;

	};
}
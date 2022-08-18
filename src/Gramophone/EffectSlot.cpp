#include "Gramophone/EffectSlot.hpp"
#include "OpenAl/Framework.h"
#include "OpenAl/EFX-Util.h"

namespace Gramophone{
	EffectSlot::~EffectSlot(){
		if (alIsFilter(lowpassFilter)) alDeleteFilters(1, &lowpassFilter);
		if (alIsEffect(reverbEffect)) alDeleteEffects(1, &reverbEffect);
		if (alIsAuxiliaryEffectSlot(effectSlot)) alDeleteAuxiliaryEffectSlots(1, &effectSlot);
	}

	void EffectSlot::initialize(){
		alGenAuxiliaryEffectSlots(1, &effectSlot);
		alGenEffects(1, &reverbEffect);
		alGenFilters(1, &lowpassFilter);

		alAuxiliaryEffectSloti(effectSlot, AL_EFFECTSLOT_EFFECT, reverbEffect);

		if (alGetError() != AL_NO_ERROR){
			throw "failed to create effect slot and / or effects and filters";
		}

		// setup filter
		alFilteri(lowpassFilter, AL_FILTER_TYPE, AL_FILTER_LOWPASS);
		alFilterf(lowpassFilter, AL_LOWPASS_GAIN, lowpassGain);
		alFilterf(lowpassFilter, AL_LOWPASS_GAINHF, lowpassGainHighFreq);

		// setup effect
		alEffecti(reverbEffect, AL_EFFECT_TYPE, AL_EFFECT_REVERB);
		alEffectf(reverbEffect, AL_REVERB_DENSITY, reverbDesity);
		alEffectf(reverbEffect, AL_REVERB_DIFFUSION, reverbDiffusion);
		alEffectf(reverbEffect, AL_REVERB_GAIN, reverbGain);
		alEffectf(reverbEffect, AL_REVERB_GAINHF, reverbGainHF);
		alEffectf(reverbEffect, AL_REVERB_DECAY_TIME, reverbDecayTime);
		alEffectf(reverbEffect, AL_REVERB_DECAY_HFRATIO, reverbDecayHFRatio);
		alEffectf(reverbEffect, AL_REVERB_REFLECTIONS_GAIN, reverbReflectionGain);
		alEffectf(reverbEffect, AL_REVERB_REFLECTIONS_DELAY, reverbReflectionDelay);
		alEffectf(reverbEffect, AL_REVERB_LATE_REVERB_GAIN, reverbLateReverbGain);
		alEffectf(reverbEffect, AL_REVERB_LATE_REVERB_DELAY, reverbLateReverbDelay);
		alEffectf(reverbEffect, AL_REVERB_ROOM_ROLLOFF_FACTOR, reverbRoomRolloffFactor);
		alEffecti(reverbEffect, AL_REVERB_DECAY_HFLIMIT, reverbDecayHFLimit);
	}

	void EffectSlot::setSourceEffect(SoundSource *source, bool reverb, bool lowpass){
		ALuint effect = reverb ? effectSlot : AL_EFFECTSLOT_NULL;
		ALuint filter = lowpass ? lowpassFilter : AL_FILTER_NULL;

		alSource3i(source->source, AL_AUXILIARY_SEND_FILTER, effect, 0, filter);
	}

	void EffectSlot::setMusicEffect(MusicBuffer *music, bool reverb, bool lowpass){
		ALuint effect = reverb ? effectSlot : AL_EFFECTSLOT_NULL;
		ALuint filter = lowpass ? lowpassFilter : AL_FILTER_NULL;

		alSource3i(music->source, AL_AUXILIARY_SEND_FILTER, effect, 0, lowpass);
	}

}
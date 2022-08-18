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
		// setup filter
		alGenFilters(1, &lowpassFilter);

		if (!alIsFilter(lowpassFilter) || alGetError() != AL_NO_ERROR){
			throw "failed to create lowpas filter";
		}

		if (alGetError() != AL_NO_ERROR){
			throw "failed to create effect slot and / or effects and filters";
		}

		alFilteri(lowpassFilter, AL_FILTER_TYPE, AL_FILTER_LOWPASS);
		alFilterf(lowpassFilter, AL_LOWPASS_GAIN, lowpassGain);
		alFilterf(lowpassFilter, AL_LOWPASS_GAINHF, lowpassGainHighFreq);

		// setup effect
		alGenAuxiliaryEffectSlots(1, &effectSlot);
		
		if (!alIsAuxiliaryEffectSlot(effectSlot) || alGetError() != AL_NO_ERROR){
			throw "failed to create effect slot";
		}

		alGenEffects(1, &reverbEffect);

		if (!alIsEffect(reverbEffect) || alGetError() != AL_NO_ERROR){
			throw "failed to create reverb effect";
		}

		alEffecti(reverbEffect, AL_EFFECT_TYPE, AL_EFFECT_EAXREVERB);
		EFXEAXREVERBPROPERTIES p;
		EAXREVERBPROPERTIES a = REVERB_PRESET_HANGAR;
		ConvertReverbParameters(&a, &p);
		alEffectf(reverbEffect, AL_EAXREVERB_DENSITY, p.flDensity);
		alEffectf(reverbEffect, AL_EAXREVERB_DIFFUSION, p.flDiffusion);
		alEffectf(reverbEffect, AL_EAXREVERB_GAIN, p.flGain);
		alEffectf(reverbEffect, AL_EAXREVERB_GAINHF, p.flGainHF);
		alEffectf(reverbEffect, AL_EAXREVERB_GAINLF, p.flGainLF);
		alEffectf(reverbEffect, AL_EAXREVERB_DECAY_TIME, p.flDecayTime);
		alEffectf(reverbEffect, AL_EAXREVERB_DECAY_HFRATIO, p.flDecayHFRatio);
		alEffectf(reverbEffect, AL_EAXREVERB_DECAY_LFRATIO, p.flDecayLFRatio);
		alEffectf(reverbEffect, AL_EAXREVERB_REFLECTIONS_GAIN, p.flReflectionsGain);
		alEffectf(reverbEffect, AL_EAXREVERB_REFLECTIONS_DELAY, p.flReflectionsDelay);
		alEffectfv(reverbEffect, AL_EAXREVERB_REFLECTIONS_PAN, p.flReflectionsPan);
		alEffectf(reverbEffect, AL_EAXREVERB_LATE_REVERB_GAIN, p.flLateReverbGain);
		alEffectf(reverbEffect, AL_EAXREVERB_LATE_REVERB_DELAY, p.flLateReverbDelay);
		alEffectfv(reverbEffect, AL_EAXREVERB_LATE_REVERB_PAN, p.flLateReverbPan);
		alEffectf(reverbEffect, AL_EAXREVERB_ECHO_TIME, p.flEchoTime);
		alEffectf(reverbEffect, AL_EAXREVERB_ECHO_DEPTH, p.flEchoDepth);
		alEffectf(reverbEffect, AL_EAXREVERB_MODULATION_TIME, p.flModulationTime);
		alEffectf(reverbEffect, AL_EAXREVERB_MODULATION_DEPTH, p.flModulationDepth);
		alEffectf(reverbEffect, AL_EAXREVERB_AIR_ABSORPTION_GAINHF, p.flAirAbsorptionGainHF);
		alEffectf(reverbEffect, AL_EAXREVERB_HFREFERENCE, p.flHFReference);
		alEffectf(reverbEffect, AL_EAXREVERB_LFREFERENCE, p.flLFReference);
		alEffectf(reverbEffect, AL_EAXREVERB_ROOM_ROLLOFF_FACTOR, p.flRoomRolloffFactor);
		alEffecti(reverbEffect, AL_EAXREVERB_DECAY_HFLIMIT, p.iDecayHFLimit);



		alAuxiliaryEffectSloti(effectSlot, AL_EFFECTSLOT_EFFECT, reverbEffect);
	}

	void EffectSlot::setSourceEffect(SoundSource *source, bool reverb, bool lowpass){
		ALuint effect = reverb ? effectSlot : AL_EFFECTSLOT_NULL;
		ALuint filter = lowpass ? lowpassFilter : AL_FILTER_NULL;

		alSource3i(source->source, AL_AUXILIARY_SEND_FILTER, effect, 0, filter);
	}

	void EffectSlot::setMusicEffect(MusicBuffer *music, bool reverb, bool lowpass){
		ALuint effect = reverb ? effectSlot : AL_EFFECTSLOT_NULL;
		ALuint filter = lowpass ? lowpassFilter : AL_FILTER_NULL;

		alSource3i(music->source, AL_AUXILIARY_SEND_FILTER, effect, 0, filter);
	}

}
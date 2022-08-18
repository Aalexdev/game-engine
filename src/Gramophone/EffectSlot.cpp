#include "Gramophone/EffectSlot.hpp"
#include "OpenAl/Framework.h"
#include "OpenAl/EFX-Util.h"

namespace Gramophone{
	static EAXREVERBPROPERTIES typeToReverb(EffectSlot::Type type){
		switch (type){
			case EffectSlot::Default: return REVERB_PRESET_GENERIC;
			case EffectSlot::Paddedcell: return REVERB_PRESET_PADDEDCELL;
			case EffectSlot::Room: return REVERB_PRESET_ROOM;
			case EffectSlot::Bathroom: return REVERB_PRESET_BATHROOM;
			case EffectSlot::Livingroom: return REVERB_PRESET_LIVINGROOM;
			case EffectSlot::Stoneroomn: return REVERB_PRESET_STONEROOM;
			case EffectSlot::Auditorium: return REVERB_PRESET_AUDITORIUM;
			case EffectSlot::Concerthall: return REVERB_PRESET_CONCERTHALL;
			case EffectSlot::Cave: return REVERB_PRESET_CAVE;
			case EffectSlot::Arena: return REVERB_PRESET_ARENA;
			case EffectSlot::Hangar: return REVERB_PRESET_HANGAR;
			case EffectSlot::Carpettedhallway: return REVERB_PRESET_CARPETTEDHALLWAY;
			case EffectSlot::Hallway: return REVERB_PRESET_HALLWAY;
			case EffectSlot::Forest: return REVERB_PRESET_FOREST;
			case EffectSlot::City: return REVERB_PRESET_CITY;
			case EffectSlot::Mountains: return REVERB_PRESET_MOUNTAINS;
			case EffectSlot::Quarry: return REVERB_PRESET_QUARRY;
			case EffectSlot::Plain: return REVERB_PRESET_PLAIN;
			case EffectSlot::ParkingLot: return REVERB_PRESET_PARKINGLOT;
			case EffectSlot::SewerPipe: return REVERB_PRESET_SEWERPIPE;
			case EffectSlot::Drugged: return REVERB_PRESET_DRUGGED;
			case EffectSlot::Dizzy: return REVERB_PRESET_DIZZY;
			case EffectSlot::Psychotic: return REVERB_PRESET_PSYCHOTIC;
			case EffectSlot::Castle_smallRoomn: return REVERB_PRESET_CASTLE_SMALLROOM;
			case EffectSlot::Castle_shortPassage: return REVERB_PRESET_CASTLE_SHORTPASSAGE;
			case EffectSlot::Castle_mediumRoom: return REVERB_PRESET_CASTLE_MEDIUMROOM;
			case EffectSlot::Castle_longPassage: return REVERB_PRESET_CASTLE_LONGPASSAGE;
			case EffectSlot::Castle_largeRoom: return REVERB_PRESET_CASTLE_LARGEROOM;
			case EffectSlot::Castle_hall: return REVERB_PRESET_CASTLE_HALL;
			case EffectSlot::Castle_cupboard: return REVERB_PRESET_CASTLE_CUPBOARD;
			case EffectSlot::Castle_courtyard: return REVERB_PRESET_CASTLE_COURTYARD;
			case EffectSlot::Castle_alcove: return REVERB_PRESET_CASTLE_ALCOVE;
			case EffectSlot::Factory_alcove: return REVERB_PRESET_FACTORY_ALCOVE;
			case EffectSlot::Factory_shortPassage: return REVERB_PRESET_FACTORY_SHORTPASSAGE;
			case EffectSlot::Factory_mediumRoom: return REVERB_PRESET_FACTORY_MEDIUMROOM;
			case EffectSlot::Factory_longPassage: return REVERB_PRESET_FACTORY_LONGPASSAGE;
			case EffectSlot::Factory_largeRoom: return REVERB_PRESET_FACTORY_LARGEROOM;
			case EffectSlot::Factory_hall: return REVERB_PRESET_FACTORY_HALL;
			case EffectSlot::Factory_cupboard: return REVERB_PRESET_FACTORY_CUPBOARD;
			case EffectSlot::Factory_courtyard: return REVERB_PRESET_FACTORY_COURTYARD;
			case EffectSlot::Factory_smallRoom: return REVERB_PRESET_FACTORY_SMALLROOM;
			case EffectSlot::IcePalace_alcove: return REVERB_PRESET_ICEPALACE_ALCOVE;
			case EffectSlot::IcePalace_shortPassage: return REVERB_PRESET_ICEPALACE_SHORTPASSAGE;
			case EffectSlot::IcePalace_mediumRoom: return REVERB_PRESET_ICEPALACE_MEDIUMROOM;
			case EffectSlot::IcePalace_longPassage: return REVERB_PRESET_ICEPALACE_LONGPASSAGE;
			case EffectSlot::IcePalace_largeRoom: return REVERB_PRESET_ICEPALACE_LARGEROOM;
			case EffectSlot::IcePalace_hall: return REVERB_PRESET_ICEPALACE_HALL;
			case EffectSlot::IcePalace_cupboard: return REVERB_PRESET_ICEPALACE_CUPBOARD;
			case EffectSlot::IcePalace_courtyard: return REVERB_PRESET_ICEPALACE_COURTYARD;
			case EffectSlot::IcePalace_smallRoom: return REVERB_PRESET_ICEPALACE_SMALLROOM;
			case EffectSlot::SpaceStation_alcove: return REVERB_PRESET_SPACESTATION_ALCOVE;
			case EffectSlot::SpaceStation_shortPassage: return REVERB_PRESET_SPACESTATION_SHORTPASSAGE;
			case EffectSlot::SpaceStation_mediumRoom: return REVERB_PRESET_SPACESTATION_MEDIUMROOM;
			case EffectSlot::SpaceStation_longPassage: return REVERB_PRESET_SPACESTATION_LONGPASSAGE;
			case EffectSlot::SpaceStation_largeRoom: return REVERB_PRESET_SPACESTATION_LARGEROOM;
			case EffectSlot::SpaceStation_hall: return REVERB_PRESET_SPACESTATION_HALL;
			case EffectSlot::SpaceStation_cupboard: return REVERB_PRESET_SPACESTATION_CUPBOARD;
			case EffectSlot::SpaceStation_smallRoom: return REVERB_PRESET_SPACESTATION_SMALLROOM;
			case EffectSlot::Wooden_alcove: return REVERB_PRESET_WOODEN_ALCOVE;
			case EffectSlot::Wooden_shortPassage: return REVERB_PRESET_WOODEN_SHORTPASSAGE;
			case EffectSlot::Wooden_mediumRoom: return REVERB_PRESET_WOODEN_MEDIUMROOM;
			case EffectSlot::Wooden_longPassage: return REVERB_PRESET_WOODEN_LONGPASSAGE;
			case EffectSlot::Wooden_largeRoom: return REVERB_PRESET_WOODEN_LARGEROOM;
			case EffectSlot::Wooden_hall: return REVERB_PRESET_WOODEN_HALL;
			case EffectSlot::Wooden_cupboard: return REVERB_PRESET_WOODEN_CUPBOARD;
			case EffectSlot::Wooden_courtyard: return REVERB_PRESET_WOODEN_COURTYARD;
			case EffectSlot::Wooden_smallRoom: return REVERB_PRESET_WOODEN_SMALLROOM;
			case EffectSlot::Sport_emptyStadium: return REVERB_PRESET_SPORT_EMPTYSTADIUM;
			case EffectSlot::Sport_squashCourt: return REVERB_PRESET_SPORT_SQUASHCOURT;
			case EffectSlot::Sport_smallSwimmingPool: return REVERB_PRESET_SPORT_SMALLSWIMMINGPOOL;
			case EffectSlot::Sport_largeSwimmingPool: return REVERB_PRESET_SPORT_LARGESWIMMINGPOOL;
			case EffectSlot::Sport_gymnasium: return REVERB_PRESET_SPORT_GYMNASIUM;
			case EffectSlot::Sport_fullStadium: return REVERB_PRESET_SPORT_FULLSTADIUM;
			case EffectSlot::Sport_stadiumTannoy: return REVERB_PRESET_SPORT_STADIUMTANNOY;
			case EffectSlot::Prefab_workshop: return REVERB_PRESET_PREFAB_WORKSHOP;
			case EffectSlot::Prefab_schoolRoom: return REVERB_PRESET_PREFAB_SCHOOLROOM;
			case EffectSlot::Prefab_practisRoom: return REVERB_PRESET_PREFAB_PRACTISEROOM;
			case EffectSlot::Prefab_outHouse: return REVERB_PRESET_PREFAB_OUTHOUSE;
			case EffectSlot::Prefab_Caravan: return REVERB_PRESET_PREFAB_CARAVAN;
			case EffectSlot::Dome_tomb: return REVERB_PRESET_DOME_TOMB;
			case EffectSlot::Dome_saintPauls: return REVERB_PRESET_DOME_SAINTPAULS;
			case EffectSlot::Pipe_small: return REVERB_PRESET_PIPE_SMALL;
			case EffectSlot::Pipe_longThin: return REVERB_PRESET_PIPE_LONGTHIN;
			case EffectSlot::Pipe_large: return REVERB_PRESET_PIPE_LARGE;
			case EffectSlot::Pipe_resonant: return REVERB_PRESET_PIPE_RESONANT;
			case EffectSlot::Outdoors_backyard: return REVERB_PRESET_OUTDOORS_BACKYARD;
			case EffectSlot::Outdoors_rollingPlains: return REVERB_PRESET_OUTDOORS_ROLLINGPLAINS;
			case EffectSlot::Outdoors_deepCanyon: return REVERB_PRESET_OUTDOORS_DEEPCANYON;
			case EffectSlot::Outdoors_creek: return REVERB_PRESET_OUTDOORS_CREEK;
			case EffectSlot::Outdoors_valley: return REVERB_PRESET_OUTDOORS_VALLEY;
			case EffectSlot::Mood_heaven: return REVERB_PRESET_MOOD_HEAVEN;
			case EffectSlot::Mood_hell: return REVERB_PRESET_MOOD_HELL;
			case EffectSlot::Mood_memory: return REVERB_PRESET_MOOD_MEMORY;
			case EffectSlot::Driving_commentator: return REVERB_PRESET_DRIVING_COMMENTATOR;
			case EffectSlot::Driving_pitGarage: return REVERB_PRESET_DRIVING_PITGARAGE;
			case EffectSlot::Driving_fullGrandStand: return REVERB_PRESET_DRIVING_FULLGRANDSTAND;
			case EffectSlot::Driving_emptyGrandStand: return REVERB_PRESET_DRIVING_EMPTYGRANDSTAND;
			case EffectSlot::Driving_tunnel: return REVERB_PRESET_DRIVING_TUNNEL;
			case EffectSlot::Driving_incarLuxury: return REVERB_PRESET_DRIVING_INCAR_LUXURY;
			case EffectSlot::Driving_incarSports: return REVERB_PRESET_DRIVING_INCAR_SPORTS;
			case EffectSlot::Driving_incarRacer: return REVERB_PRESET_DRIVING_INCAR_RACER;
			case EffectSlot::City_streets: return REVERB_PRESET_CITY_STREETS;
			case EffectSlot::City_subway: return REVERB_PRESET_CITY_SUBWAY;
			case EffectSlot::City_museum: return REVERB_PRESET_CITY_MUSEUM;
			case EffectSlot::City_library: return REVERB_PRESET_CITY_LIBRARY;
			case EffectSlot::City_underPass: return REVERB_PRESET_CITY_UNDERPASS;
			case EffectSlot::City_abandoned: return REVERB_PRESET_CITY_ABANDONED;
			case EffectSlot::DustyRoom: return REVERB_PRESET_DUSTYROOM;
			case EffectSlot::Chapel: return REVERB_PRESET_CHAPEL;
			case EffectSlot::SmallWaterRoom: return REVERB_PRESET_SMALLWATERROOM;
		}
		return REVERB_PRESET_GENERIC;
	}

	void EffectSlot::setReverbeType(Type type){
		EFXEAXREVERBPROPERTIES p;
		EAXREVERBPROPERTIES a = typeToReverb(type);
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
	}


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
		setReverbeType(Default);

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
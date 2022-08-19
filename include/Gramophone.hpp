#pragma once

#ifndef _STDINT_H
	typedef unsigned char uint8_t;
	typedef signed char sint8_t;
	typedef char int8_t;
	typedef unsigned short uint16_t;
	typedef signed short sint16_t;
	typedef short int16_t;
	typedef unsigned int uint32_t;
	typedef signed int sint32_t;
	typedef int int32_t;
	typedef unsigned long long uint64_t;
	typedef signed long long sint64_t;
	typedef long long int64_t;
#endif

namespace Gramophone{
	using Sound = uint64_t;
	using Source = uint64_t;

	enum class ReverbType{
		Default,
		Paddedcell,
		Room,
		Bathroom,
		Livingroom,
		Stoneroomn,
		Auditorium,
		Concerthall,
		Cave,
		Arena,
		Hangar,
		Carpettedhallway,
		Hallway,
		Forest,
		City,
		Mountains,
		Quarry,
		Plain,
		ParkingLot,
		SewerPipe,
		Drugged,
		Dizzy,
		Psychotic,

		Castle_smallRoomn,
		Castle_shortPassage,
		Castle_mediumRoom,
		Castle_longPassage,
		Castle_largeRoom,
		Castle_hall,
		Castle_cupboard,
		Castle_courtyard,
		Castle_alcove,
		
		Factory_alcove,
		Factory_shortPassage,
		Factory_mediumRoom,
		Factory_longPassage,
		Factory_largeRoom,
		Factory_hall,
		Factory_cupboard,
		Factory_courtyard,
		Factory_smallRoom,
		
		IcePalace_alcove,
		IcePalace_shortPassage,
		IcePalace_mediumRoom,
		IcePalace_longPassage,
		IcePalace_largeRoom,
		IcePalace_hall,
		IcePalace_cupboard,
		IcePalace_courtyard,
		IcePalace_smallRoom,

		SpaceStation_alcove,
		SpaceStation_shortPassage,
		SpaceStation_mediumRoom,
		SpaceStation_longPassage,
		SpaceStation_largeRoom,
		SpaceStation_hall,
		SpaceStation_cupboard,
		SpaceStation_smallRoom,

		Wooden_alcove,
		Wooden_shortPassage,
		Wooden_mediumRoom,
		Wooden_longPassage,
		Wooden_largeRoom,
		Wooden_hall,
		Wooden_cupboard,
		Wooden_courtyard,
		Wooden_smallRoom,

		Sport_emptyStadium,
		Sport_squashCourt,
		Sport_smallSwimmingPool,
		Sport_largeSwimmingPool,
		Sport_gymnasium,
		Sport_fullStadium,
		Sport_stadiumTannoy,
		
		Prefab_workshop,
		Prefab_schoolRoom,
		Prefab_practisRoom,
		Prefab_outHouse,
		Prefab_Caravan,

		Dome_tomb,
		Dome_saintPauls,
		
		Pipe_small,
		Pipe_longThin,
		Pipe_large,
		Pipe_resonant,
		
		Outdoors_backyard,
		Outdoors_rollingPlains,
		Outdoors_deepCanyon,
		Outdoors_creek,
		Outdoors_valley,

		Mood_heaven,
		Mood_hell,
		Mood_memory,
		
		Driving_commentator,
		Driving_pitGarage,
		Driving_fullGrandStand,
		Driving_emptyGrandStand,
		Driving_tunnel,
		Driving_incarLuxury,
		Driving_incarSports,
		Driving_incarRacer,

		City_streets,
		City_subway,
		City_museum,
		City_library,
		City_underPass,
		City_abandoned,

		DustyRoom,
		Chapel,
		SmallWaterRoom,
	};

	void initialize();

	void loadMusic(const char *filepath);

	void pauseMusic();

	void playMusic();

	void stopMusic();

	void setMusicPosition(float x, float y, float z);

	// sound effects

	Sound loadSoundEffect(const char *filepath);

	Sound reserveSoundEffect();

	void loadResevedSoundEffect(Sound sound, const char *filepath);

	void destroySoundEffect(Sound sound);

	// ========================= source

	Source createSoundSource(void);

	void destroySoundSource(Source source);

	void setSourcePitch(Source source, float pitch);

	void setSourceGain(Source source, float gain);

	void setSourcePosition(Source source, float x, float y, float z);

	void setSourceVelocity(Source source, float x, float y, float z);

	void setSourceLoop(Source source, bool loop);

	void playSource(Source source, Sound sound);

	// ============== listener

	void setListenerPosition(float x, float y, float z);

	void setListenerOrientation(float atx, float aty, float atz, float upx, float upy, float upz);

	void setListenerGain(float gain);

	void setListenerPitch(float pitch);

	// ============= effects

	void setSoundSourceEffects(Sound sound, bool reverb, bool lowpass);

	void setMusicEffects(bool reverb, bool lowpass);

	void setReverbType(ReverbType type);
}
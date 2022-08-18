#pragma once

#include "OpenAl/al.h"
#include "SoundSource.hpp"
#include "SoundDevice.hpp"
#include "MusicBuffer.hpp"

namespace Gramophone{
	class EffectSlot{
		public:
			enum Type{
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

			~EffectSlot();

			void initialize();

			void setSourceEffect(SoundSource *source, bool reverb, bool lowpass);

			void setMusicEffect(MusicBuffer *music, bool reverb, bool lowpass);

			void setReverbeType(Type type);

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
#include "Gramophone/SoundDevice.hpp"
#include "OpenAl/alc.h"
#include "OpenAl/al.h"
#include "OpenAl/Framework.h"

#include <stdio.h>

namespace Gramophone{
	SoundDevice& SoundDevice::getInstance(){
		static SoundDevice instance;
		return instance;
	}

	SoundDevice::~SoundDevice(){
		alcDestroyContext(context);
	}

	void SoundDevice::setListenerPosition(const glm::vec3 &position){
		alListener3f(AL_POSITION, position.x, position.y, position.z);
	}

	void SoundDevice::setListenerOrientation(const glm::vec3 &at, const glm::vec3 &up){
		float arr[6];
		arr[0] = at.x;
		arr[1] = at.y;
		arr[2] = at.z;
		arr[3] = up.x;
		arr[4] = up.y;
		arr[5] = up.z;
		alListenerfv(AL_ORIENTATION, arr);
	}

	void SoundDevice::setListenerGain(float gain){
		alListenerf(AL_GAIN, gain);
	}

	void SoundDevice::setListenerPitch(float pitch){
		alListenerf(AL_PITCH, pitch);
	}

	void SoundDevice::initialize(){
		SoundDevice& instance = getInstance();
		instance.device = alcOpenDevice(nullptr); // default device

		if (!instance.device){
			throw "failed to get OpenAl sound device";
		}

		instance.context = alcCreateContext(instance.device, nullptr);
		if (!instance.context){
			throw "failed to create OpenAl context";
		}

		if (!alcMakeContextCurrent(instance.context)){
			throw "failed to make the OpenAl context current";
		}
		
		const ALchar *name = nullptr;
		if (alcIsExtensionPresent(instance.device, "ALC_ENUMERATE_ALL_EXT")){
			name = alcGetString(instance.device, ALC_ALL_DEVICES_SPECIFIER);
		}

		if (!name || alcGetError(instance.device) != AL_NO_ERROR){
			name = alcGetString(instance.device, ALC_DEVICE_SPECIFIER);
		}

		printf("Opened : %s\n", name);

		if (!alcIsExtensionPresent(instance.device, (ALCchar*)ALC_EXT_EFX_NAME)){
			throw "cannot support sound effect";
		}

		alGenEffects = (LPALGENEFFECTS)alGetProcAddress("alGenEffects");
		alDeleteEffects = (LPALDELETEEFFECTS )alGetProcAddress("alDeleteEffects");
		alIsEffect = (LPALISEFFECT )alGetProcAddress("alIsEffect");
		alEffecti = (LPALEFFECTI)alGetProcAddress("alEffecti");
		alEffectiv = (LPALEFFECTIV)alGetProcAddress("alEffectiv");
		alEffectf = (LPALEFFECTF)alGetProcAddress("alEffectf");
		alEffectfv = (LPALEFFECTFV)alGetProcAddress("alEffectfv");
		alGetEffecti = (LPALGETEFFECTI)alGetProcAddress("alGetEffecti");
		alGetEffectiv = (LPALGETEFFECTIV)alGetProcAddress("alGetEffectiv");
		alGetEffectf = (LPALGETEFFECTF)alGetProcAddress("alGetEffectf");
		alGetEffectfv = (LPALGETEFFECTFV)alGetProcAddress("alGetEffectfv");
		alGenFilters = (LPALGENFILTERS)alGetProcAddress("alGenFilters");
		alDeleteFilters = (LPALDELETEFILTERS)alGetProcAddress("alDeleteFilters");
		alIsFilter = (LPALISFILTER)alGetProcAddress("alIsFilter");
		alFilteri = (LPALFILTERI)alGetProcAddress("alFilteri");
		alFilteriv = (LPALFILTERIV)alGetProcAddress("alFilteriv");
		alFilterf = (LPALFILTERF)alGetProcAddress("alFilterf");
		alFilterfv = (LPALFILTERFV)alGetProcAddress("alFilterfv");
		alGetFilteri = (LPALGETFILTERI )alGetProcAddress("alGetFilteri");
		alGetFilteriv= (LPALGETFILTERIV )alGetProcAddress("alGetFilteriv");
		alGetFilterf = (LPALGETFILTERF )alGetProcAddress("alGetFilterf");
		alGetFilterfv= (LPALGETFILTERFV )alGetProcAddress("alGetFilterfv");
		alGenAuxiliaryEffectSlots = (LPALGENAUXILIARYEFFECTSLOTS)alGetProcAddress("alGenAuxiliaryEffectSlots");
		alDeleteAuxiliaryEffectSlots = (LPALDELETEAUXILIARYEFFECTSLOTS)alGetProcAddress("alDeleteAuxiliaryEffectSlots");
		alIsAuxiliaryEffectSlot = (LPALISAUXILIARYEFFECTSLOT)alGetProcAddress("alIsAuxiliaryEffectSlot");
		alAuxiliaryEffectSloti = (LPALAUXILIARYEFFECTSLOTI)alGetProcAddress("alAuxiliaryEffectSloti");
		alAuxiliaryEffectSlotiv = (LPALAUXILIARYEFFECTSLOTIV)alGetProcAddress("alAuxiliaryEffectSlotiv");
		alAuxiliaryEffectSlotf = (LPALAUXILIARYEFFECTSLOTF)alGetProcAddress("alAuxiliaryEffectSlotf");
		alAuxiliaryEffectSlotfv = (LPALAUXILIARYEFFECTSLOTFV)alGetProcAddress("alAuxiliaryEffectSlotfv");
		alGetAuxiliaryEffectSloti = (LPALGETAUXILIARYEFFECTSLOTI)alGetProcAddress("alGetAuxiliaryEffectSloti");
		alGetAuxiliaryEffectSlotiv = (LPALGETAUXILIARYEFFECTSLOTIV)alGetProcAddress("alGetAuxiliaryEffectSlotiv");
		alGetAuxiliaryEffectSlotf = (LPALGETAUXILIARYEFFECTSLOTF)alGetProcAddress("alGetAuxiliaryEffectSlotf");
		alGetAuxiliaryEffectSlotfv = (LPALGETAUXILIARYEFFECTSLOTFV)alGetProcAddress("alGetAuxiliaryEffectSlotfv");
	}

}
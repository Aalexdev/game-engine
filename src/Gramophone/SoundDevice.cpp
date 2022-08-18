#include "Gramophone/SoundDevice.hpp"
#include "OpenAl/alc.h"
#include "OpenAl/al.h"

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
	}

}
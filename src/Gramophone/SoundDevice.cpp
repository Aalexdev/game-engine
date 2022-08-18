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
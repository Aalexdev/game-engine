#include "Gramophone/SoundSource.hpp"
#include "OpenAl/EFX-Util.h"
#include "OpenAl/Framework.h"

#include <stdio.h>

namespace Gramophone{
	SoundSource::SoundSource(){
		alGenSources(1, &source);
		alSourcef(source, AL_PITCH, pitch);
		alSourcef(source, AL_GAIN, gain);
		alSource3f(source, AL_POSITION, position.x, position.y, position.z);
		alSource3f(source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
		alSourcei(source, AL_LOOPING, loop);
	}

	SoundSource::~SoundSource(){
		alDeleteSources(1, &source);
	}

	void SoundSource::setPitch(float pitch){
		alSourcef(source, AL_PITCH, pitch);
		this->pitch = pitch;
	}

	void SoundSource::setGain(float gain){
		alSourcef(source, AL_GAIN, gain);
		this->gain = gain;
	}

	void SoundSource::setPosition(const glm::vec3 &position){
		alSource3f(source, AL_POSITION, position.x, position.y, position.z);
		this->position = position;
	}

	void SoundSource::setVelocity(const glm::vec3 &velocity){
		alSource3f(source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
		this->velocity = velocity;
	}

	void SoundSource::setLoop(bool looping){
		alSourcei(source, AL_LOOPING, looping);
		loop = looping;
	}

	void SoundSource::play(SoundEffect *sound){
		if (sound != effect){
			effect = sound;
			alSourcei(source, AL_BUFFER, (ALint)sound->soundEffect);
		}

		alSourcePlay(source);
	}
}
#include "Gramophone/Gramophone.hpp"
#include "Gramophone/SoundDevice.hpp"
#include "Gramophone/SoundEffectLibrary.hpp"

/**
 * @link https://www.youtube.com/watch?v=kWQM1iQ1W0E
 * 
 */

namespace Gramophone{
	struct Core{
		SoundDevice device;
		SoundEffectLibrary soundeffectLibrary;
	};

	Core& getInstance(){
		static Core core;
		return core;
	}

	void initialize(){
		getInstance().device.initialize();
	}

	Sound loadSoundEffect(const char *filepath){
		SoundEffect* sound = new SoundEffect(filepath);
		return getInstance().soundeffectLibrary.push(sound);
	}

	Sound reserveSoundEffect(){
		return getInstance().soundeffectLibrary.reserve();
	}

	void loadResevedSoundEffect(Sound sound, const char *filepath){
		SoundEffect* soundEffect = new SoundEffect(filepath);
		getInstance().soundeffectLibrary.set(sound, soundEffect);
	}

	void destroySoundEffect(Sound sound){
		getInstance().soundeffectLibrary.erase(sound);
	}
}
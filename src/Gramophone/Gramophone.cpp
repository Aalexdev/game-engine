#include "Gramophone/SoundDevice.hpp"
#include "Gramophone/SoundEffectLibrary.hpp"
#include "Gramophone/SoundSourceLibrary.hpp"

#include "Gramophone/Gramophone.hpp"

/**
 * @link https://www.youtube.com/watch?v=kWQM1iQ1W0E
 * 
 */

namespace Gramophone{
	struct Core{
		SoundDevice device;
		SoundEffectLibrary soundeffectLibrary;
		SoundSourceLibrary soundeSourceLibrary;
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

	Source createSoundSource(void){
		SoundSource* source = new SoundSource();
		return getInstance().soundeSourceLibrary.push(source);
	}

	void destroySoundSource(Source source){
		getInstance().soundeSourceLibrary.erase(source);
	}

	void setSourcePitch(Source source, float pitch){
		getInstance().soundeSourceLibrary.get(source)->setPitch(pitch);
	}

	void setSourceGain(Source source, float gain){
		getInstance().soundeSourceLibrary.get(source)->setGain(gain);
	}

	void setSourcePosition(Source source, float x, float y, float z){
		getInstance().soundeSourceLibrary.get(source)->setPosition({x, y, z});
	}

	void setSourceVelocity(Source source, float x, float y, float z){
		getInstance().soundeSourceLibrary.get(source)->setVelocity({x, y, z});
	}

	void setSourceLoop(Source source, bool loop){
		getInstance().soundeSourceLibrary.get(source)->setLoop(loop);
	}

	void playSource(Source source, Sound sound){
		getInstance().soundeSourceLibrary.get(source)->play(getInstance().soundeffectLibrary.get(sound));
	}
}
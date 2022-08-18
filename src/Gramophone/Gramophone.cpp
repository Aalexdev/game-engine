#include "Gramophone/SoundDevice.hpp"
#include "Gramophone/SoundEffectLibrary.hpp"
#include "Gramophone/SoundSourceLibrary.hpp"
#include "Gramophone/MusicBuffer.hpp"
#include "Gramophone/EffectSlot.hpp"

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
		EffectSlot effectSlot;
		MusicBuffer* musicBuffer = nullptr;

		~Core(){
			if (musicBuffer) delete musicBuffer;
		}
	};

	Core& getInstance(){
		static Core core;
		return core;
	}

	void initialize(){
		getInstance().device.initialize();
		getInstance().effectSlot.initialize();
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

	void loadMusic(const char *filepath){
		Core& instance = getInstance();
		if (instance.musicBuffer) delete instance.musicBuffer;
		instance.musicBuffer = new MusicBuffer(filepath);
	}

	void pauseMusic(){
		Core& instance = getInstance();
		if (instance.musicBuffer) instance.musicBuffer->pause();
	}

	void playMusic(){
		Core& instance = getInstance();
		if (instance.musicBuffer) instance.musicBuffer->play();
	}

	void stopMusic(){
		Core& instance = getInstance();
		if (instance.musicBuffer) delete instance.musicBuffer;
	}

	void setMusicPosition(float x, float y, float z){
		Core& instance = getInstance();
		if (instance.musicBuffer) instance.musicBuffer->setPosition({x, y, z});
	}

	void setListenerPosition(float x, float y, float z){
		getInstance().device.setListenerPosition({x, y, z});
	}

	void setListenerOrientation(float atx, float aty, float atz, float upx, float upy, float upz){
		getInstance().device.setListenerOrientation({atx, aty, atz}, {upx, upy, upz});
	}

	void setListenerGain(float gain){
		getInstance().device.setListenerGain(gain);
	}

	void setListenerPitch(float pitch){
		getInstance().device.setListenerPitch(pitch);
	}

	void setSoundSourceEffects(Sound sound, bool reverb, bool lowpass){
		getInstance().effectSlot.setSourceEffect(getInstance().soundeSourceLibrary.get(sound), reverb, lowpass);
	}

	void setMusicEffects(bool reverb, bool lowpass){
		if (getInstance().musicBuffer){
			getInstance().effectSlot.setMusicEffect(getInstance().musicBuffer, reverb, lowpass);
		}
	}

	void setReverbType(ReverbType type){
		getInstance().effectSlot.setReverbeType(static_cast<EffectSlot::Type>(type));
	}
}
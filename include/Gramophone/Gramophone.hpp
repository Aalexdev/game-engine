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
}
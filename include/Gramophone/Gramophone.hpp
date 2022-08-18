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

	void initialize();

	Sound loadSoundEffect(const char *filepath);

	Sound reserveSoundEffect();

	void loadResevedSoundEffect(Sound sound, const char *filepath);

	void destroySoundEffect(Sound sound);
}
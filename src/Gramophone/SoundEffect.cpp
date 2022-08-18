#include "Gramophone/SoundEffect.hpp"
#include "sndfile/sndfile.h"
#include "OpenAl/alext.h"

#include <string>

namespace Gramophone{
	SoundEffect::~SoundEffect(){
		if (soundEffect && alIsBuffer(soundEffect)){
			alDeleteBuffers(1, &soundEffect);
		}
	}

	void SoundEffect::initialize(const char* filepath){
		load(filepath);
	}

	void SoundEffect::load(const char* filepath){
		ALenum err, format;
		SNDFILE* sndfile;
		SF_INFO sfinfo;
		short* membuf;
		sf_count_t numFrames;
		ALsizei numBytes;

		sndfile = sf_open(filepath, SFM_READ, &sfinfo);
		if (!sndfile){
			std::string err = "failed to open \"" + std::string(filepath) + "\" sound file";
			throw err.c_str();
		}

		if (sfinfo.frames < 1 || sfinfo.frames > (sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels){
			std::string err = "bad sample count int \"" + std::string(filepath) + "\" sound file (" + std::to_string(sfinfo.frames) + ")";
			sf_close(sndfile);
			throw err.c_str();
		}

		format = AL_NONE;
		switch (sfinfo.channels){
			case 1: format = AL_FORMAT_MONO16; break;
			case 2: format = AL_FORMAT_STEREO16; break;
			case 3: if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, nullptr, 0) == SF_AMBISONIC_B_FORMAT){format = AL_FORMAT_BFORMAT2D_16;} break;
			case 4: if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, nullptr, 0) == SF_AMBISONIC_B_FORMAT){format = AL_FORMAT_BFORMAT3D_16;} break;
		}

		if (format == AL_NONE){
			std::string err = "unsupported channel count (" + std::to_string(sfinfo.frames) + ")";
			sf_close(sndfile);
			throw err.c_str();
		}

		membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));
		if (!membuf){
			fprintf(stderr, "MALLOC ERROR");
			exit(1);
		}

		numFrames = sf_read_short(sndfile, membuf, sfinfo.frames);
		if (numFrames < 1){
			free(membuf);
			std::string err = "failed to reand samples in \n" + std::string(filepath) + "\n";
			sf_close(sndfile);
			throw err.c_str();
		}

		numBytes = (ALsizei)(numFrames * sfinfo.channels) * (ALsizei)sizeof(short);

		soundEffect = 0;
		alGenBuffers(1, &soundEffect);
		alBufferData(soundEffect, format, membuf, numBytes, sfinfo.samplerate);

		free(membuf);
		sf_close(sndfile);

		err = alGetError();
		if (err != AL_NO_ERROR){
			std::string error = "OpenAL Error : " + std::string(alGetString(err));
			if (soundEffect && alIsBuffer(soundEffect)) alDeleteBuffers(1, &soundEffect);
			throw error.c_str();
		}
	}
}
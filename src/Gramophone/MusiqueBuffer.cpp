#include "Gramophone/MusicBuffer.hpp"
#include "OpenAl/alext.h"

#include <string>

namespace Gramophone{
	MusicBuffer::~MusicBuffer(){
		stop();

		alDeleteSources(1, &source);
		if (sndFile){
			sf_close(sndFile);
		}

		sndFile = nullptr;
		free(membuf);

		alDeleteBuffers(NUM_BUFFERS, buffers);
	}

	void MusicBuffer::play(){
		alSourcePlay(source);
	}

	void MusicBuffer::pause(){
		alSourcePause(source);
	}

	void MusicBuffer::stop(){
		alSourceStop(source);
	}

	void MusicBuffer::setPosition(const glm::vec3 &pos){
		alSource3f(source, AL_POSITION, pos.x, pos.y, pos.z);
	}

	void MusicBuffer::start(){
		ALsizei i;

		alGetError();

		alSourceRewind(source);
		alSourcei(source, AL_BUFFER, 0);

		for (i=0; i<NUM_BUFFERS; i++){
			sf_count_t slen = sf_readf_short(sndFile, membuf, BUFFER_SAMPLES);
			if (slen < 1) break;

			slen *= sfinfo.channels * (sf_count_t)sizeof(short);
			alBufferData(buffers[i], format, membuf, (ALsizei)slen, sfinfo.samplerate);
		}

		if (alGetError() != AL_NO_ERROR){
			throw "Error buffering for playback";
		}

		alSourceQueueBuffers(source, i, buffers);
		alSourcePlay(source);
		if (alGetError() != AL_NO_ERROR){
			throw ("Error starting playback");
		}
	}

	void MusicBuffer::updateBufferStream(){
		ALint processed, state;

		alGetError();

		alGetSourcei(source, AL_SOURCE_STATE, &state);
		alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);

		if (alGetError() != AL_NO_ERROR){
			throw "error checking music source state";
		}
		
		while (processed > 0){
			ALuint bufid;
			sf_count_t slen;

			alSourceUnqueueBuffers(source, 1, &bufid);
			processed--;

			slen = sf_readf_short(sndFile, membuf, BUFFER_SAMPLES);
			if (slen > 0){
				slen *= sfinfo.channels * (sf_count_t)sizeof(short);
				alBufferData(bufid, format, membuf, (ALsizei)slen, sfinfo.samplerate);
				alSourceQueueBuffers(source, 1, &bufid);
			}

			if (alGetError() != AL_NO_ERROR){
				throw "error buffering music data";
			}
		}

		if (state != AL_PLAYING && state != AL_PAUSED){
			ALint queued;

			alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);
			if (queued == 0) return;

			alSourcePlay(source);
			
			if (alGetError() != AL_NO_ERROR){
				throw "error restarting music playback";
			}
		}
	}

	MusicBuffer::MusicBuffer(const char *filepath){
		alGenSources(1, &source);
		alGenBuffers(NUM_BUFFERS, buffers);

		size_t frameSize;

		sndFile = sf_open(filepath, SFM_READ, &sfinfo);
		if (!sndFile){
			throw "failed to open the given music file, check path";
		}

		switch (sfinfo.format){
			case 1: format = AL_FORMAT_MONO16;
			case 2: format = AL_FORMAT_STEREO16;
			case 3: if (sf_command(sndFile, SFC_WAVEX_GET_AMBISONIC, nullptr, 0) == SF_AMBISONIC_B_FORMAT){format = AL_FORMAT_BFORMAT2D_16;} break;
			case 4: if (sf_command(sndFile, SFC_WAVEX_GET_AMBISONIC, nullptr, 0) == SF_AMBISONIC_B_FORMAT){format = AL_FORMAT_BFORMAT3D_16;} break;
		}

		if (format == AL_NONE){
			std::string err = "unsupported channel count (" + std::to_string(sfinfo.frames) + ")";
			sf_close(sndFile);
			sndFile = nullptr;
			throw err.c_str();
		}

		frameSize = ((size_t)BUFFER_SAMPLES * (size_t)sfinfo.channels) * sizeof(short);
		membuf = static_cast<short*>(malloc(frameSize));
	}
}
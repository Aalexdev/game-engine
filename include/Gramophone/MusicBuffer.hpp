#pragma once

#include "OpenAl/al.h"
#include "sndfile/sndfile.h"
#include <glm/glm.hpp>

namespace Gramophone{
	class MusicBuffer{
		friend class EffectSlot;

		static constexpr int BUFFER_SAMPLES = 8192;
		static constexpr int NUM_BUFFERS = 4;

		public:
			~MusicBuffer();
			void play();
			void pause();
			void stop();

			void start();

			void updateBufferStream();

			void setPosition(const glm::vec3 &pos);

			MusicBuffer(const char *filepath);
			MusicBuffer() = delete;

		private:
			ALuint source;
			ALuint buffers[NUM_BUFFERS];
			SNDFILE* sndFile;
			SF_INFO sfinfo;
			short* membuf;
			ALenum format;
			
	};
}
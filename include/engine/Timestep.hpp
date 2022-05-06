#pragma once

#include "core.hpp"

namespace engine{
    class ENGINE_API Timestep{
        public:
            Timestep(float time = 0.f) : time{time} {}

            inline float getSeconds() const {return time;}
            inline float getMilliseconds() const {return time * 1000.f;}

            inline operator float &() {return time;}
        
        private:
            float time;
    };
}
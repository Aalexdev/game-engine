#pragma once

#include "../core.hpp"
#include "Logger.hpp"

namespace engine{

    class ENGINE_API Log{
        public:
            static void Init();

            static inline Ref<Logger>& getCoreLogger() {return s_CoreLogger;}
            static inline Ref<Logger>& getClientLogger() {return s_ClientLogger;}

        private:
            static Ref<Logger> s_CoreLogger;
            static Ref<Logger> s_ClientLogger;
    };
}

#define ENGINE_CORE_CRITICAL(...)   ::engine::Log::getCoreLogger()->fatal(__VA_ARGS__)
#define ENGINE_CORE_ERROR(...)      ::engine::Log::getCoreLogger()->error(__VA_ARGS__)
#define ENGINE_CORE_WARN(...)       ::engine::Log::getCoreLogger()->warn(__VA_ARGS__)
#define ENGINE_CORE_INFO(...)       ::engine::Log::getCoreLogger()->info(__VA_ARGS__)
#define ENGINE_CORE_TRACE(...)      ::engine::Log::getCoreLogger()->trace(__VA_ARGS__)

#define ENGINE_CRITICAL(...)        ::engine::Log::getClientLogger()->fatal(__VA_ARGS__)
#define ENGINE_ERROR(...)           ::engine::Log::getClientLogger()->error(__VA_ARGS__)
#define ENGINE_WARN(...)            ::engine::Log::getClientLogger()->warn(__VA_ARGS__)
#define ENGINE_INFO(...)            ::engine::Log::getClientLogger()->info(__VA_ARGS__)
#define ENGINE_TRACE(...)           ::engine::Log::getClientLogger()->trace(__VA_ARGS__)
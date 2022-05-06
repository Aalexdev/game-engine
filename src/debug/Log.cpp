#include "engine/pch.hpp"
#include "engine/debug/Log.hpp"

namespace engine{
    Ref<Logger> Log::s_CoreLogger = Logger::create("engine");
    Ref<Logger> Log::s_ClientLogger = Logger::create("application");

    void Log::Init(){
        
    }
}
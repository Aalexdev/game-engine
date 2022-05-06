#include "engine/debug/Logger.hpp"
#include "engine/debug/WindowsLogger.hpp"

namespace engine{
    Ref<Logger> Logger::create(const std::string name){
        #if defined(_WIN32)
            return createRef<WindowsLogger>(name);
        #endif
        return nullptr;
    }
}
/**
 * @file Instrumentor.hpp
 * @author TheCherno (https://github.com/TheCherno)
 * @link https://gist.github.com/TheCherno/31f135eea6ee729ab5f26a6908eb3a5e
 */

//
// Basic instrumentation profiler by Cherno

// Usage: include this header file somewhere in your code (eg. precompiled header), and then use like:
//
// Instrumentor::Get().BeginSession("Session Name");        // Begin session 
// {
//     InstrumentationTimer timer("Profiled Scope Name");   // Place code like this in scopes you'd like to include in profiling
//     // Code
// }
// Instrumentor::Get().EndSession();                        // End Session
//
// You will probably want to macro-fy this, to switch on/off easily and use things like __FUNCSIG__ for the profile name.
//

#pragma once

#include "../core.hpp"

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <iostream>

#include <thread>

namespace engine{
    struct ENGINE_API ProfileResult{
        std::string Name;
        long long Start, End;
        uint32_t ThreadID;
    };

    struct ENGINE_API InstrumentationSession{
        std::string Name;
    };

    class ENGINE_API Instrumentor{
        public:
            Instrumentor() : m_CurrentSession(nullptr), m_ProfileCount(0) {}

            void BeginSession(const std::string& name, const std::string& filepath = "results.json");
            void EndSession();
            void WriteProfile(const ProfileResult& result);
            void WriteHeader();
            void WriteFooter();

            void newFrame() {frameCount++;}
            void setFrameTarget(uint64_t frameCount) {frameTarget = frameCount; this->frameCount = 0;}

            static Instrumentor& Get(){
                return instance;
            }
        
        private:
            InstrumentationSession* m_CurrentSession;
            std::ofstream m_OutputStream;
            int m_ProfileCount;

            uint64_t frameCount = 0;
            uint64_t frameTarget = 0;

            static Instrumentor instance;
    };

    class InstrumentationTimer{
        public:
            InstrumentationTimer(const char* name) : m_Name(name), m_Stopped(false){
                m_StartTimepoint = std::chrono::high_resolution_clock::now();
            }

            ~InstrumentationTimer(){
                if (!m_Stopped) Stop();
            }

            void Stop(){
                auto endTimepoint = std::chrono::high_resolution_clock::now();

                long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
                long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

                uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
                Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });

                m_Stopped = true;
            }

        private:
            const char* m_Name;
            std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
            bool m_Stopped;
    };
}

#define COMBINE(x, y) x  ## y
#define LEVEL1(x,y) COMBINE(x,y)

#ifdef ENGINE_PROFILE
    #define ENGINE_PROFILE_BEGIN_SESSION(name, filepath) ::engine::Instrumentor::Get().BeginSession(name, filepath)
    #define ENGINE_PROFILE_END_SESSION() ::engine::Instrumentor::Get().EndSession()
    #define ENGINE_PROFILE_SCOPE(name) LEVEL1(::engine::InstrumentationTimer timer, __LINE__) (name)
    #define ENGINE_PROFILE_FUNCTION() ENGINE_PROFILE_SCOPE(__PRETTY_FUNCTION__)
    #define ENGINE_PROFILE_RECORD() ::engine::Instrumentor::Get().setFrameTarget(UINT64_MAX)
    #define ENGINE_PROFILE_STOP_RECORD() ::engine::Instrumentor::Get().setFrameTarget(0)
    #define ENGINE_PROFILE_RECORD_FRAMES(frameCount) ::engine::Instrumentor::Get().setFrameTarget(frameCount)
    #define ENGINE_PROFILE_UPDATE_FRAME() ::engine::Instrumentor::Get().newFrame()
#else
    #define ENGINE_PROFILE_BEGIN_SESSION(name, filepath)
    #define ENGINE_PROFILE_END_SESSION()
    #define ENGINE_PROFILE_SCOPE(name)
    #define ENGINE_PROFILE_FUNCTION()
    #define ENGINE_PROFILE_RECORD()
    #define ENGINE_PROFILE_STOP_RECORD()
    #define ENGINE_PROFILE_RECORD_FRAMES(frameCount)
    #define ENGINE_PROFILE_UPDATE_FRAME()
#endif
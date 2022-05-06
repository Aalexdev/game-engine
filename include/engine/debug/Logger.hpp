#pragma once

#include "../pch.hpp"
#include "../core.hpp"

namespace engine{
    class ENGINE_API Logger{
        public:
            Logger(const std::string &name) : name{name} {}

            template<class... Args>
            void fatal(Args... args){
                setFatalColor();
                printl(args...);
            }

            template<class... Args>
            void error(Args... args){
                setErrorColor();
                printl(args...);
            }

            template<class... Args>
            void warn(Args... args){
                setWarningColor();
                printl(args...);
            }

            template<class... Args>
            void info(Args... args){
                setInfoColor();
                printl(args...);
            }

            template<class... Args>
            void trace(Args... args){
                setTraceColor();
                printl(args...);
            }

            static Ref<Logger> create(const std::string name);

        private:
            std::string name = "logger";

            template<typename... Args>
            void printl(Args... args){
                int houres, minutes, secondes, millisecs;
                getCurrentTime(houres, minutes, secondes, millisecs);
                std::cout << "[" << houres << ":" << minutes << ":" << secondes << "] : " << name << " : ";
                print(args...);
                std::cout << std::endl;
            }

            template <typename T>
            void print(T t){
                std::cout << t;
            }

            template<typename T, typename... Args>
            void print(T t, Args... args){
                std::cout << t;
                print(args...);
            }

        protected:
            virtual void getCurrentTime(int &houres, int &minutes, int &secondes, int &milisecs) = 0;
            virtual void setTraceColor() = 0;
            virtual void setInfoColor() = 0;
            virtual void setWarningColor() = 0;
            virtual void setErrorColor() = 0;
            virtual void setFatalColor() = 0;
    };
}
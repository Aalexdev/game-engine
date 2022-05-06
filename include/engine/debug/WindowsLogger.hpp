#include "Logger.hpp"
#include "../core.hpp"

// this code will not compile if it's not on a windows device
#ifdef _WIN32

namespace engine{
    class ENGINE_API WindowsLogger : public Logger{
        public:
            WindowsLogger(const std::string &name) : Logger(name){
                hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            }

        private:
            HANDLE hConsole;

            virtual void getCurrentTime(int &houres, int &minutes, int &secondes, int &millisecs) override{
                static SYSTEMTIME time;
                GetLocalTime(&time);
                
                houres = time.wHour;
                minutes = time.wMinute;
                secondes = time.wSecond;
                millisecs = time.wMilliseconds;
            }

            virtual inline void setTraceColor() override {SetConsoleTextAttribute(hConsole, 8);}
            virtual inline void setInfoColor() override {SetConsoleTextAttribute(hConsole, 7);}
            virtual inline void setWarningColor() override {SetConsoleTextAttribute(hConsole, 6);}
            virtual inline void setErrorColor() override {SetConsoleTextAttribute(hConsole, 4);}
            virtual inline void setFatalColor() override {SetConsoleTextAttribute(hConsole, 70);}
    };
}

#endif
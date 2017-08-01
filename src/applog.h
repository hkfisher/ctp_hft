/******************************************************************************
Copyright (c) 2016. All Rights Reserved.

FileName: applog.h
Version: 1.0
Date: 2017.4.25

History:
shengkaishan     2017.4.25   1.0     Create
******************************************************************************/

#ifndef __UTILITY_APPLOG_H__
#define __UTILITY_APPLOG_H__

#include <fstream>
#include <sstream>
#include <string>
#include "singleton.h"
#include <mutex>

#ifdef WIN32
#include <Windows.h>
#else

#endif

using namespace std;
#define APP_LOG(LEVEL) \
    future::applog_finisher() = \
    future::applog_input(LEVEL, __FILE__, __LINE__)

namespace future {

    class applog 
        : public singleton<applog>
    {
    private:
        applog(){};
        virtual ~applog(){};

        friend class singleton<applog>;

    public:
        enum level{
            LOG_TEST = 1,
            LOG_INFO,
            LOG_DEBUG,
            LOG_WARNING,
            LOG_ERROR,
            LOG_CRITICAL
        };

        static void set_log_level(unsigned int loglevel);
        std::string get_time_str();

        void log(level elevel, const std::string& info, const std::string& msg);

    private:
        static int log_level;
        static mutex m_mutex;
    };

    class applog_input
    {    
    public:
        applog_input(applog::level elevel, const char* filename, int line) 
            :m_elevel(elevel)
        {   
            info << filename << ":" << line;
        }

        friend class applog_finisher;

        applog_input& operator<<(char c)                 
        { message_stream << c; return *this; };
        applog_input& operator<<(short s)                
        { message_stream << s; return *this; };
        applog_input& operator<<(int n)                  
        { message_stream << n; return *this; };
        applog_input& operator<<(unsigned int un)        
        { message_stream << un; return *this; };
        applog_input& operator<<(float f)                
        { message_stream << f; return *this; };
        applog_input& operator<<(double d)               
        { message_stream << d; return *this; };
        applog_input& operator<<(long l)                 
        { message_stream << l; return *this; };
        applog_input& operator<<(const char* msg)        
        { message_stream << msg; return *this; };
        applog_input& operator<<(const std::string& msg) 
        { message_stream << msg; return *this; };

    private:
        std::stringstream info;
        std::stringstream message_stream;
        applog::level m_elevel;
    };

    class applog_finisher
    {
    public:
        void operator=(applog_input& input)
        {
            applog::get_instance()->log(input.m_elevel, input.info.str(), 
                input.message_stream.str());
        };
    };

}
#endif


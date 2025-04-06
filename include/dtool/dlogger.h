#pragma once
#include<bits/stdc++.h>
#include"dpath.h"

namespace dag{
namespace tool{
	class Logger{
	public:
		enum Level{
			DEBUG=0,
			INFO,
			WARN,
			ERROR,
			FATAL,
			LEVEL_COUNT
		};
		static Logger *getInstance();
		void open(const std::string&filename);
		void close();
		void lprintf(Level level,const std::string&file,int line,const char*format,...);
		template<typename... Args>
		void lOutPut(Level level,const std::string&file,int line,Args ... x);
		Logger(const Logger&)=delete; // 禁止拷贝构造
		Logger& operator=(const Logger&)=delete; // 禁止赋值操作
	private:
		Logger();
		~Logger();
	private:
		std::string m_filename;
		std::ofstream m_fout;
		static const char *s_level[LEVEL_COUNT];
		static Logger *m_instance;
	};
	#define logDebug(...) (dag::tool::Logger::getInstance()->lOutPut(dag::tool::Logger::DEBUG,dag::tool::getFileName().string(),dag::tool::getLine(),##__VA_ARGS__))
	#define logInfo(...) (dag::tool::Logger::getInstance()->lOutPut(dag::tool::Logger::INFO,dag::tool::getFileName().string(),dag::tool::getLine(),##__VA_ARGS__))
	#define logWarn(...) (dag::tool::Logger::getInstance()->lOutPut(dag::tool::Logger::WARN,dag::tool::getFileName().string(),dag::tool::getLine(),##__VA_ARGS__))
	#define logError(...) (dag::tool::Logger::getInstance()->lOutPut(dag::tool::Logger::ERROR,dag::tool::getFileName().string(),dag::tool::getLine(),##__VA_ARGS__))
	#define logFatal(...) (dag::tool::Logger::getInstance()->lOutPut(dag::tool::Logger::FATAL,dag::tool::getFileName().string(),dag::tool::getLine(),##__VA_ARGS__))
}
}

namespace dag{
namespace tool{
	
	template<typename... Args>
	void Logger::lOutPut(Level level, const std::string&file, int line, Args ... x){
		static time_t tim;
		static struct tm ptm;
		static char timestamp[32];
		tim=std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		localtime_r(&tim,&ptm);//线程安全
		memset(timestamp,0,sizeof(timestamp));
		strftime(timestamp,sizeof(timestamp),"%Y-%m-%d %H:%M:%S",&ptm);
		m_fout<<'['<<timestamp<<']';
		m_fout<<'['<<s_level[level]<<"]["<<file<<':'<<line<<"] ";
		(m_fout<<...<<x);m_fout<<'\n';
		m_fout.flush();
	}
	
}
}
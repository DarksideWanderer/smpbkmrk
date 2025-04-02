#include<bits/stdc++.h>
#include"dlogger.h"

namespace dag{
namespace tool{
	const char* Logger::s_level[LEVEL_COUNT]={
		"DUBUG",
		"INFO",
		"WARN",
		"ERROR",
		"FATAL"
	};

	Logger* Logger::m_instance = nullptr;

	Logger::Logger(){
		
	}
	Logger::~Logger(){
		
	}
	Logger* Logger::getInstance(){
		if(m_instance==nullptr){
			m_instance=new Logger();
		}
		return m_instance;
	}
	void Logger::open(const std::string&filename){
		m_filename=filename;
		m_fout.open(filename,std::ios::app);//追加方式
		if(m_fout.fail()){
			throw std::logic_error("open file failed "+filename);
		}
	}
	void Logger::close(){
		m_fout.close();
	}
	void Logger::lprintf(Level level,const std::string&file,int line,const char *format,...){
		if(m_fout.fail()){
			throw std::logic_error("open file failed "+m_filename);
		}
		//这里只能用 C And Cpp 的方式去获取本地时间
		static time_t tim;
		static struct tm ptm;
		static char timestamp[32];
		tim=std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		localtime_r(&tim,&ptm);//线程安全
		memset(timestamp,0,sizeof(timestamp));
		strftime(timestamp,sizeof(timestamp),"%Y-%m-%d-%H:%M:%S",&ptm);
		//C的方式输出,原理是snprintf
		const char *fmt="[%s-%s][%s:%d] ";
		int size=snprintf(NULL,0,fmt,timestamp,s_level[level],file.c_str(),line);
		if(size>0){
			char *buffer=new char[size+1];
			snprintf(buffer,size+1,fmt,timestamp,s_level[level],file,line);
			buffer[size]='\0';
			m_fout<<buffer;
			delete[] buffer;
		}
		
		va_list arg_ptr;
		va_start(arg_ptr,format);
		size=vsnprintf(NULL,0,format,arg_ptr);
		va_end(arg_ptr);
		if(size>0){
			char *buffer=new char[size+1];
			va_start(arg_ptr,format);
			vsnprintf(buffer,size+1,format,arg_ptr);
			va_end(arg_ptr);
			m_fout<<buffer;
			delete[] buffer;
		}
		m_fout<<'\n';
		m_fout.flush();
	}
}
}
#include<bits/stdc++.h>

#include"application.h"
#include"dtool/dlogger.h"
#include"dtool/dpath.h"



Application &app=*Application::getInstance();

dag::tool::Logger &logger=*dag::tool::Logger::getInstance();

int main(int argc,char *argv[]){
	
	logger.open((dag::tool::getPath()/"mainlog.txt").string());
	
	app.execute(argc,argv);
	return 0;
}
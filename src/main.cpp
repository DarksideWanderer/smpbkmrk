#include<bits/stdc++.h>

#include"application.h"
#include"dtool/dlogger.h"



Application &app=*Application::getInstance();

dag::tool::Logger &logger=*dag::tool::Logger::getInstance();

int main(){
	logger.open("mainlog.txt");
	
	int argc=0;
	char *argv[]={};
	app.execute(argc,argv);
	return 0;
}
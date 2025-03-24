#include<bits/stdc++.h>

#include"smpbkmrk.h"
#include"dtool/dlogger.h"



Smpbkmrk &app=*Smpbkmrk::getInstance();

dag::tool::Logger &logger=*dag::tool::Logger::getInstance();

int main(){
	logger.open("mainlog.txt");
	
	int argc=0;
	char *argv[]={};
	app.execute(argc,argv);
	return 0;
}
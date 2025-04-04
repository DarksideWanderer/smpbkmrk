#include"dtool/dlogger.h"
#include"dscreen/dscreen.h"
#include"dgui/dgui.h"
#include"application.h"
#include"dtool/dbasic.h"



bool TestScreen::init(){
	return true;
}

int TestScreen::cleanUp(){
	return 0;
}

void TestScreen::render(){
	
}

int TestScreen::onEvent(){
	return 0;
}

int TestScreen::execute(int argc,char *argv[]){
	if(!init())return 1;
	
	com.push_back(new Button(app,{100,100,100,100},"Sb",[](){std::cerr<<"Sb";}));
	
	while(running){
		onEvent();
		render();
	}
	
	for(auto t:com)
		delete(t);
	clearContainer(com);
	
	cleanUp();
	return 0;
}
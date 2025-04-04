#include"dtool/dlogger.h"
#include"dscreen/dscreen.h"
#include"dgui/dgui.h"
#include"application.h"



bool TestScreen::init(){
	
}

void TestScreen::cleanUp(){
	
}

void TestScreen::render(){
	
}

void TestScreen::onEvent(){
	
}

int TestScreen::execute{
	if(!init())return false;
	while(running){
		onEvent();
		render();
	}
	cleanUp();
}
#include<bits/stdc++.h>
#include<SDL2/SDL.h>
#include"dtool/dlogger.h"
#include"dscreen/dscreen.h"
#include"dgui/dgui.h"
#include"application.h"
#include"dtool/dbasic.h"



bool TestScreen::init(){
	// com.push_back(new Button(app,{100,100,100,100},"Woc 我是sb 啊",[](){
	// 	static int t;
	// 	std::cout<<++t<<std::endl;
	// }));
	com.push_back(new TextBox(app,{100,100,500,500}));
	return true;
}

int TestScreen::cleanUp(){
	for(auto t:com)
		delete(t);
	clearContainer(com);
	return 0;
}

void TestScreen::render(){
	SDL_SetRenderDrawColor(app->renderer,242,245,250,100);
	SDL_RenderClear(app->renderer);
	for(auto t:com)
		t->render();
		
	SDL_RenderPresent(app->renderer);
}

int TestScreen::onEvent(){
	SDL_Event e;
	if(!SDL_PollEvent(&e))return 0;
	if(e.type==SDL_QUIT){
		running=false;
		app->running = false;
	}
	for(auto t:com)
		t->handleEvent(e);
	return 0;
}

int TestScreen::execute(int argc,char *argv[]){
	if(!init())return 1;
	running=true;
	while(running){
		Uint64 start = SDL_GetPerformanceCounter();
		onEvent();
		render();
		Uint64 end = SDL_GetPerformanceCounter();
		float msecondsElapsed = (end - start) / static_cast<float>(SDL_GetPerformanceFrequency())/1000;
		if(msecondsElapsed<app->flash)SDL_Delay((int)(app->flash-msecondsElapsed));
	}
	cleanUp();
	return 0;
}
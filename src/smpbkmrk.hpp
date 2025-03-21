#pragma once
#include<bits/stdc++.h>
#include<SDL2/SDL.h>
#include"dtool/dlogger.h"

class Smpbkmrk{
private:
	static Smpbkmrk*instance;
	
	SDL_Window*window=nullptr;
	SDL_Renderer*renderer=nullptr;
	
	bool running=true;
	
	int width,height;//基础大小,靠纹理变更放大某些特殊东西
	double alpha;//放大比率
	
private:
	
	
	Smpbkmrk(); // 私有构造函数
	Smpbkmrk(int _height,int _wight);
	~Smpbkmrk();
	
	bool init();
	void cleanUp();
	
	void onEvent();
	void render();
	
	int execute(int argc, char *argv[]);
	
public:
	
	
	Smpbkmrk(const Smpbkmrk&)=delete; // 禁止拷贝构造
	Smpbkmrk& operator=(const Smpbkmrk&)=delete; // 禁止赋值操作
	
	static Smpbkmrk*getInstance();
	
};

bool Smpbkmrk::init(){
	if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_EVENTS|SDL_INIT_VIDEO) < 0) { // 初始化SDL
		return false;
	}
	window = SDL_CreateWindow(
		"Hello SDL world!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		width,height, SDL_WINDOW_ALLOW_HIGHDPI
	); // 创建SDL窗口
	if(window==nullptr) {
		std::cout << "SDL_CreateWindow error: " << SDL_GetError() << std::endl;
		return false;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer==nullptr) {
		std::cout << "SDL_CreateRenderer error: " << SDL_GetError() << std::endl;
		return false;
	}
	return true;
}

Smpbkmrk::Smpbkmrk(int _height,int _width){
	height=_height;width=_width;
	alpha=1;
}
Smpbkmrk::~Smpbkmrk(){
	cleanUp();
}

void Smpbkmrk::cleanUp(){
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}

void Smpbkmrk::onEvent(){//用户交互
	SDL_Event event;
	if(!SDL_PollEvent(&event))return ;
	if (event.type == SDL_QUIT) {
		running = false;
	} 
	else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
		width = event.window.data1;
		height = event.window.data2;
	}
}

void Smpbkmrk::render(){
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

int Smpbkmrk::execute(int argc, char *argv[]) {
	if (!init()) {
		return 1;
	}
	while(running){
		onEvent();
		render();
	}
	cleanUp();
	return 0;
}



Smpbkmrk* Smpbkmrk::getInstance(){
	static Smpbkmrk* instance = new Smpbkmrk(768,1024);
	return instance;
}

#pragma once
#include<bits/stdc++.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#include"dtool/dio.h"

class Button;
class GuiComponent;
class Smpbkmrk{//应用主体
public:
	friend class Button;
	friend class GuiComponent;
	
protected:
	
	SDL_Window*window=nullptr;
	SDL_Renderer*renderer=nullptr;
	TTF_Font* font;
	double fps=60;
	double flash=1000.0/60;
	bool running=true;
	
	int width,height;//基础大小,靠纹理变更放大某些特殊东西
	double alpha;//放大比率
	Smpbkmrk(int _height,int _wight);
	~Smpbkmrk();
	bool init();
	void cleanUp();
	float getDPI();
	void onEvent();
	void render();
	
	std::list<GuiComponent*>Li;
	
public:
	int execute(int argc, char *argv[]);
	Smpbkmrk(const Smpbkmrk&)=delete; // 禁止拷贝构造
	Smpbkmrk& operator=(const Smpbkmrk&)=delete; // 禁止赋值操作
	
	static Smpbkmrk*getInstance();
};

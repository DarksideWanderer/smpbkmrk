#pragma once
#include<bits/stdc++.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>

class Button;
class Component;
class Screen;
class TestScreen;
class Application{//应用主体
public:
	friend class Button;
	friend class Component;
	friend class Screen;
	friend class TestScreen;
	
protected:
	
	SDL_Window*window=nullptr;
	SDL_Renderer*renderer=nullptr;
	TTF_Font* font;
	double fps=60;
	double flash=1000.0/60;
	bool running=true;
	
	int width,height;//基础大小,靠纹理变更放大某些特殊东西
	double alpha;//放大比率
	
	//参数函数
	Application(int _height,int _wight);
	~Application();
	float getDPI();
	
	//过程函数
	bool init();
	void cleanUp();
	
	int index;//目前处于的screen编号
	std::vector<Screen*>scr;
	
public:
	int execute(int argc, char *argv[]);
	Application(const Application&)=delete; // 禁止拷贝构造
	Application& operator=(const Application&)=delete; // 禁止赋值操作
	static Application*getInstance();
};
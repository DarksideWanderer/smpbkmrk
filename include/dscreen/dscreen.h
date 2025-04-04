#pragma once
#include<bits/stdc++.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#include"dtool/dio.h"

class Button;
class Component;
class Application;
struct Screen{
public:
	friend class Button;
	friend class Component;
	friend class Application;
	
protected:
	Application* app;
	//参数函数
	Screen(Application* app):app(app){}
	~Screen()=default;
	
	int running;
	
	//过程函数
	virtual bool init()=0;
	virtual void render()=0;//渲染画面
	virtual int onEvent()=0;//事件处理
	virtual int cleanUp()=0;
	
	int index;//目前处于的screen编号
	std::vector<Component*>scr;
	
public:
	virtual int execute(int argc, char *argv[])=0;
	Screen(const Screen&)=delete; // 禁止拷贝构造
	Screen& operator=(const Screen&)=delete; // 禁止赋值操作
};

struct TestScreen:public Screen{
public:
	friend class Button;
	friend class Component;
	friend class Application;
protected:
	TestScreen(Application*app):Screen(app){}
	~TestScreen()=default;
	
	bool init() override;
	void render() override;
	int onEvent() override;
	int cleanUp() override;
public:
	int execute(int argc,char *argv[])=0;
	
};
#pragma once
#include<bits/stdc++.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#include"dtool/dstring.h"

class Application;
class Component{
public:
	friend class Application;
	friend class Screen;
	friend class TestScreen;
protected:
	Application* app;
	virtual int handleEvent(SDL_Event&e)=0;  // 处理事件
	virtual void render()=0;  // 渲染组件
	Component(Application* app):app(app){}
	~Component()=default;
};

//按钮
class Button : public Component {
public:
	friend class Application;
	friend class Screen;
	friend class TestScreen;
private:
	SDL_Rect rect;//开发范式 以(1080,1920) 为基准开发
	SDL_Color colorr={0,0,255,255};//默认蓝框白字
	SDL_Color colort={0,0,0,255};
	std::string label;
	std::function<void()> onClick;//点击发生的事件,思路是利用友元类的指针
	
	//这里还需要补全样式
	
protected:
	Button(Application*app,SDL_Rect rect,const std::string& text, std::function<void()> onClick);
	~Button()=default;
	int handleEvent(SDL_Event&e) override;
	void render() override;
	void setColorr(SDL_Color _color);
	void setColort(SDL_Color _color);
};

class TextBox : public Component {
	public:
		friend class Application;
		friend class Screen;
		friend class TestScreen;
	private:
		String text;// UTF-8 编码的文本
		SDL_Rect rect;
		SDL_Color bgcr={0,0,255,255};
		SDL_Color txtcr={0,255,0,255};
		SDL_Color bdcr={255,0,0,255};
		bool active;
		
		void render();
		int handleEvent(SDL_Event& e);
		
		TextBox(Application* app, SDL_Rect rect) : Component(app), rect(rect), active(false) {}
		~TextBox() = default;
		
	};
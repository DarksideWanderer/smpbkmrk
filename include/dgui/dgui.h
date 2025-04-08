#pragma once
#include<bits/stdc++.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>

class Application;
class Component{
public:
	friend class Application;
	friend class Screen;
	friend class TestScreen;
protected:
	Application* app;
	virtual int handleEvent(SDL_Event&e)=0;  // 处理事件
	virtual bool render()=0;  // 渲染组件
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
	bool render() override;
	void setColorr(SDL_Color _color);
	void setColort(SDL_Color _color);
};

class TextBox : public Component {//
public:
	friend class Application;
	friend class Screen;
	friend class TestScreen;
	void setcolorr(unsigned char red,unsigned char green,unsigned char blue,unsigned char transparency);
	void setcolort(unsigned char red,unsigned char green,unsigned char blue,unsigned char transparency);
	
private:
	SDL_Rect rect;
	std::string text;
	bool active;
	SDL_Color colorr={0,0,255,255};//默认蓝框白字
	SDL_Color colort={0,0,0,255};

protected:
	TextBox(Application*app,SDL_Rect rect) : Component(app),rect(rect), active(false) {}
	bool render() override;
	int handleEvent(SDL_Event& e) override;
	const std::string& getText() const {return text;}
};
#pragma once
#include<bits/stdc++.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>

class Smpbkmrk;
class GuiComponent{
public:
	friend class Smpbkmrk;
protected:
	Smpbkmrk* app;
	virtual ~GuiComponent()=default;
	virtual int handleEvent(SDL_Event&e)=0;  // 处理事件
	virtual bool render()=0;  // 渲染组件
	GuiComponent(Smpbkmrk* app):app(app){}
};

//按钮
class Button : public GuiComponent {
public:
	friend class Smpbkmrk;
private:
	SDL_Rect rect;//开发范式 以(1080,1920) 为基准开发
	SDL_Color colorr={0,0,255,255};//默认蓝框白字
	SDL_Color colort={0,0,0,255};
	std::string label;
	std::function<void()> onClick;//点击发生的事件,思路是利用友元类的指针
	
	//这里还需要补全样式
	
protected:
	Button(Smpbkmrk*app,SDL_Rect rect,const std::string& text, std::function<void()> onClick);
	bool render() override;
	void setColorr(SDL_Color _color);
	void setColort(SDL_Color _color);
	int handleEvent(SDL_Event&e) override;
};

// class TextBox : public GuiComponent {
// private:
// 	SDL_Rect rect;
// 	std::string text;
// 	bool active;

// public:
// 	TextBox(int x, int y, int w, int h) : rect{ x, y, w, h }, active(false) {}

// 	void render(const SDL_Renderer*renderer) override {
// 		// 渲染文本框
// 		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // 白色框
// 		SDL_RenderFillRect(renderer, &rect);

// 		// 渲染文本（同样可以使用 SDL_ttf 库来渲染文本）
// 		// TODO: 在文本框内显示 text
// 	}

// 	void handleEvent(const SDL_Event& e) override {
// 		if (e.type == SDL_MOUSEBUTTONDOWN) {
// 			if (e.button.x >= rect.x && e.button.x <= rect.x + rect.w &&
// 				e.button.y >= rect.y && e.button.y <= rect.y + rect.h) {
// 				active = true;  // 激活文本框
// 			} 
// 			else {
// 				active = false;  // 失去焦点
// 			}
// 		}
// 		if (e.type == SDL_KEYDOWN && active) {
// 			if (e.key.keysym.sym == SDLK_BACKSPACE && !text.empty()) {
// 				text.pop_back();  // 处理退格键
// 			} 
// 			else if (e.type == SDL_TEXTINPUT && active) {
//                 text += e.text.text;  // SDL_TEXTINPUT 直接返回 UTF-8 字符串
//             }
// 		}
// 	}

// 	const std::string& getText() const {
// 		return text;
// 	}
// };

// 处理所有注册
// class EventDispatcher {
// private:
// 	std::list<GuiComponent*> components;

// public:
// 	void addComponent(GuiComponent* component) {
// 		components.push_back(component);
// 	}

// 	void dispatchEvent(const SDL_Event& e) {
// 		for (auto& component : components) {
// 			component->handleEvent(e);
// 		}
// 	}
// };
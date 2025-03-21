#include<bits/stdc++.h>
#include<SDL2/SDL.h>

class GuiComponent {
public:
	virtual ~GuiComponent() {}
	virtual void render(const SDL_Renderer*renderer) = 0;  // 渲染组件
	virtual void handleEvent(const SDL_Event& e) = 0;  // 处理事件
};

//按钮
class Button : public GuiComponent {
private:
	SDL_Rect rect;
	std::string label;
	std::function<void()> onClick;//

public:
	Button(int x, int y, int w, int h, const std::string& text, std::function<void()> onClick)
		: rect{ x, y, w, h }, label(text), onClick(onClick) {}

	void render(const SDL_Renderer*renderer) override {
		// 渲染按钮
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // 按钮颜色为蓝色
		SDL_RenderFillRect(renderer, &rect);

		// 渲染文本 (简单的文本渲染，实际使用可能需要更复杂的文本渲染库)
		// TODO: 在按钮上渲染文字（可以使用 SDL_ttf 库）
		
		
	}

	void handleEvent(const SDL_Event& e) override {
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			if (e.button.x >= rect.x && e.button.x <= rect.x + rect.w &&
				e.button.y >= rect.y && e.button.y <= rect.y + rect.h) {
				// 按钮被点击
				onClick();
			}
		}
	}
};

class TextBox : public GuiComponent {
private:
	SDL_Rect rect;
	std::string text;
	bool active;

public:
	TextBox(int x, int y, int w, int h) : rect{ x, y, w, h }, active(false) {}

	void render(const SDL_Renderer*renderer) override {
		// 渲染文本框
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // 白色框
		SDL_RenderFillRect(renderer, &rect);

		// 渲染文本（同样可以使用 SDL_ttf 库来渲染文本）
		// TODO: 在文本框内显示 text
	}

	void handleEvent(const SDL_Event& e) override {
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			if (e.button.x >= rect.x && e.button.x <= rect.x + rect.w &&
				e.button.y >= rect.y && e.button.y <= rect.y + rect.h) {
				active = true;  // 激活文本框
			} 
			else {
				active = false;  // 失去焦点
			}
		}
		if (e.type == SDL_KEYDOWN && active) {
			if (e.key.keysym.sym == SDLK_BACKSPACE && !text.empty()) {
				text.pop_back();  // 处理退格键
			} 
			else if (e.type == SDL_TEXTINPUT && active) {
                text += e.text.text;  // SDL_TEXTINPUT 直接返回 UTF-8 字符串
            }
		}
	}

	const std::string& getText() const {
		return text;
	}
};

// 处理所有注册时间
class EventDispatcher {
private:
	std::vector<GuiComponent*> components;

public:
	void addComponent(GuiComponent* component) {
		components.push_back(component);
	}

	void dispatchEvent(const SDL_Event& e) {
		for (auto& component : components) {
			component->handleEvent(e);
		}
	}
};
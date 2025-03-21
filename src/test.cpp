#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>

// UTF-8 删除最后一个字符（处理多字节字符）
void removeLastUtf8Char(std::string& text) {
    if (!text.empty()) {
        while (!text.empty() && (text.back() & 0xC0) == 0x80) {
            text.pop_back(); // 删除 UTF-8 多字节字符的额外字节
        }
        text.pop_back(); // 删除主字节
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("SDL2 中文 + 英文输入",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // 加载字体（确保路径正确）
    TTF_Font* font = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial Unicode.ttf", 36);
    if (!font) {
        std::cerr << "字体加载失败: " << TTF_GetError() << std::endl;
        return -1;
    }

    SDL_StartTextInput(); // 启用 IME 输入

    std::string text; // 存储输入文本
    bool running = true;
    SDL_Event e;
	bool isComposing = false;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            // 输入文本（支持中英文）
            if (e.type == SDL_TEXTEDITING) {
				isComposing = true;  // **正在输入拼音**
			} 
			else if (e.type == SDL_TEXTINPUT) {
				isComposing = false;  // **输入完成**
				text += e.text.text;
			}
			else if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_BACKSPACE) {
					if (!isComposing) {  // **仅在拼音输入完成后删除**
						removeLastUtf8Char(text);
					}
				} else if (e.key.keysym.sym == SDLK_RETURN) {
					text += "\n"; 
				}
			}
        }

        // 清屏
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // 渲染文本
        if (!text.empty()) {
            SDL_Color textColor = {255, 255, 255, 255}; // 白色
            SDL_Surface* textSurface = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), textColor, 600);
            if (textSurface) {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_Rect textRect = {100, 200, textSurface->w, textSurface->h}; // 设定文字位置
                SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

                SDL_FreeSurface(textSurface);
                SDL_DestroyTexture(textTexture);
            }
        }

        SDL_RenderPresent(renderer);
    }

    // 释放资源
    SDL_StopTextInput();
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
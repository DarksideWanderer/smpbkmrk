#include"application.h"
#include"dgui/dgui.h"
#include"dtool/dlogger.h"
#include"dtool/dstring.h"
#include <SDL2/SDL2_rotozoom.h> 

Button::Button(Application*app,SDL_Rect rect,const std::string&text,std::function<void()>onClick)
			: Component(app),rect(rect),label(text), onClick(onClick) {}

// SDL_Surface Scale(SDL_Surface ,double tmp){
// 	SDL_Surface target=SDL_CreateRGBSurface(0, (int)target, tmp, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
// }
SDL_Surface* scaleSurface(SDL_Surface* surface, double scaleX, double scaleY) {
    if(!surface) return NULL;
    return rotozoomSurfaceXY(surface, 0, scaleX, scaleY, 1);
}

void Button::render() {
	SDL_SetRenderDrawColor(app->renderer, colorr.r, colorr.g, colorr.b, colorr.a);
	SDL_Rect rectt=rect;
	rectt.x=rectt.x*app->alpha;
	rectt.y=rectt.y*app->alpha;
	rectt.w=rectt.w*app->alpha;
	rectt.h=rectt.h*app->alpha;
	SDL_RenderDrawRect(app->renderer, &rectt); //绘制框框
	// 渲染文本
	
	double target=rectt.h*0.95/98;
	SDL_Surface* textSurface = TTF_RenderUTF8_Blended_Wrapped(app->font, label.c_str(), colort,0);
	if(textSurface==nullptr){
		logDebug("TTF_RenderUTF8_Blended_Wrapped Error: ",TTF_GetError());
	}
	double real=textSurface->h;
	
	SDL_Surface* tmpSurface;
	tmpSurface=scaleSurface(textSurface,target,target);
	SDL_FreeSurface(textSurface);textSurface=tmpSurface;
	
	//绘制出了一个纹理,现在看纹理放在哪里
	int textw=textSurface->w;
	int texth=textSurface->h;
	
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(app->renderer, textSurface);
	
	SDL_Rect textRect = {rectt.x+(int)((rectt.w-textw)/2.0),rectt.y+(int)((rectt.h-texth)/2.0), textw , texth }; // 设定文字位置,提供一个矩形,自动居中
	SDL_SetRenderDrawColor(app->renderer, colort.r, colort.g, colort.b, colort.a);
	SDL_RenderCopy(app->renderer, textTexture, nullptr, &textRect);

	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
}
void Button::setColorr(SDL_Color _color){colorr=_color;}
void Button::setColort(SDL_Color _color){colort=_color;}
int Button::handleEvent(SDL_Event&e) {
	if(e.type == SDL_MOUSEBUTTONDOWN) {
		if(e.button.x >= rect.x*app->alpha &&e.button.x <= (rect.x + rect.w)*app->alpha &&
			e.button.y >= rect.y*app->alpha && e.button.y <= (rect.y + rect.h)*app->alpha) {
			onClick();
		}
	}
	return true;
}

int TextBox::handleEvent(SDL_Event& e) {
	static bool isComposing=false;
	if(e.type == SDL_MOUSEBUTTONDOWN) {
		if(e.button.x >= rect.x*app->alpha && e.button.x <= (rect.x + rect.w)*app->alpha &&
			e.button.y >= rect.y*app->alpha && e.button.y <= (rect.y + rect.h)*app->alpha) {
			active = true;  // 激活文本框
		} 
		else active = false;  // 失去焦点
	}
	if(active) {
		if(e.type == SDL_TEXTEDITING){
			if (std::string(e.edit.text).empty())isComposing = false; // 如果输入法缓冲区为空
			else isComposing = true;  // 正在输入拼音
		}
		else if(e.type == SDL_TEXTINPUT) {
			isComposing = false;  // 输入完成
			text += e.text.text;
		}
		else if(e.type==SDL_KEYDOWN&&!isComposing){ 
			if(e.key.keysym.sym == SDLK_BACKSPACE)
				text.pop_back();  // 删除最后一个字符
			else if(e.key.keysym.sym == SDLK_RETURN)
				text += "\n"; 
		}
	}
	return 0;
}

void TextBox::render(){
	//缩放
	SDL_Rect rectt={static_cast<int>(rect.x*app->alpha),static_cast<int>(rect.y*app->alpha),
					static_cast<int>(rect.w*app->alpha),static_cast<int>(rect.h*app->alpha)};
	
	//渲染背景
	SDL_SetRenderDrawColor(app->renderer, bgcr.r, bgcr.g, bgcr.b, bgcr.a);
	SDL_RenderFillRect(app->renderer, &rectt);
	
	//渲染边框
	SDL_SetRenderDrawColor(app->renderer, bdcr.r, bdcr.g, bdcr.b, bdcr.a);
	SDL_RenderDrawRect(app->renderer, &rectt);
	
	if(text.empty())return ;
	//渲染文本
	//设置裁剪区域
	SDL_Rect clip = rectt;
	SDL_RenderSetClipRect(app->renderer, &clip);
	//SDL_SetRenderDrawColor(app->renderer, txtcr.r, txtcr.g, txtcr.b, txtcr.a);
	SDL_Surface* textSurface = TTF_RenderUTF8_Blended_Wrapped(app->font, text.c_str(), txtcr,0);
	textSurface == nullptr&&
		(logDebug("TTF_RenderUTF8_Blended_Wrapped Error: ", TTF_GetError()),std::exit(1),0);
	
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(app->renderer, textSurface);
	textTexture == nullptr&&
		(logDebug("SDL_CreateTextureFromSurface Error: ", SDL_GetError()),std::exit(1),0);
	
	SDL_Rect textrext={rectt.x,rectt.y,textSurface->w,textSurface->h};
	SDL_RenderCopy(app->renderer,textTexture,nullptr,&textrext);
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
	SDL_RenderSetClipRect(app->renderer, nullptr);
}
#include"application.h"
#include"dgui/dgui.h"
#include"dtool/dlogger.h"
#include <SDL2/SDL2_rotozoom.h> 

Button::Button(Application*app,SDL_Rect rect,const std::string&text,std::function<void()>onClick)
			: Component(app),rect(rect),label(text), onClick(onClick) {}

// SDL_Surface Scale(SDL_Surface ,double tmp){
// 	SDL_Surface target=SDL_CreateRGBSurface(0, (int)target, tmp, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
// }
SDL_Surface* scaleSurface(SDL_Surface* surface, double scaleX, double scaleY) {
    if (!surface) return NULL;
    return rotozoomSurfaceXY(surface, 0, scaleX, scaleY, 1);
}

bool Button::render() {
	//渲染按钮
	SDL_SetRenderDrawColor(app->renderer, colorr.r, colorr.g, colorr.b, colorr.a);
	SDL_Rect rectt=rect;
	rectt.x=rectt.x*app->alpha;
	rectt.y=rectt.y*app->alpha;
	rectt.w=rectt.w*app->alpha;
	rectt.h=rectt.h*app->alpha;
	SDL_RenderDrawRect(app->renderer, &rectt); //绘制框框
	// 渲染文本
	
	double target=rectt.h*0.95/98;
	SDL_Surface* textSurface = TTF_RenderUTF8_Blended_Wrapped(app->font, label.c_str(), colort,0*rectt.w/target);
	if(textSurface==nullptr){
		logDebug("TTF_RenderUTF8_Blended_Wrapped Error: ",TTF_GetError());
		return false;
	}
	double real=textSurface->h;
	std::cout<<real<<' '<<target<<std::endl;
	
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
	return true;
}
void Button::setColorr(SDL_Color _color){colorr=_color;}
void Button::setColort(SDL_Color _color){colort=_color;}
int Button::handleEvent(SDL_Event&e) {
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		if (e.button.x >= rect.x*app->alpha &&e.button.x <= (rect.x + rect.w)*app->alpha &&
			e.button.y >= rect.y*app->alpha && e.button.y <= (rect.y + rect.h)*app->alpha) {
			onClick();
		}
	}
	return true;
}
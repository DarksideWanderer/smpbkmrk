#include"dtool/dlogger.h"
#include"smpbkmrk.h"
#include"dgui/dgui.h"

using dag::tool::Logger;

float Smpbkmrk::getDPI() {
	int displayIndex=SDL_GetWindowDisplayIndex(window);
	
	float ddpi, hdpi, vdpi;
	if (SDL_GetDisplayDPI(displayIndex, &ddpi, &hdpi, &vdpi) != 0) {
		logDebug("Failed to get DPI: ",SDL_GetError());
		return -1;
	}
	return ddpi;
}

bool Smpbkmrk::init(){
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)return false;
	if(TTF_Init()<0)return false;
	window = SDL_CreateWindow(
	"Hello SDL world!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		width,height, SDL_WINDOW_ALLOW_HIGHDPI
	); // 创建SDL窗口
	if(window==nullptr) {
		logDebug("SDL_CreateWindow error: " , SDL_GetError());
		return false;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer==nullptr) {
		logDebug("SDL_CreateRenderer error: ",SDL_GetError());
		return false;
	}
	if (SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1") == -1) {
		logDebug("SDL_SetHint: ",SDL_GetError());
	}
	//字体默认大小为36,渲染字体时优化纹理
	font = TTF_OpenFont("../resource/Arial Unicode.ttf",36);
	if(font==nullptr){
		logDebug("TTF_OpenFont error: ",TTF_GetError());
		return false;
	}
	SDL_StartTextInput();
	return true;
}
Smpbkmrk::Smpbkmrk(int _width,int _height){
	alpha=0.25;
	//窗口缩放倍率,默认为1/4
	height=static_cast<int>(alpha*_height);width=static_cast<int>(alpha*_width);
	//窗口分辨率,默认为(1080,1920)
}
Smpbkmrk::~Smpbkmrk(){
	cleanUp();
}

void Smpbkmrk::cleanUp(){
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}

void Smpbkmrk::onEvent(){//用户交互
	SDL_Event event;
	if(!SDL_PollEvent(&event))return ;
	if (event.type == SDL_QUIT) {
		running = false;
	} 
	for(auto t:Li)
		t->handleEvent(event);
	// 没有写窗口大小变化的逻辑
}

void Smpbkmrk::render(){
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);
	for(auto t:Li)
		t->render();
	SDL_RenderPresent(renderer);
}

int Smpbkmrk::execute(int argc, char *argv[]) {
	if (!init()) {
		return 1;
	}
	
	Button A(getInstance(),{100,100,400,100},"宁神启动",[](){printf("Sb\n");});
	Li.push_back(&A);
	
	while(running){
		Uint64 start = SDL_GetPerformanceCounter();
		onEvent();
		render();
		Uint64 end = SDL_GetPerformanceCounter();
		float msecondsElapsed = (end - start) / static_cast<float>(SDL_GetPerformanceFrequency())/1000;
		if(msecondsElapsed<flash)SDL_Delay((int)(flash-msecondsElapsed));
	}
	cleanUp();
	return 0;
}

Smpbkmrk* Smpbkmrk::getInstance(){
	static Smpbkmrk* instance = new Smpbkmrk(1080,1920);
	return instance;
}

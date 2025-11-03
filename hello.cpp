#include <iostream>
#include <SDL2/SDL.h>

// 初始化窗口的长宽
const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char *argv[]) {
	SDL_Window *window = nullptr;
	SDL_Surface *screenSurface = nullptr;

	// 初始化SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	// 创建窗口
	window = SDL_CreateWindow("Hello SDL2 Window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN // 窗口创建后立即显示
	);

	if (window == nullptr) {
		std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	// 获取窗口表面
	screenSurface = SDL_GetWindowSurface(window);

	// 填充表面为白色
	SDL_FillRect(screenSurface, nullptr,
		SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF) // SDL_MapRGB 需要一个颜色结构体
	);

	// 更新窗口表面以显示更改
	SDL_UpdateWindowSurface(window);

	// 等待两秒钟
	SDL_Delay(2000);

	// 事件循环: 保持窗口打开直到用户关闭它
	SDL_Event e;
	bool quit = false;
	while (quit == false) {
		while (SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				quit = true;
			}
		}
	}

	// 清理并退出
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

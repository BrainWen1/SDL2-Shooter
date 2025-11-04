#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

int main(int, char**) {
    // 初始化SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // 创建一个窗口
    SDL_Window *window = SDL_CreateWindow("Shooter",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, // 窗口位置
                                          800, 950, // 窗口尺寸
                                          SDL_WINDOW_SHOWN); // 窗口标志
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // 创建渲染器
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // 初始化SDL_image
    if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // 加载图像
    SDL_Texture *texture = IMG_LoadTexture(renderer, "../assets/image/medium-A.png");
    if (texture == nullptr) {
        std::cerr << "IMG_LoadTexture Error: " << IMG_GetError() << std::endl;
        // 打印实际路径，确认程序读取的路径是否正确
        std::cerr << "Wrong image: " << "../assets/image/bg.png" << std::endl;
        // 继续执行，即使图像加载失败
    }

    // 初始化SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;
        // 继续执行，即使音频初始化失败
    }

    // 加载音频
    Mix_Music *bgm = Mix_LoadMUS("../assets/music/03_Racing_Through_Asteroids_Loop.ogg");
    if (bgm == nullptr) {
        std::cerr << "Mix_LoadMUS Error: " << Mix_GetError() << std::endl;
        // 继续执行，即使音频加载失败
    } else {
        Mix_PlayMusic(bgm, -1); // 循环播放背景音乐
    }

    // 初始化SDL_ttf
    if (TTF_Init() != 0) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        // 继续执行，即使字体初始化失败
    }

    TTF_Font *font = TTF_OpenFont("../assets/font/VonwaonBitmap-16px.ttf", 24); // 路径 字体大小
    if (font == nullptr) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
            // 继续执行，即使字体加载失败
    }
    SDL_Color textColor = { 255, 0, 255, 255 }; // 紫色文字
    SDL_Surface *textSurface = TTF_RenderUTF8_Solid(font, "Hello, SDL_ttf! 你好你好", textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    // Do some rendering here...
    while (true) {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) { // 用户请求关闭窗口
                break;
            }
        }

        // 清屏为黑色
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 白色背景
        // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 黑色背景
        SDL_RenderClear(renderer); // 清除渲染器

        // 渲染矩形
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // 渲染器颜色设为红色
        
        SDL_Rect rect = { 100, 100, 200, 200 }; // 定义一个矩形
        SDL_RenderFillRect(renderer, &rect); // 绘制填充矩形

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 换回白色
        // 渲染图像纹理
        if (texture != nullptr) {
            SDL_Rect disrect = { 200, 200, 300, 300 }; // 定义图像显示位置和大小
            SDL_RenderCopy(renderer, texture, nullptr, &disrect); // 复制纹理到渲染器
        }

        // 渲染文字纹理
        if (textTexture != nullptr) {
            SDL_Rect textRect = { 250, 600, textSurface->w, textSurface->h }; // 定义文字显示位置和大小
            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect); // 复制纹理到渲染器
        }

        // 显示渲染内容
        SDL_RenderPresent(renderer); // 显示渲染器内容
    }

    // ending

    // 清理图片资源并退出SDL_image
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
    }
    IMG_Quit();

    // 清理音频资源并退出SDL_mixer
    if (bgm != nullptr) {
        Mix_FreeMusic(bgm);
    }
    Mix_CloseAudio();
    Mix_Quit();

    // 清理字体资源并退出SDL_ttf
    if (textTexture != nullptr) {
        SDL_DestroyTexture(textTexture);
    }
    if (textSurface != nullptr) {
        SDL_FreeSurface(textSurface);
    }
    if (font != nullptr) {
        TTF_CloseFont(font);
    }
    TTF_Quit();

    // 清理并退出
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
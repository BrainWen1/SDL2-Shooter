#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

int main(int argc, char *argv[]) {
    // 初始化SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // 创建一个窗口
    SDL_Window *window = SDL_CreateWindow("Shooter",
                                          100, 100, // 窗口位置
                                          800, 600, // 窗口尺寸
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
    if(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != (IMG_INIT_PNG | IMG_INIT_JPG)) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // 加载图像
    SDL_Texture *texture = IMG_LoadTexture(renderer, "../../assets/image/medium-A.png");
    if (texture == nullptr) {
        std::cerr << "IMG_LoadTexture Error: " << SDL_GetError() << std::endl;
        // 打印实际路径，确认程序读取的路径是否正确
        std::cerr << "Wrong image: " << "../../assets/image/bg.png" << std::endl;
        // 继续执行，即使图像加载失败
    }

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
        SDL_Rect rect = { 100, 100, 200, 200 }; // 定义一个矩形
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // 渲染器颜色设为红色
        SDL_RenderFillRect(renderer, &rect); // 绘制填充矩形
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 白色背景

        // 渲染图像纹理
        if (texture != nullptr) {
            SDL_Rect disrect = { 200, 200, 300, 300 }; // 定义图像显示位置和大小
            SDL_RenderCopy(renderer, texture, nullptr, &disrect); // 复制纹理到渲染器
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

    // 清理并退出
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
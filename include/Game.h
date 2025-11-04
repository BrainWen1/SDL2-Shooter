#ifndef GAME_H
#define GAME_H

#include "Screen.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Game {
public:
    static Game &getInstance() {
        static Game instance;
        return instance;
    }
    ~Game();

    SDL_Window *getWindow() { return window; }
    SDL_Renderer *getRenderer() { return renderer; }

    void run(); // 运行游戏
    void init(); // 初始化游戏
    void clean(); // 清理资源

    void changeScreen(Screen* newScreen); // 切换屏幕

    void handleEvent(SDL_Event*);
    void update();
    void render();

    // 获取窗口尺寸
    int getScreenWidth() const { return screenWidth; }
    int getScreenHeight() const { return screenHeight; }

private:
    bool isRunning = true; // 游戏是否在运行
    Screen* currentScreen = nullptr; // 当前屏幕
    SDL_Window* window = nullptr; // 游戏窗口
    SDL_Renderer* renderer = nullptr; // 渲染器

    int screenWidth = 600; // 窗口宽度
    int screenHeight = 800; // 窗口高度

    // 单例模式，禁止拷贝和赋值
    Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
};

#endif // GAME_H
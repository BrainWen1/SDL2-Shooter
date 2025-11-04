#include "Game.h"
#include "ScreenMain.h"

Game::Game() : isRunning(true) { // 构造函数
}

Game::~Game() { // 析构函数
    clean(); // 清理资源
}

void Game::init() { // 初始化游戏

    // 创建帧率
    frameTime = 1000 / FPS;

    // 初始化SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Init Error: %s", SDL_GetError()); // 报告错误信息
        isRunning = false;
        return;
    }

    // 创建窗口：窗口要会经常使用，所以放在Game类中管理
    window = SDL_CreateWindow("Game",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, // 窗口初始位置在屏幕中央
                              screenWidth, screenHeight, // 窗口尺寸
                              SDL_WINDOW_SHOWN); // 窗口标志
    if (window == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateWindow Error: %s", SDL_GetError());
        isRunning = false;
        return;
    }

    // 创建渲染器
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // 硬件加速
    if (renderer == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateRenderer Error: %s", SDL_GetError());
        isRunning = false;
        return;
    }

    // 初始化第一个屏幕
    // 这里可以根据需要切换不同的屏幕
    currentScreen = new ScreenMain();
    currentScreen->init(); // 初始化主屏幕

    // 初始化SDL_image
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "IMG_Init Error: %s", IMG_GetError());
        isRunning = false;
    }
}

void Game::run() {

    // 运行游戏主循环
    while (isRunning) {
        auto frameStart = SDL_GetTicks(); // 帧开始时间

        SDL_Event event;
        handleEvent(&event); // 处理事件

        update(); // 更新游戏状态

        render(); // 渲染游戏画面

        auto frameEnd = SDL_GetTicks(); // 帧结束时间        
        // 计算帧时间并延时以维持稳定帧率
        auto diff = frameEnd - frameStart;
        if (diff < frameTime) {
            SDL_Delay(frameTime - diff); // 延时以维持稳定帧率
            deltaTime = frameTime / 1000.0f; // 转换为秒
        } else { // 如果帧时间超过预期，直接使用实际帧时间
            deltaTime = diff / 1000.0f; // 实际帧时间
        }
    }
}

void Game::clean() { // 清理资源

    // 清理当前屏幕
    if (currentScreen != nullptr) {
        currentScreen->clean();
        delete currentScreen;
        currentScreen = nullptr;
    }

    // 退出SDL_image
    IMG_Quit();

    // 销毁渲染器和窗口
    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window != nullptr) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    // 退出SDL
    SDL_Quit();
}

void Game::changeScreen(Screen* newScreen) { // 切换屏幕

    // 清理当前屏幕
    if (currentScreen != nullptr) {
        currentScreen->clean();
        delete currentScreen;
    }

    // 切换到新屏幕并初始化
    currentScreen = newScreen;
    if (currentScreen != nullptr) {
        currentScreen->init();
    }
}

void Game::handleEvent(SDL_Event* event) {

    while (SDL_PollEvent(event)) { // 轮询事件
        if (event->type == SDL_QUIT) { // 处理退出事件
           isRunning = false;
        }
        currentScreen->handleEvents(event);
    }
}

void Game::update() {

    if (currentScreen != nullptr) {
        currentScreen->update(this->deltaTime);
    }
}

void Game::render() {

    // 清屏
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 黑色背景
    SDL_RenderClear(renderer);

    // 具体渲染操作
    currentScreen->render();

    // 显示渲染内容
    SDL_RenderPresent(renderer);
}
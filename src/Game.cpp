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

    // 初始化SDL_image（在加载任何纹理之前）
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "IMG_Init Error: %s", IMG_GetError());
        isRunning = false;
    }

    // 初始化SDL_mixer（在加载任何音乐之前）
    if (Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3) != (MIX_INIT_OGG | MIX_INIT_MP3)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Mix_Init Error: %s", Mix_GetError());
        isRunning = false;
    }

    // 打开音频设备
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Mix_OpenAudio Error: %s", Mix_GetError());
        isRunning = false;
    }
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4); // 设置背景音乐音量为最大音量的1/4

    Mix_AllocateChannels(32); // 分配32个混音通道，用于音效播放
    Mix_Volume(-1, MIX_MAX_VOLUME / 6); // 设置所有音效通道的音量为最大音量的1/2

    // 初始化背景图
    nearStars.texture = IMG_LoadTexture(renderer, "../assets/image/Stars-A.png");
    farStars.texture = IMG_LoadTexture(renderer, "../assets/image/Stars-B.png");
    SDL_QueryTexture(nearStars.texture, nullptr, nullptr, &nearStars.width, &nearStars.height);
    SDL_QueryTexture(farStars.texture, nullptr, nullptr, &farStars.width, &farStars.height);
    farStars.speed = 20; // 远星背景速度较慢
    nearStars.width /= (7 / 4); nearStars.height /= (7 / 4); // 缩放近星背景尺寸
    farStars.width /= (7 / 4); farStars.height /= (7 / 4); // 缩放远星背景尺寸

    // 初始化第一个屏幕（在图像/音频子系统初始化之后）
    // 这里可以根据需要切换不同的屏幕
    currentScreen = new ScreenMain();
    currentScreen->init(); // 初始化主屏幕
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

    // 清理背景图
    if (nearStars.texture != nullptr) {
        SDL_DestroyTexture(nearStars.texture);
        nearStars.texture = nullptr;
    }
    if (farStars.texture != nullptr) {
        SDL_DestroyTexture(farStars.texture);
        farStars.texture = nullptr;
    }

    // 清理当前屏幕
    if (currentScreen != nullptr) {
        currentScreen->clean();
        delete currentScreen;
        currentScreen = nullptr;
    }

    // 退出SDL_image
    IMG_Quit();

    // 退出SDL_mixer
    Mix_CloseAudio();
    Mix_Quit();

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

    updateBackgrounds(this->deltaTime); // 更新背景图

    if (currentScreen != nullptr) {
        currentScreen->update(this->deltaTime);
    }
}

void Game::render() {

    // 清屏
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 黑色背景
    SDL_RenderClear(renderer);

    // 渲染背景图
    renderBackgrounds();

    // 具体渲染操作
    currentScreen->render();

    // 显示渲染内容
    SDL_RenderPresent(renderer);
}

void Game::updateBackgrounds(float deltaTime) { // 更新背景图

    nearStars.offsetY += nearStars.speed * deltaTime; // 更新近星背景偏移
    if (nearStars.offsetY >= 0) {
        nearStars.offsetY = -nearStars.height; // 重置偏移，实现循环滚动
    }

    farStars.offsetY += farStars.speed * deltaTime; // 更新远星背景偏移
    if (farStars.offsetY >= 0) {
        farStars.offsetY = -farStars.height; // 重置偏移，实现循环滚动
    }
}

void Game::renderBackgrounds() { // 渲染背景图

    // 渲染远星背景
    for (int y = static_cast<int>(farStars.offsetY); y < screenHeight; y += farStars.height) {
        for (int x = 0; x < screenWidth; x += farStars.width) {
            SDL_Rect destRect = {x, y, farStars.width, farStars.height};
            SDL_RenderCopy(renderer, farStars.texture, nullptr, &destRect);
        }
    }

    // 渲染近星背景
    for (int y = static_cast<int>(nearStars.offsetY); y < screenHeight; y += nearStars.height) {
        for (int x = 0; x < screenWidth; x += nearStars.width) {
            SDL_Rect destRect = {x, y, nearStars.width, nearStars.height};
            SDL_RenderCopy(renderer, nearStars.texture, nullptr, &destRect);
        }
    }
}
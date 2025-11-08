#include "Title.h"
#include "Game.h"
#include "ScreenMain.h"

Title::Title() {
    // 构造函数
}

Title::~Title() {
    // 析构函数
}

void Title::init() { // 初始化标题屏幕

    // 播放背景音乐
    bgm = Mix_LoadMUS("../assets/music/homepage.ogg");
    if (bgm != nullptr) {
        Mix_PlayMusic(bgm, -1); // 循环播放背景音乐
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Mix_LoadMUS Error: %s", Mix_GetError());
    }
}

void Title::clean() { // 清理标题屏幕

    // 清理音乐资源
    if (bgm != nullptr) {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
        bgm = nullptr;
    }
}

void Title::update(float deltaTime) { // 更新标题屏幕
    
    // 文字闪烁：控制一个周期为2秒
    timer += deltaTime;
    if (timer >= flashInterval * 2.0f) {
        timer = 0.0f;
    }
}

void Title::render() { // 渲染标题屏幕
    
    // 渲染标题文字
    std::string title = "SDL2 Shooter";
    game.renderTextCentered(title, 0.3f, true);

    // 渲染提示文字
    if (timer <= flashInterval) { // 文字闪烁效果
        std::string prompt = "Press ENTER/SPACE to Start";
        game.renderTextCentered(prompt, 0.7f, false);
    }
}

void Title::handleEvents(SDL_Event* event) { // 处理标题屏幕的事件
    
    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.scancode == SDL_SCANCODE_RETURN || event->key.keysym.scancode == SDL_SCANCODE_SPACE) {
            // 切换到主屏幕
            auto newScreen = new ScreenMain();
            game.changeScreen(newScreen);
        }
    }
}
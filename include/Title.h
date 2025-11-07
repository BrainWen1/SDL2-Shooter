#ifndef TITLE_H
#define TITLE_H

#include "Screen.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <string>

// 标题屏幕类，负责游戏的标题界面
class Title : public Screen {
public:
    Title();
    ~Title();

    virtual void init() override;
    virtual void clean() override;
    virtual void update(float deltaTime) override;
    virtual void render() override;
    virtual void handleEvents(SDL_Event* event) override;

private:
    Mix_Music *bgm = nullptr; // 背景音乐

    float timer = 0.0f; // 计时器
    float flashInterval = 0.75f; // 文字闪烁间隔时间
};

#endif // TITLE_H
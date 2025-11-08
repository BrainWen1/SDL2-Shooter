#ifndef ENDING_H
#define ENDING_H

#include "Screen.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>

class Ending : public Screen {
public:
    Ending() = default;
    virtual ~Ending() = default;

    virtual void init() override;
    virtual void clean() override;
    virtual void update(float deltaTime) override;
    virtual void render() override;
    virtual void handleEvents(SDL_Event* event) override;

    // 渲染不同阶段
    void renderPhase1();
    void renderPhase2();

private:
    bool isTying = true; // 是否正在输入名字
    std::string Name; // 玩家输入的名字

    float timer = 0.0f; // 计时器
    const float flashInterval = 0.5f; // 闪烁间隔

    Mix_Music* bgm = nullptr; // 背景音乐
};

#endif // ENDING_H
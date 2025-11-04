#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>

// 抽象基类 Screen，定义屏幕的基本接口
class Screen {
public:
    Screen() = default;
    ~Screen() = default;

    // 纯虚函数，派生类必须实现这些方法
    virtual void init() = 0;
    virtual void clean() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void handleEvents(SDL_Event*) = 0;

private:
};

#endif // SCREEN_H
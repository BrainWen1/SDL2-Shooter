#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>

class Game; // 前向声明，避免头文件循环依赖

// 抽象基类 Screen，定义屏幕的基本接口
class Screen {
public:
    explicit Screen(); // 构造函数
    virtual ~Screen() = default; // 虚析构函数，确保派生类正确析构基类部分

    // 纯虚函数，派生类必须实现这些方法
    virtual void init() = 0;
    virtual void clean() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void handleEvents(SDL_Event*) = 0;

protected:
    Game &game; // 游戏实例引用，由派生类在构造时注入
};

#endif // SCREEN_H
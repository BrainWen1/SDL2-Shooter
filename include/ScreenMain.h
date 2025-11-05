#ifndef SCREEN_MAIN_H
#define SCREEN_MAIN_H

#include "Screen.h"
#include "Obj.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <list>

// 前向声明，避免头文件循环依赖
class Game;

// 主屏幕类，负责游戏的主界面
class ScreenMain : public Screen {
public:
    ScreenMain();
    ~ScreenMain();

    void init() override;
    void clean() override;
    void update(float deltaTime) override;
    void render() override;
    void handleEvents(SDL_Event* event) override;

    void keyboardControls(float deltaTime);

    void PlayerShoot();
    void EnemyShoot();

    void updatePlayerProjectiles(float deltaTime);
    void renderPlayerProjectiles();

private:
    // 主屏幕特有的成员变量
    Player player; // 玩家对象
    Game &game;

    PlayerProjectile playerprojectile; // 单个玩家子弹模板
    std::list<PlayerProjectile*> playerProjectiles; // 玩家子弹列表，使用指针管理避免拷贝问题

};

#endif // SCREEN_MAIN_H
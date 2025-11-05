#ifndef SCREEN_MAIN_H
#define SCREEN_MAIN_H

#include "Screen.h"
#include "Obj.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <list>
#include <random>

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

    void PlayerShoot(); // 玩家射击
    void EnemyShoot(); // 敌人射击

    void updatePlayerProjectiles(float deltaTime); // 更新玩家子弹位置
    void renderPlayerProjectiles(); // 渲染玩家子弹

    void spawnEnemy(); // 生成敌人
    void updateEnemies(float deltaTime); // 更新敌人位置
    void renderEnemies(); // 渲染敌人

private:
    // 主屏幕特有的成员变量
    Player player; // 玩家对象
    Game &game;

    PlayerProjectile playerprojectile; // 单个玩家子弹模板
    std::list<PlayerProjectile*> playerProjectiles; // 玩家子弹列表，使用指针管理避免拷贝问题

    Enemy enemy; // 敌人模板
    std::list<Enemy*> enemies; // 敌人列表，使用指针管理避免拷贝问题

    std::mt19937 gen; // 随机数生成器
    std::uniform_real_distribution<float> dis; // 随机数分布器
};

#endif // SCREEN_MAIN_H
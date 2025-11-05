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
    void EnemyShoot(Enemy* enemy); // 敌人射击

    void updatePlayerProjectiles(float deltaTime); // 更新玩家子弹状态
    void renderPlayerProjectiles(); // 渲染玩家子弹

    void spawnEnemy(); // 生成敌人
    void updateEnemies(float deltaTime); // 更新敌人状态
    void renderEnemies(); // 渲染敌人

    SDL_FPoint getDirection(Enemy* enemy); // 计算敌人子弹方向
    void updateEnemyProjectiles(float deltaTime); // 更新敌人子弹状态
    void renderEnemyProjectiles(); // 渲染敌人子弹

    void enemyExplosion(Enemy* enemy); // 敌人爆炸效果

    void updatePlayer(); // 更新玩家状态
    void renderPlayer(); // 渲染玩家

    void updateExplosions(); // 更新爆炸效果状态
    void renderExplosions(); // 渲染爆炸效果

    void dropItem(Enemy *enemy); // 掉落道具
    void updateItems(float deltaTime); // 更新道具状态
    void renderItems(); // 渲染道具
    void playerGetItem(Item* item); // 玩家获取道具

private:
    // 主屏幕特有的成员变量
    Player player; // 玩家对象
    bool isdead = false; // 玩家是否死亡
    Game &game;

    PlayerProjectile playerprojectile; // 单个玩家子弹模板
    std::list<PlayerProjectile*> playerProjectiles; // 玩家子弹列表，使用指针管理避免拷贝问题

    Enemy enemy; // 敌人模板
    std::list<Enemy*> enemies; // 敌人列表

    EnemyProjectile enemyprojectile; // 单个敌人子弹模板
    std::list<EnemyProjectile*> enemyProjectiles; // 敌人子弹列表

    Explosion explosion; // 爆炸效果模板
    std::list<Explosion*> explosions; // 爆炸效果列表

    Item item_HEALTH_PACK; // 血包道具模板
    Item item_SHIELD; // 护盾道具模板
    Item item_POWER_UP; // 强化道具模板
    std::list<Item*> items; // 道具列表

    std::mt19937 gen; // 随机数生成器
    std::uniform_real_distribution<float> dis; // 随机数分布器
};

#endif // SCREEN_MAIN_H
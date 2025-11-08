#ifndef SCREEN_MAIN_H
#define SCREEN_MAIN_H

#include "Screen.h"
#include "Obj.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <list>
#include <random>
#include <map>
#include <string>

// 前向声明，避免头文件循环依赖
class Game;

// 主屏幕类，负责游戏的主界面
class ScreenMain : public Screen {
public:
    ScreenMain();
    ~ScreenMain();

    virtual void init() override;
    virtual void clean() override;
    virtual void update(float deltaTime) override;
    virtual void render() override;
    virtual void handleEvents(SDL_Event* event) override;

private:
    void keyboardControls(float deltaTime);

    void PlayerShoot(); // 玩家射击
    void PlayerShootDown(); // 玩家向下射击（K键）
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

    void renderUI(); // 渲染UI

    void changeToEndingScreen(float deltaTime, float duration); // 切换到结束屏幕

private:
    // 主屏幕特有的成员变量
    Player player; // 玩家对象
    bool isdead = false; // 玩家是否死亡

    Mix_Music *bgm = nullptr; // 背景音乐
    std::map<std::string, Mix_Chunk*> soundCache; // 音效缓存

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

    SDL_Texture *healthBar; // 玩家生命值显示

    TTF_Font *scoreFont; // 分数字体
    int score = 0; // 玩家分数

    std::mt19937 gen; // 随机数生成器
    std::uniform_real_distribution<float> dis; // 随机数分布器

    float timerEnding = 0.0f; // 游戏结束计时器
};

#endif // SCREEN_MAIN_H
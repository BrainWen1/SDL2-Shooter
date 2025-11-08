#ifndef OBJ_H
#define OBJ_H

#include <SDL2/SDL.h>

// 玩家
class Player {
public:
    SDL_Texture* texture = nullptr; // 玩家纹理
    SDL_FPoint position = {0, 0}; // 玩家位置：浮点数坐标

    int width = 50; // 玩家宽度
    int height = 50; // 玩家高度

    float speed = 250.0f; // 玩家移动速度（像素/秒）

    Uint32 cooldownTime = 200; // 射击冷却时间（毫秒）
    Uint32 lastShotTime = 0; // 上次射击时间（毫秒）

    // 玩家生命值
    int health = 5;
    int MaxHealth = 5; // 玩家最大生命值
};

// 玩家子弹
class PlayerProjectile {
public:
    SDL_Texture* texture = nullptr; // 子弹纹理
    SDL_FPoint position = {0, 0}; // 子弹位置：浮点数坐标
    SDL_FPoint direction = {0.0f, -1.0f}; // 子弹方向，默认向上

    int width = 0; // 子弹宽度
    int height = 0; // 子弹高度

    float speed = 400.0f; // 子弹移动速度（像素/秒）

    // 子弹攻击力
    int damage = 1;
};

// 敌人
class Enemy {
public:
    SDL_Texture* texture = nullptr; // 敌人纹理
    SDL_FPoint position = {0, 0}; // 敌人位置：浮点数坐标
    int width = 50; // 敌人宽度
    int height = 50; // 敌人高度
    float speed = 80.0f; // 敌人移动速度（像素/秒）

    Uint32 cooldownTime = 1800; // 射击冷却时间（毫秒）
    Uint32 lastShotTime = 0; // 上次射击时间（毫秒）

    // 敌人生命值
    int health = 2;

    // 敌人碰撞伤害
    int collisionDamage = 1;

    // 敌人分值
    int scoreValue = 10;
};

// 敌人子弹
class EnemyProjectile {
public:
    SDL_Texture* texture = nullptr; // 子弹纹理
    SDL_FPoint position = {0, 0}; // 子弹位置：浮点数坐标
    int width = 0; // 子弹宽度
    int height = 0; // 子弹高度
    float speed = 200.0f; // 子弹移动速度（像素/秒）

    // 子弹朝向
    SDL_FPoint direction = {0.0f, 1.0f}; // 默认向下

    // 子弹攻击力
    int damage = 1;
};

// 爆炸效果
class Explosion {
public:
    SDL_Texture* texture = nullptr; // 爆炸纹理
    SDL_FPoint position = {0, 0}; // 爆炸位置：浮点数坐标
    int width = 0; // 爆炸宽度
    int height = 0; // 爆炸高度

    int currentFrame = 0; // 当前动画帧
    int totalFrames = 0; // 总动画帧数
    Uint32 startTime = 0; // 动画开始时间
    Uint32 FPS = 10; // 动画帧率
};

enum class ItemType {
    HEALTH_PACK, // 血包
    SHIELD, // 护盾
    POWER_UP // 强化
};

// 道具
class Item {
public:
    SDL_Texture* texture = nullptr; // 道具纹理
    SDL_FPoint position = {0, 0}; // 道具位置：浮点数坐标
    int width = 0; // 道具宽度
    int height = 0; // 道具高度

    SDL_FPoint direction = {0.0f, 0.0f}; // 道具移动方向

    ItemType type = ItemType::HEALTH_PACK; // 道具类型

    int speed = 200; // 道具移动速度（像素/秒）
    int bounceCount = 3; // 道具弹跳次数

    int scoreValue = 5; // 道具分值
};

// 背景图
class Background {
public:
    SDL_Texture* texture = nullptr; // 背景纹理
    SDL_FPoint position = {0, 0}; // 背景位置：浮点数坐标
    int width = 0; // 背景宽度
    int height = 0; // 背景高度
    int speed = 30; // 背景移动速度（像素/秒）

    float offsetY = 0.0f; // 背景垂直偏移
};

#endif // OBJ_H
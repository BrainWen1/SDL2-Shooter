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

    float speed = 200.0f; // 玩家移动速度（像素/秒）

    Uint32 cooldownTime = 500; // 射击冷却时间（毫秒）
    Uint32 lastShotTime = 0; // 上次射击时间（毫秒）
};

// 玩家子弹
class PlayerProjectile {
public:
    SDL_Texture* texture = nullptr; // 子弹纹理
    SDL_FPoint position = {0, 0}; // 子弹位置：浮点数坐标

    int width = 0; // 子弹宽度
    int height = 0; // 子弹高度

    float speed = 300.0f; // 子弹移动速度（像素/秒）
};

// 敌人
class Enemy {
public:
    SDL_Texture* texture = nullptr; // 敌人纹理
    SDL_FPoint position = {0, 0}; // 敌人位置：浮点数坐标
    int width = 50; // 敌人宽度
    int height = 50; // 敌人高度
    float speed = 100.0f; // 敌人移动速度（像素/秒）
};

#endif // OBJ_H
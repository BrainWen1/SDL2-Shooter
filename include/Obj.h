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

};

#endif // OBJ_H
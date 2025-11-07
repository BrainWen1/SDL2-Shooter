#ifndef GAME_H
#define GAME_H

#include "Obj.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Screen; // 前向声明，避免头文件循环依赖

class Game {
public:
    static Game &getInstance() {
        static Game instance;
        return instance;
    }
    ~Game();

    SDL_Window *getWindow() { return window; }
    SDL_Renderer *getRenderer() { return renderer; }

    void run(); // 运行游戏
    void init(); // 初始化游戏
    void clean(); // 清理资源

    void changeScreen(Screen* newScreen); // 切换屏幕

    void handleEvent(SDL_Event*);
    void update();
    void render();

    // 获取窗口尺寸
    int getScreenWidth() const { return screenWidth; }
    int getScreenHeight() const { return screenHeight; }

    // 渲染文字
    SDL_Point renderTextCentered(const std::string &text, float y, bool isTitle);
    void renderTextPoint(const std::string &text, int x, int y);

    int getFinalScore() const { return finalScore; } // 获取最终得分
    void setFinalScore(const int &score) { finalScore = score; } // 设置最终得分

    // // 设置游戏难度
    // int getEnemySpawnRate() const { return enemySpawnRate; }
    // void setEnemySpawnRate(float rate) { enemySpawnRate = rate; } // 设置敌人生成速率

    // int getEnemySpeed() const { return enemySpeed; }
    // void setEnemySpeed(float speed) { enemySpeed = speed; } // 设置敌人移动速度

    // int getPlayerShootCooldown() const { return playerShootCooldown; }
    // void setPlayerShootCooldown(float cooldown) { playerShootCooldown = cooldown; } // 设置玩家射击冷却时间

    // int getPlayerSpeed() const { return playerSpeed; }
    // void setPlayerSpeed(float speed) { playerSpeed = speed; } // 设置玩家移动速度

private:
    bool isRunning = true; // 游戏是否在运行
    Screen* currentScreen = nullptr; // 当前屏幕
    SDL_Window* window = nullptr; // 游戏窗口
    SDL_Renderer* renderer = nullptr; // 渲染器

    int screenWidth = 600; // 窗口宽度
    int screenHeight = 800; // 窗口高度

    // 单例模式，禁止拷贝和赋值
    Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    // 游戏帧率
    const int FPS = 120;
    Uint32 frameTime;
    float deltaTime; // 每帧时间间隔

    // 背景图
    Background nearStars ; // 近星背景
    Background farStars; // 远星背景
    void updateBackgrounds(float deltaTime); // 更新背景图
    void renderBackgrounds(); // 渲染背景图

    // 标题和结束字体
    TTF_Font *titleFont;
    TTF_Font *textFont;

    int finalScore = 0; // 最终得分

    // // 难度：计划中的游戏参数
    // float enemySpawnRate = 0.7f; // 敌人生成速率（每秒）
    // float enemySpeed = 100.0f; // 敌人移动速度（像素/秒）
    // float enemyShootSpeed = 1.0f; // 敌人攻击速度（秒）

    // float playerShootCooldown = 0.5f; // 玩家攻击速度（秒）
    // float playerSpeed = 200.0f; // 玩家移动速度（像素/秒）
};

#endif // GAME_H
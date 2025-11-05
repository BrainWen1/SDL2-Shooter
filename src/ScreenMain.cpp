#include "ScreenMain.h"
#include "Game.h"

ScreenMain::ScreenMain() : game(Game::getInstance()) {
    // 构造函数
}

ScreenMain::~ScreenMain() {
    // 析构函数
}

void ScreenMain::init() { // 初始化主屏幕

    player.texture = IMG_LoadTexture(game.getRenderer(), "../assets/image/SpaceShip.png"); // 加载玩家纹理
    SDL_QueryTexture(player.texture, nullptr, nullptr, &player.width, &player.height); // 查询纹理尺寸，并载入 player.width 和 player.height
    
    // 缩放玩家尺寸
    player.width /= 4;
    player.height /= 4;
   
    // 设置玩家初始位置：屏幕底部中央
    player.position.x = (game.getScreenWidth() - player.width) / 2.0f;
    player.position.y = game.getScreenHeight() - player.height;

    // 初始化玩家子弹纹理，避免频繁读取硬盘
    playerprojectile.texture = IMG_LoadTexture(game.getRenderer(), "../assets/image/laser-1.png");
    SDL_QueryTexture(playerprojectile.texture, nullptr, nullptr, &playerprojectile.width, &playerprojectile.height);
    // 缩放子弹尺寸
    playerprojectile.width /= 4;
    playerprojectile.height /= 4;
}

void ScreenMain::clean() { // 清理主屏幕

    // 清理玩家纹理
    if (player.texture != nullptr) {
        SDL_DestroyTexture(player.texture);
        player.texture = nullptr;
    }

    // 清理子弹模板（注意：所有子弹共享该纹理，故不在循环中逐个销毁）
    if (playerprojectile.texture != nullptr) {
        SDL_DestroyTexture(playerprojectile.texture);
        playerprojectile.texture = nullptr;
    }

    // 清理玩家子弹列表
    for (auto projectile : playerProjectiles) {
        // 不销毁 projectile->texture（共享模板纹理），只释放对象
        projectile->texture = nullptr;
        delete projectile;
    }
    playerProjectiles.clear();
}

void ScreenMain::update(float deltaTime) { // 更新主屏幕

    keyboardControls(deltaTime); // 处理键盘控制
    updatePlayerProjectiles(deltaTime); // 更新玩家子弹位置
}

void ScreenMain::render() { // 渲染主屏幕

    // 渲染玩家子弹
    renderPlayerProjectiles(); // 写在玩家前面，确保子弹被玩家覆盖
    
    // 将玩家的纹理绘制到屏幕上
    SDL_Rect rect = {
        static_cast<int>(player.position.x), // SDL_FPoint 存储浮点数坐标，需转换为整数
        static_cast<int>(player.position.y),
        player.width,
        player.height
    };
    SDL_RenderCopy(game.getRenderer(), player.texture, nullptr, &rect);
}

void ScreenMain::handleEvents(SDL_Event* event) {
    // 处理主屏幕的事件
}

void ScreenMain::keyboardControls(float deltaTime) {

    auto state = SDL_GetKeyboardState(NULL); // 获取键盘状态数组

    float move = player.speed * deltaTime; // 计算移动距离

    if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT]) {
        player.position.x -= move;

        if (player.position.x < 0) {
            player.position.x = 0; // 防止移出左边界
        }
    }
    if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]) {
        player.position.x += move;

        if (player.position.x > game.getScreenWidth() - player.width) {
            player.position.x = game.getScreenWidth() - player.width; // 防止移出右边界
        }
    }
    if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP]) {
        player.position.y -= move;

        if (player.position.y < 0) {
            player.position.y = 0; // 防止移出上边界
        }
    }
    if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN]) {
        player.position.y += move;
        
        if (player.position.y > game.getScreenHeight() - player.height) {
            player.position.y = game.getScreenHeight() - player.height; // 防止移出下边界
        }
    }

    if (state[SDL_SCANCODE_J]) {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - player.lastShotTime >= player.cooldownTime) {
            // 可以射击
            PlayerShoot();
            player.lastShotTime = currentTime; // 更新上次射击时间
        }
    }
}

void ScreenMain::PlayerShoot() { // 玩家射击

    // 创建一个新的玩家子弹
    PlayerProjectile *projectile = new PlayerProjectile(playerprojectile); // 使用模板初始化新子弹

    // 设置子弹初始位置：玩家顶部中央
    projectile->position.x = player.position.x + (player.width - projectile->width) / 2.0f;
    projectile->position.y = player.position.y;

    // 将新子弹添加到子弹列表
    playerProjectiles.push_back(projectile);
}

void ScreenMain::updatePlayerProjectiles(float deltaTime) { // 更新玩家子弹位置

    for (auto it = playerProjectiles.begin(); it != playerProjectiles.end(); ) {
        PlayerProjectile* projectile = *it; // 获取当前子弹指针
        projectile->position.y -= projectile->speed * deltaTime; // 更新子弹位置，向上移动

        // 如果子弹移出屏幕顶部，则删除该子弹
        if (projectile->position.y + projectile->height < 0) {
            // 子弹越界：不要销毁纹理（所有子弹共享模板纹理），只释放对象
            projectile->texture = nullptr; // 防止误用悬空指针

            delete projectile; // 释放内存
            it = playerProjectiles.erase(it); // 从列表中移除子弹
        } else {
            ++it; // 移动到下一个子弹
        }
    }
}

void ScreenMain::renderPlayerProjectiles() { // 渲染玩家子弹

    for (auto projectile : playerProjectiles) {
        SDL_Rect rect = {
            static_cast<int>(projectile->position.x),
            static_cast<int>(projectile->position.y),
            projectile->width,
            projectile->height
        };
        SDL_RenderCopy(game.getRenderer(), projectile->texture, nullptr, &rect);
    }
}
#include "ScreenMain.h"
#include "Game.h"

ScreenMain::ScreenMain() : game(Game::getInstance()) {
    // 构造函数
}

ScreenMain::~ScreenMain() {
    // 析构函数
}

void ScreenMain::init() { // 初始化主屏幕

    // 初始化随机数引擎
    std::random_device rd;
    gen = std::mt19937(rd());
    dis = std::uniform_real_distribution<float>(0.0f, 1.0f); // 随机数分布

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

    // 初始化敌人纹理
    enemy.texture = IMG_LoadTexture(game.getRenderer(), "../assets/image/insect-1.png");
    SDL_QueryTexture(enemy.texture, nullptr, nullptr, &enemy.width, &enemy.height);
    // 缩放敌人尺寸
    enemy.width /= 4;
    enemy.height /= 4;

    // 初始化敌人子弹纹理
    enemyprojectile.texture = IMG_LoadTexture(game.getRenderer(), "../assets/image/bullet-2.png");
    SDL_QueryTexture(enemyprojectile.texture, nullptr, nullptr, &enemyprojectile.width, &enemyprojectile.height);
    // 缩放敌人子弹尺寸
    enemyprojectile.width /= 4;
    enemyprojectile.height /= 4;
}

void ScreenMain::clean() { // 清理主屏幕

    // 清理玩家纹理
    if (player.texture != nullptr) {
        SDL_DestroyTexture(player.texture);
        player.texture = nullptr;
    }

    // 清理子弹模板纹理
    if (playerprojectile.texture != nullptr) {
        SDL_DestroyTexture(playerprojectile.texture);
        playerprojectile.texture = nullptr;
    }

    // 清理玩家子弹列表
    for (auto &projectile : playerProjectiles) {
        projectile->texture = nullptr;
        delete projectile;
    }
    playerProjectiles.clear();

    // 清理敌人模板纹理
    if (enemy.texture != nullptr) {
        SDL_DestroyTexture(enemy.texture);
        enemy.texture = nullptr;
    }

    // 清理敌人列表
    for (auto &enemy : enemies) {
        enemy->texture = nullptr;
        delete enemy;
    }
    enemies.clear();

    // 清理敌人子弹模板纹理
    if (enemyprojectile.texture != nullptr) {
        SDL_DestroyTexture(enemyprojectile.texture);
        enemyprojectile.texture = nullptr;
    }

    // 清理敌人子弹列表
    for (auto &projectile : enemyProjectiles) {
        projectile->texture = nullptr;
        delete projectile;
    }
    enemyProjectiles.clear();
}

void ScreenMain::update(float deltaTime) { // 更新主屏幕

    keyboardControls(deltaTime); // 处理键盘控制
    updatePlayerProjectiles(deltaTime); // 更新玩家子弹位置
    spawnEnemy(); // 生成敌人
    updateEnemies(deltaTime); // 更新敌人位置
    updateEnemyProjectiles(deltaTime); // 更新敌人子弹位置
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

    // 渲染敌人
    renderEnemies();

    // 渲染敌人子弹
    renderEnemyProjectiles();
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

            // 子弹越界 -> 销毁
            projectile->texture = nullptr; // 置空
            delete projectile; // 释放内存
            it = playerProjectiles.erase(it); // 从列表中移除子弹
        } else {
            ++it; // 移动到下一个子弹
        }
    }
}

void ScreenMain::renderPlayerProjectiles() { // 渲染玩家子弹

    for (const auto &projectile : playerProjectiles) {
        SDL_Rect rect = {
            static_cast<int>(projectile->position.x),
            static_cast<int>(projectile->position.y),
            projectile->width,
            projectile->height
        };
        SDL_RenderCopy(game.getRenderer(), projectile->texture, nullptr, &rect);
    }
}

void ScreenMain::spawnEnemy() { // 生成敌人

    if (dis(gen) <= 0.65f / 60.0f) { // 敌人生成概率：每秒约生成0.65个敌人
        Enemy* newEnemy = new Enemy(enemy); // 使用模板初始化新敌人

        // 设置敌人初始位置：屏幕顶部随机位置
        newEnemy->position.x = dis(gen) * (game.getScreenWidth() - newEnemy->width);
        newEnemy->position.y = -newEnemy->height; // 从屏幕顶部外开始

        // 将新敌人添加到敌人列表
        enemies.push_back(newEnemy);
    }
}

void ScreenMain::updateEnemies(float deltaTime) { // 更新敌人位置

    auto currentTime = SDL_GetTicks(); // 获取当前时间

    for (auto it = enemies.begin(); it != enemies.end(); ) {
        Enemy* enemy = *it; // 获取当前敌人指针
        enemy->position.y += enemy->speed * deltaTime; // 更新敌人位置，向下移动

        // 如果敌人移出屏幕底部，则删除该敌人
        if (enemy->position.y > game.getScreenHeight()) {

            // 敌人越界 -> 销毁
            enemy->texture = nullptr; // 置空
            delete enemy; // 释放内存
            it = enemies.erase(it); // 从列表中移除敌人
        } else {
            if (currentTime - enemy->lastShotTime >= enemy->cooldownTime) {
                EnemyShoot(enemy);
                enemy->lastShotTime = currentTime; // 更新上次射击时间
            }

            ++it; // 移动到下一个敌人
        }
    }
}

void ScreenMain::renderEnemies() { // 渲染敌人

    for (const auto &enemy : enemies) {
        SDL_Rect rect = {
            static_cast<int>(enemy->position.x),
            static_cast<int>(enemy->position.y),
            enemy->width,
            enemy->height
        };
        SDL_RenderCopy(game.getRenderer(), enemy->texture, nullptr, &rect);
    }
}

void ScreenMain::EnemyShoot(Enemy* enemy) { // 敌人射击

    // 创建一个新的敌人子弹
    EnemyProjectile *projectile = new EnemyProjectile(enemyprojectile); // 使用模板初始化新子弹

    // 设置子弹初始位置：敌人正中心
    projectile->position.x = enemy->position.x + (enemy->width - projectile->width) / 2.0f;
    projectile->position.y = enemy->position.y + (enemy->height - projectile->height) / 2.0f;

    // 确定子弹朝向玩家
    projectile->direction = getDirection(enemy);

    // 将新子弹添加到敌人子弹列表
    enemyProjectiles.push_back(projectile);
}

SDL_FPoint ScreenMain::getDirection(Enemy* enemy) { // 计算敌人子弹方向

    // 获取玩家和敌人中心点坐标差
    float dx = (player.position.x + player.width / 2.0f) - (enemy->position.x + enemy->width / 2.0f);
    float dy = (player.position.y + player.height / 2.0f) - (enemy->position.y + enemy->height / 2.0f);
    float length = sqrtf(dx * dx + dy * dy);

    // 归一化方向向量
    if (length != 0) {
        dx /= length;
        dy /= length;
    } else {
        dx = 0.0f;
        dy = 1.0f; // 默认向下
    }

    return SDL_FPoint{dx, dy};
}

void ScreenMain::updateEnemyProjectiles(float deltaTime) { // 更新敌人子弹位置

    for (auto it = enemyProjectiles.begin(); it != enemyProjectiles.end(); ) {
        EnemyProjectile* projectile = *it; // 获取当前子弹指针

        // 更新子弹位置，按方向移动
        projectile->position.x += projectile->direction.x * projectile->speed * deltaTime;
        projectile->position.y += projectile->direction.y * projectile->speed * deltaTime;

        // 如果子弹移出屏幕，则删除该子弹
        if (projectile->position.y > game.getScreenHeight() ||
            projectile->position.y + projectile->height < 0 ||
            projectile->position.x + projectile->width < 0 ||
            projectile->position.x > game.getScreenWidth()) {
                
            // 子弹越界 -> 销毁
            projectile->texture = nullptr; // 置空
            delete projectile; // 释放内存
            it = enemyProjectiles.erase(it); // 从列表中移除子弹
        } else {
            ++it; // 移动到下一个子弹
        }
    }
}

void ScreenMain::renderEnemyProjectiles() { // 渲染敌人子弹

    for (const auto &projectile : enemyProjectiles) {
        SDL_Rect rect = {
            static_cast<int>(projectile->position.x),
            static_cast<int>(projectile->position.y),
            projectile->width,
            projectile->height
        };
        // SDL_RenderCopy(game.getRenderer(), projectile->texture, nullptr, &rect);
        // 旋转子弹以匹配方向
        float angle = atan2(projectile->direction.y, projectile->direction.x) * 180.0f / M_PI - 90.0f;
        SDL_RenderCopyEx(game.getRenderer(), projectile->texture, nullptr, &rect, angle, nullptr, SDL_FLIP_NONE);
    }
}

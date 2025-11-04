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

}

void ScreenMain::clean() { // 清理主屏幕
    if (player.texture != nullptr) {
        SDL_DestroyTexture(player.texture);
        player.texture = nullptr;
    }
}

void ScreenMain::update() {
    // 更新主屏幕
}

void ScreenMain::render() { // 渲染主屏幕
    
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

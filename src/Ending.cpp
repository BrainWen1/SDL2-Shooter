#include "Ending.h"
#include "Game.h"
#include "Title.h"

void Ending::init() { // 初始化结束屏幕

    // 播放背景音乐
    bgm = Mix_LoadMUS("../assets/music/homepage.ogg");
    if (bgm != nullptr) {
        Mix_PlayMusic(bgm, -1); // 循环播放背景音乐
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Mix_LoadMUS Error: %s", Mix_GetError());
    }

    // 启动文本输入
    if (SDL_IsTextInputActive() == SDL_FALSE) {
        SDL_StartTextInput();
    }
    if (SDL_IsTextInputActive() == SDL_FALSE) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_StartTextInput Error");
    }
}

void Ending::clean() { // 清理结束屏幕

    if (SDL_IsTextInputActive() == SDL_TRUE) {
        SDL_StopTextInput(); // 停止文本输入
    }

    // 清理背景音乐
    if (bgm != nullptr) {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
        bgm = nullptr;
    }
}

void Ending::update(float deltaTime) { // 更新结束屏幕

    timer += deltaTime;
    if (timer >= flashInterval * 2.0f) { // 控制一个周期为1秒
        timer = 0.0f;
    }
}

void Ending::render() { // 渲染结束屏幕

    if (isTying) {
        renderPhase1();       
    } else {
        renderPhase2();
    }
}

void Ending::handleEvents(SDL_Event* event) { // 处理结束屏幕的事件

    if (isTying == true) {
        if (event->type == SDL_TEXTINPUT) { // 处理文本输入事件

            Name += event->text.text; // 将输入的文本添加到 Name 中 

        }
        if (event->type == SDL_KEYDOWN) { // 处理按键事件

            if (event->key.keysym.scancode == SDL_SCANCODE_RETURN) {
                // 按下回车键，结束输入
                isTying = false;
                SDL_StopTextInput(); // 停止文本输入

                if (Name.empty()) {
                    Name = "Anonymous"; // 如果名字为空，设置默认名字
                }
                game.insertHighScores(std::make_pair(game.getFinalScore(), Name)); // 插入得分榜单

            } else if (event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE) {
                
                // 正确处理中文字符退格
                if (Name.empty() == false) {
                    // 使用 UTF-8 编码处理退格
                    size_t len = Name.length();
                    while (len > 0) {
                        unsigned char c = Name[--len];
                        if ((c & 0b10000000) == 0) { // 单字节字符
                            break;
                        } else if ((c & 0b11000000) == 0b11000000) { // 找到多字节字符的起始字节
                            break;
                        }
                    }
                    Name.erase(len); // 删除从 len 到末尾的字符
                } else {
                    return;
                }
            }
        }
    } else { // 按任意键除了F4, 返回标题屏幕
        if (event->type == SDL_KEYDOWN && event->key.keysym.scancode != SDL_SCANCODE_F4) {
                auto newScreen = new Title();
                game.changeScreen(newScreen);
        }
    }
}

void Ending::renderPhase1() { // 渲染结束屏幕的第一阶段
    
    int score = game.getFinalScore(); // 获取最终得分
    std::string gameOver = "Game Over";
    std::string message = "你的最终得分: " + std::to_string(score);
    std::string prompt = "请输入你的名字，按回车确认: ";

    game.renderTextCentered(gameOver, 0.3f, true);
    game.renderTextCentered(message, 0.5f, false);
    game.renderTextCentered(prompt, 0.7f, false);

    // 渲染玩家输入的名字
    if (Name.empty() == false) {
        SDL_Point pos = game.renderTextCentered(Name, 0.8f, false);

        if (timer <= flashInterval) { // 文字闪烁效果
            game.renderTextPoint("_", pos.x, pos.y); // 在名字后面显示光标
        }

    } else if (timer <= flashInterval) {
        game.renderTextCentered("_", 0.8f, false); // 显示光标  
    }
    
}

void Ending::renderPhase2() { // 渲染结束屏幕的第二阶段

    // 渲染得分榜单
    game.renderTextCentered("Score List", 0.1f, true);

    float posY = 0.2f * game.getScreenHeight();
    float posX = 100;
    size_t rank = 1;
    for (auto &p : game.getHighScores()) {
        std::string name = std::to_string(rank) + " " + p.second;
        std::string score = std::to_string(p.first);

        game.renderTextPoint(name, static_cast<int>(posX), static_cast<int>(posY)); // 默认左对齐
        game.renderTextPoint(
            score,
            static_cast<int>(posX),
            static_cast<int>(posY),
            false // 右对齐
        );
        ++rank;
        posY += 40.0f; // 每条记录间隔40像素
    }

    if (timer <= flashInterval) { // 文字闪烁效果
        game.renderTextCentered("Press Any-Key to Return to Title", 0.9f, false);
    }
}
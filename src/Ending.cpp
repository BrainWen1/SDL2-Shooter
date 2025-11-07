#include "Ending.h"
#include "Game.h"

void Ending::init() { // 初始化结束屏幕

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

void Ending::renderPhase2() {
    // 渲染结束屏幕的第二阶段
    std::string message = "Game Over";
    game.renderTextCentered(message, 0.5f, false);
}
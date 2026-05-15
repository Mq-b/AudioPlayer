#include "AudioPlayer/AudioPlayer.h"
#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    std::string soundDir = "./sound";
    if (argc > 1) {
        soundDir = argv[1];
    }

    AudioPlayer player;
    int index = 0;

    // 自动注册目录下的音频文件
    for (const auto& entry : fs::directory_iterator(soundDir)) {
        if (entry.is_regular_file()) {
            std::string path = entry.path().string();
            std::string key = "s" + std::to_string(index++);
            player.registerSound(key, path);
            std::cout << "  [" << key << "] " << entry.path().filename().string() << std::endl;
        }
    }
    std::cout << std::endl;

    std::cout << "命令: p <key> 播放 | d <key> 禁用 | e <key> 启用 | l 列出 | q 退出" << std::endl;

    std::string line;
    while (std::cout << "> ", std::getline(std::cin, line)) {
        if (line.empty()) continue;

        char cmd = line[0];
        std::string arg = line.size() > 2 ? line.substr(2) : "";

        switch (cmd) {
        case 'p':
            if (!arg.empty()) player.play(arg);
            break;
        case 'd':
            if (!arg.empty()) {
                player.setEnabled(arg, false);
                std::cout << (player.isEnabled(arg) ? "已禁用" : "未找到") << std::endl;
            }
            break;
        case 'e':
            if (!arg.empty()) {
                player.setEnabled(arg, true);
                std::cout << (player.isEnabled(arg) ? "已启用" : "未找到") << std::endl;
            }
            break;
        case 'l':
            for (int i = 0; i < index; i++) {
                std::string key = "s" + std::to_string(i);
                std::cout << "  [" << key << "] " << (player.isEnabled(key) ? "启用" : "禁用") << std::endl;
            }
            break;
        case 'q':
            return 0;
        default:
            std::cout << "未知命令" << std::endl;
        }
    }
}

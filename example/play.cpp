#include "AudioPlayer/AudioPlayer.h"
#include <iostream>
#include <chrono>
#include <thread>
using namespace std::chrono_literals;

int main() {
    AudioPlayer player;

    // 注册声音
    player.registerSound("init_failed",    "./sound/初始化失败.mp3");
    player.registerSound("init_success",   "./sound/初始化成功.mp3");
    player.registerSound("process_error",  "./sound/流程异常.mp3");
    player.registerSound("detection_done", "./sound/检测完成.mp3");

    // 播放
    std::cout << "播放 init_success ..." << std::endl;
    player.play("init_success");
    std::this_thread::sleep_for(2s);

    // 禁用后播放无效
    player.setEnabled("process_error", false);
    std::cout << "process_error 已禁用，尝试播放（应无声音）..." << std::endl;
    player.play("process_error");
    std::this_thread::sleep_for(2s);

    // 重新启用
    player.setEnabled("process_error", true);
    std::cout << "播放 process_error ..." << std::endl;
    player.play("process_error");
    std::this_thread::sleep_for(2s);

    std::cout << "播放 detection_done ..." << std::endl;
    player.play("detection_done");
    std::this_thread::sleep_for(3s);

    std::cout << "结束" << std::endl;
}

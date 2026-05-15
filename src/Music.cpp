#define MINIAUDIO_IMPLEMENTATION
#include "AudioPlayer/Music.h"
#include <chrono>
#include <iostream>
#include <thread>
using namespace std::chrono_literals;

Music::Music() {
    ma_result result = ma_engine_init(nullptr, &engine);
    if (result != MA_SUCCESS) {
        std::cerr << "AudioPlayer: failed to initialize miniaudio engine" << std::endl;
    }
}

Music::~Music() {
    ma_engine_uninit(&engine);
}

void Music::play(const std::string& filePath) {
    ma_sound sound;
    ma_result result = ma_sound_init_from_file(&engine, filePath.c_str(), 0, nullptr, nullptr, &sound);
    if (result != MA_SUCCESS) {
        std::cerr << "AudioPlayer: failed to load " << filePath << std::endl;
        return;
    }

    result = ma_sound_start(&sound);
    if (result != MA_SUCCESS) {
        std::cerr << "AudioPlayer: failed to play " << filePath << std::endl;
        ma_sound_uninit(&sound);
        return;
    }

    while (ma_sound_is_playing(&sound)) {
        std::this_thread::sleep_for(100ms);
    }

    ma_sound_uninit(&sound);
}

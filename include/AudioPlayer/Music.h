#ifndef AUDIO_PLAYER_MUSIC_H
#define AUDIO_PLAYER_MUSIC_H

#include "miniaudio.h"
#include <string>

class Music {
public:
    Music();
    ~Music();

    void play(const std::string& filePath);

private:
    ma_engine engine;
};

#endif // AUDIO_PLAYER_MUSIC_H

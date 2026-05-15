#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include "Music.h"
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <unordered_map>
#include <atomic>

class AudioPlayer {
public:
    AudioPlayer() {
        start();
    }

    ~AudioPlayer() {
        while (!audioQueue.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        stop = true;
        condVar.notify_all();
        if (playerThread.joinable()) {
            playerThread.join();
        }
    }

    AudioPlayer(const AudioPlayer&) = delete;
    AudioPlayer& operator=(const AudioPlayer&) = delete;

    void registerSound(const std::string& key, const std::string& filePath) {
        std::lock_guard<std::mutex> lock{mtx};
        soundMap[key] = {filePath, true};
    }

    void play(const std::string& key) {
        std::lock_guard<std::mutex> lock{mtx};
        auto it = soundMap.find(key);
        if (it == soundMap.end() || !it->second.enabled) return;
        addAudioPath(it->second.filePath);
    }

    void setEnabled(const std::string& key, bool enabled) {
        std::lock_guard<std::mutex> lock{mtx};
        auto it = soundMap.find(key);
        if (it != soundMap.end()) {
            it->second.enabled = enabled;
        }
    }

    bool isEnabled(const std::string& key) const {
        std::lock_guard<std::mutex> lock{mtx};
        auto it = soundMap.find(key);
        return it != soundMap.end() && it->second.enabled;
    }

private:
    struct SoundEntry {
        std::string filePath;
        bool enabled = true;
    };

    void addAudioPath(const std::string& path) {
        audioQueue.push(path);
        condVar.notify_one();
    }

    void start() {
        playerThread = std::thread(&AudioPlayer::playLoop, this);
    }

    void playLoop() {
        while (!stop) {
            std::string path;
            {
                std::unique_lock<std::mutex> lock{mtx};
                condVar.wait(lock, [this] { return !audioQueue.empty() || stop; });
                if (audioQueue.empty()) return;
                path = audioQueue.front();
                audioQueue.pop();
            }
            music.play(path);
        }
    }

    std::unordered_map<std::string, SoundEntry> soundMap;
    std::queue<std::string> audioQueue;
    std::atomic<bool> stop{false};
    mutable std::mutex mtx;
    std::condition_variable condVar;
    Music music;
    std::thread playerThread;
};

#endif // AUDIO_PLAYER_H

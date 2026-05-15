# AudioPlayer

一个简洁的 C++17 音频播放库，基于 [miniaudio](https://github.com/mackron/miniaudio)。

支持外部注册声音资源，无需修改源码。

## 特性

- 后台线程播放，不阻塞调用方
- 通过字符串 key 注册和播放声音
- 支持运行时启用/禁用单个声音
- 线程安全

## 依赖

- C++17
- miniaudio（已包含在 `third_party/` 中）
- pthread（Linux）

## 系统要求

本库通过 [miniaudio](https://miniaud.io/) 访问系统音频，运行时依赖系统音频后端：

|系统|音频后端|要求|
|---|---|---|
|Linux (桌面)|ALSA / PulseAudio|现代发行版默认满足|
|Linux (嵌入式)|ALSA|需内核 ALSA 驱动 + `libasound`|
|Windows|WASAPI|Win7+ 自带|

> 嵌入式 Linux：推荐使用 ALSA 后端，内核需启用 `CONFIG_SND` 相关选项，用户空间需 `alsa-lib`（libasound）。miniaudio 运行时通过 `dlopen` 动态加载，编译时无需显式链接。

## 构建

```bash
mkdir build && cd build
cmake ..
cmake --build . -j
```

运行示例：

```bash
./bin/play-example     # 自动播放演示
./bin/cli-example      # 交互式命令行
```

## 集成到你的项目

复制以下 4 个文件到你的项目中：

```txt
include/AudioPlayer/AudioPlayer.h
include/AudioPlayer/Music.h
src/Music.cpp
third_party/miniaudio.h
```

在你的 `CMakeLists.txt` 中将 `Music.cpp` 加入编译，并添加头文件路径：

```cmake
add_executable(your_app
    main.cpp
    path/to/Music.cpp
)

target_include_directories(your_app PRIVATE
    path/to/include      # 包含 AudioPlayer/AudioPlayer.h
    path/to/third_party  # 包含 miniaudio.h
)
target_link_libraries(your_app PRIVATE Threads::Threads ${CMAKE_DL_LIBS})
```

然后就可以直接 `#include "AudioPlayer/AudioPlayer.h"` 使用了。

## 使用

```cpp
#include "AudioPlayer/AudioPlayer.h"

AudioPlayer player;

// 注册声音
player.registerSound("alert", "./sound/alert.mp3");
player.registerSound("success", "./sound/success.mp3");

// 播放
player.play("alert");

// 禁用某个声音
player.setEnabled("alert", false);
player.play("alert"); // 不会播放

// 重新启用
player.setEnabled("alert", true);
player.play("alert"); // 正常播放
```

## CLI 示例

交互式命令行工具，自动注册指定目录下的音频文件：

```bash
./bin/cli-example [sound_dir]   # 默认 ./sound
```

命令：

- `p <key>` — 播放
- `d <key>` — 禁用
- `e <key>` — 启用
- `l` — 列出所有声音及状态
- `q` — 退出

## 项目结构

```txt
AudioPlayer/
├── CMakeLists.txt
├── README.md
├── include/
│   └── AudioPlayer/
│       ├── AudioPlayer.h    # 核心播放器（header-only）
│       └── Music.h          # miniaudio 封装
├── src/
│   └── Music.cpp
├── third_party/
│   └── miniaudio.h          # miniaudio v0.11.21
├── example/
│   ├── play.cpp             # 自动播放演示
│   └── cli.cpp              # 交互式命令行
└── sound/                   # 示例音频文件
```

## 音频制作

如需生成 TTS 语音文件作为播放资源，可使用 [tts-vue-next](https://github.com/LokerL/tts-vue-next)，支持多种 TTS 引擎，可批量导出音频文件。

## 许可证

本项目基于 [Apache License 2.0](LICENSE)。

miniaudio 遵循其自身的 [许可证](https://github.com/mackron/miniaudio#license)（Public Domain / MIT No Attribution）。

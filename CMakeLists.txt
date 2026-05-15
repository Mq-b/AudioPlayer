cmake_minimum_required(VERSION 3.14)
project(AudioPlayer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

option(AUDIO_PLAYER_BUILD_EXAMPLE "Build example" ON)
if(AUDIO_PLAYER_BUILD_EXAMPLE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

    add_executable(audio_example
        example/play.cpp
        src/Music.cpp
    )

    target_include_directories(audio_example PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party
    )
    target_link_libraries(audio_example PRIVATE Threads::Threads ${CMAKE_DL_LIBS})

    add_custom_command(TARGET audio_example POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/sound
            $<TARGET_FILE_DIR:audio_example>/sound
    )
endif()

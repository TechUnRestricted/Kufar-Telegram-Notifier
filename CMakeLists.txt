cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(
    Kufar-Telegram-Notifier
    VERSION 2.5
)

add_executable(
    Kufar-Telegram-Notifier

    src/helperfunctions.hpp
    src/helperfunctions.cpp
    src/networking.hpp
    src/networking.cpp
    src/json.hpp
    src/kufar.hpp
    src/kufar.cpp
    src/telegram.hpp
    src/telegram.cpp
    src/main.cpp
)

target_link_libraries(Kufar-Telegram-Notifier curl)

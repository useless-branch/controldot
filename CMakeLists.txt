cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(ControlDot)

add_compile_options (-fdiagnostics-color=always)
add_compile_options (-Wfatal-errors)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options("/W4")
    message("Compiling with: Microsoft Compiler")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options("-Wall" "-Wextra" "-pedantic")
    message("Compiling with: GCC Compiler")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    #add_compile_options("-Weverything")
    add_compile_options(
            "-Wall"
            "-Wextra"
            "-pedantic"
            "-Wfatal-errors"
            "-Wno-c++98-compat"
            "-Wno-c++98-c++11-compat-binary-literal"
            "-Wno-c++98-compat-pedantic"
    )
    message("Compiling with: Clang Compiler")
endif()

find_package(fmt)
find_package(Threads)
find_package(OpenCV REQUIRED)
find_package(SDL2 REQUIRED)
add_executable(ControlDot
    src/main.cpp src/imageOptions/clockFaceWithDate.hpp src/imageOptions/clockFace.hpp src/imageOptions/weather/weather.hpp src/imageOptions/weather/weatherAPI.hpp)

add_subdirectory(toxic_spokes)
add_subdirectory(aglio)
add_subdirectory(recreated_goon)
add_subdirectory(json)

add_subdirectory(mcufont)

mcufont_generate(main_font_9
        FONT UbuntuMono-R.ttf
        SIZE 9
        ITERATIONS 80
        RANGES 0-127)

mcufont_generate(main_font_11
        FONT UbuntuMono-R.ttf
        SIZE 11
        ITERATIONS 80
        RANGES 0-127)

mcufont_generate(main_font_16
        FONT UbuntuMono-R.ttf
        SIZE 16
        ITERATIONS 80
        RANGES 0-127)

mcufont_generate(main_font_16_bold
        FONT UbuntuMono-B.ttf
        SIZE 16
        ITERATIONS 80
        RANGES 0-127)

mcufont_generate(main_font_20_bold
        FONT UbuntuMono-B.ttf
        SIZE 20
        ITERATIONS 80
        RANGES 0-127)

mcufont_generate(main_font_12_bold
        FONT UbuntuMono-B.ttf
        SIZE 12
        ITERATIONS 80
        RANGES 0-127)

recreated_goon_generate(
        type_descriptors
        OUTPUT_DIRECTORY type_descriptors
        IGNORE_ERRORS
        FILES external/firmdot/src/package.hpp
)

mcufont_add_library(ControlDot FONTS main_font_9 main_font_11 main_font_16 main_font_16_bold main_font_12_bold main_font_20_bold)

target_link_libraries(ControlDot
    toxic_spokes::toxic_spokes
    fmt::fmt-header-only
    Threads::Threads
    aglio::aglio
    nlohmann_json::nlohmann_json
    ${SDL2_LIBRARIES}
    type_descriptors
        )

include_directories(${OpenCV_INCLUDE_DIRS})

target_include_directories(ControlDot PRIVATE external)


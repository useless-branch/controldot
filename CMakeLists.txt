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
    src/main.cpp)

add_subdirectory(toxic_spokes)
add_subdirectory(external/CRCpp)
add_subdirectory(aglio)
add_subdirectory(recreated_goon)


recreated_goon_generate(
        type_descriptors
        OUTPUT_DIRECTORY type_descriptors
        IGNORE_ERRORS
        FILES external/firmdot/src/package.hpp
)

target_link_libraries(ControlDot PRIVATE
    toxic_spokes::sockets
    fmt::fmt-header-only
    Threads::Threads
    aglio::aglio
    ${SDL2_LIBRARIES}
    type_descriptors
    ${OpenCV_LIBS}
        )


include_directories(${OpenCV_INCLUDE_DIRS})

target_include_directories(ControlDot PRIVATE external)


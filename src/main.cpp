#include "Display.hpp"
#include "filedummy.hpp"
#include "serial.hpp"
#include "UART.hpp"

#include <chrono>
#include <thread>
#include <fmt/chrono.h>
#include <CLI/CLI.hpp>
#include <SDL2/SDL.h>
#include <cassert>
#include <string>



int main(int argc, char const* const* argv) {
    CLI::App app{"ControlDot - application to control dot displays"};

    std::string serialConnection;
    app.add_option("-d,--device", serialConnection, "Serial Connection Device")->required()->check(CLI::ExistingFile);
    CLI11_PARSE(app, argc, argv);

    Display<Serial> display(serialConnection);
    bool foo{true};
    //Image Size 28*24
    while(true) {
        display.setPixel(52,13,foo);
        display.setPixel(53, 13, foo);
        foo = !foo;
        //display.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}

#include "Display.hpp"
#include "UART.hpp"
#include "filedummy.hpp"
#include "serial.hpp"
#include "Image.hpp"

#include <CLI/CLI.hpp>
#include <SDL2/SDL.h>
#include <cassert>
#include <chrono>
#include <fmt/chrono.h>
#include <string>
#include <thread>


int main(int argc, char const* const* argv) {
    CLI::App app{"ControlDot - application to control dot displays"};

    std::string serialConnection;
    app.add_option("-d,--device", serialConnection, "Serial Connection Device")
      ->required()
      ->check(CLI::ExistingFile);
    CLI11_PARSE(app, argc, argv);

    Display<Serial> display(serialConnection);
    bool            foo{true};
    Image img;
    
    //Image Size 28*24
    while(true) {
        for(std::size_t i{0}; i < 28*2; ++i){
        img.clear();
        img.renderString(i,13,"Hack Regular 8", "Bepis");
        display.setImage(img.data);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }

    return 0;
}

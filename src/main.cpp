#include "Display.hpp"
#include "UART.hpp"
#include "filedummy.hpp"
#include "serial.hpp"
#include "Image.hpp"

#include <CLI/CLI.hpp>
#include <SDL2/SDL.h>
#include <cassert>
#include <chrono>
#include <ctime>
#include <fmt/chrono.h>
#include <string>
#include <thread>
#include <cmath>


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
        img.clear();
        
        auto timePoint =  std::chrono::system_clock::now();
        time_t tt = std::chrono::system_clock::to_time_t(timePoint);
        tm local_tm = *localtime(&tt);
        std::string timeString = fmt::format("{:02}:{:02}:{:02}", local_tm.tm_hour, local_tm.tm_min, local_tm.tm_sec);      
        std::string dateString = fmt::format("{:04}-{:02}-{:02}", local_tm.tm_year + 1900,  local_tm.tm_mon + 1, local_tm.tm_mday);
        img.renderString(5,0,"main_font_12_bold", timeString);
        img.renderString(3,15,"main_font_9", dateString);
        display.setImage(img.data);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}

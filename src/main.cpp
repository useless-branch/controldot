#include "Display.hpp"
#include "UART.hpp"
#include "Image.hpp"

#include <CLI/CLI.hpp>
#include "imageOptions/clockFaceWithDate.hpp"
#include "imageOptions/clockFace.hpp"
#include <SDL2/SDL.h>
#include <cassert>
#include <chrono>
#include <ctime>
#include <fmt/chrono.h>
#include <string>
#include <thread>
#include <cmath>
#include "imageOptions/weather/weatherAPI.hpp"
#include "toxic_spokes/Serial/Serial.hpp"


int main(int argc, char const* const* argv) {
    CLI::App app{"ControlDot - application to control dot displays"};

    std::string serialConnection;
    std::size_t watchFace = 0;
    app.add_option("-d,--device", serialConnection, "Serial Connection Device")
      ->required()
      ->check(CLI::ExistingFile);
    app.add_option("-f,--face", watchFace, "Setting the Watchface");
    CLI11_PARSE(app, argc, argv);

    Display<ts::Serial> display(serialConnection);
    bool            foo{true};
    Image img;
    clockFaceWithDate sna;
    clockFace snu;
    WeatherAPI weatherApi{"2a03:4000:57:ff3:e4ec:5cff:fe63:1b77",80, 49.015255402948384, 9.206269726559652};

    //Image Size 28*24
    while(true) {
        img.clear();
        switch(watchFace){
            case 0:
            {
                img.merge(snu.getImage());
            }
            break;
            default:
            {
                img.merge(sna.getImage());
            }
        }
        display.setImage(img.data);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        auto const weatherData{weatherApi.getWeatherData()};
        if(weatherData.has_value()){
            auto const& w{*weatherData};
            //fmt::print("Temp: {}, Code: {}, Winddir: {}, Windsped: {}\n",
            //           w.temperature, w.weathercode, w.winddirection, w.windspeed);
        }
    }

    return 0;
}

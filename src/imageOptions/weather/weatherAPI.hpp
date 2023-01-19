//
// Weather Data by https://open-meteo.com/
// Created by patrick on 1/19/23.
//

#pragma once
#include <string>
#include "toxic_spokes/IP/Socket.hpp"
#include <mutex>
#include <chrono>
#include "fmt/format.h"
#include <thread>
#include <exception>
#include <span>
#include "nlohmann/json.hpp"

/*
 * "current_weather": {
        "temperature": -0.9,
        "time": "2023-01-19T21:00",
        "weathercode": 85,
        "winddirection": 214.0,
        "windspeed": 1.3
    },
 * */

struct WeatherObject{
    double temperature;
    int weathercode;
    double winddirection;
    double windspeed;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(WeatherObject, temperature, weathercode, winddirection, windspeed);
};

struct WeatherAPI{
    float const latitude;
    float const longitude;
    std::string const ipAddress;
    std::uint16_t const port;
    ts::TCP_ClientSocket socket;
    std::mutex  socket_mute;

    std::optional<WeatherObject> weatherObject{};

    explicit WeatherAPI(std::string ipAddress, std::uint16_t port, float latitude, float longitude):
    latitude{latitude},
    longitude{longitude},
    ipAddress{ipAddress},
    port{port},
    socket{ipAddress, port}{}

    void acquireData(){
            std::string sendString{fmt::format(FMT_STRING("GET /v1/forecast?latitude={}&longitude={}&current_weather=true HTTP/1.0\r\n"
                                                          "Host: api.open-meteo.com\r\n"
                                                          "Content-Length: 0\r\n\r\n"), latitude, longitude)};
            std::span sendSpan{sendString};
            socket.send(std::as_bytes(sendSpan));
    }

    std::optional<WeatherObject> getWeatherData(){
        std::lock_guard<std::mutex> lock{socket_mute};
        return weatherObject;
    }

    void apiHandler(){
        auto stopToken{apiThread.get_stop_token()};
        while(!stopToken.stop_requested()){
            socket = ts::TCP_ClientSocket{ipAddress,port};
            std::vector<char> message;
            static constexpr std::size_t bytesToCatch{50};
            try{
                acquireData();
                while(socket.is_valid()) {
                    if (socket.can_recv(std::chrono::milliseconds(1000))) {
                        std::size_t const oldSize{message.size()};
                        message.resize(oldSize+bytesToCatch);
                        std::span receiveSpan{message.begin()+oldSize, message.end()};
                        std::size_t const receivedBytes = socket.recv_nonblocking(std::as_writable_bytes(receiveSpan));
                        message.resize(oldSize+receivedBytes);
                    }
                }
            }
            catch (ts::Com_Transmit_Exception const& e){
                if(e.bytes != 0){
                    message.resize(message.size()-(bytesToCatch-e.bytes));
                }
                throw;
            }
            catch (std::exception const& e){
                fmt::print("{}\n", e.what());
            }

            fmt::print("Size: {}\n", message.size());
            //fmt::print("{}\n", std::string_view{reinterpret_cast<char const*>(message.data()), message.size()});
            auto const beginOfJson{std::find(message.begin(), message.end(), '{')};
            if(beginOfJson != message.end()){
                nlohmann::json j;
                j = nlohmann::json::parse(std::string_view{beginOfJson, message.end()});
                std::lock_guard<std::mutex> lock(socket_mute);
                weatherObject = j["current_weather"].get<WeatherObject>();
            }
            else{
                //TODO: schiefgegangen
            }

            std::this_thread::sleep_for(std::chrono::minutes(20));
        }
    }

    std::jthread apiThread{&WeatherAPI::apiHandler, this};
};
//https://api.open-meteo.com/v1/forecast?latitude=49.0157840820953&longitude=9.205805749415036&current_weather=true
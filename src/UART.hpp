#pragma once

#include "ControlCharacters.hpp"

#include <chrono>
#include <concepts>
#include <cstddef>
#include <fmt/format.h>
#include <fstream>
#include <functional>
#include <iterator>
#include <optional>
#include <span>
#include <sstream>
#include <thread>
#include <boost/crc.hpp>



template<typename Serial>
/*requires requires(
  Serial                                 s,
  std::byte*                             p,
  std::size_t                            ss,
  std::chrono::duration<int, std::milli> dur) {
    {Serial{std::string{}, speed_t{}}};
    {
        s.recv_nonblocking(p, ss)
        } -> std::same_as<std::size_t>;
    {
        s.send(p, ss)
        } -> std::same_as<void>;
    {
        s.can_recv(dur)
        } -> std::same_as<bool>;
}*/
struct UART {
    std::string                  _dev;
    Serial                       _conn{_dev, 115200};
    std::vector<std::byte>       _fifo{};
    static constexpr std::size_t MaxPackageSize{300};

    void sendData(std::span<const std::byte> data) {
        _conn.send_nonblocking(data);
    }

    void receiveThreadFunc() {
        std::stop_token st{_receiveThread.get_stop_token()};
        while(!st.stop_requested()) {
            try {
                //auto buffer = getPacket(std::chrono::milliseconds(500));
                //if(buffer) {
                    //TODO implement RX functionality
                //}
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            } catch(std::exception const& e) {
                fmt::print(stderr, "Error: {}\n", e.what());
                //_conn = Serial{_dev, B115200};
            }
        }
    }

    explicit UART(std::string const& dev) : _dev{dev} {}

    std::jthread _receiveThread{&UART::receiveThreadFunc, this};
};

//
// Created by patrick on 11/5/22.
//
#pragma once
#include "Panel.hpp"
#include "UART.hpp"
#include "aglio/packager.hpp"
#include "firmdot/src/BinaryUtility.hpp"
#include "firmdot/src/package.hpp"

#include <algorithm>
#include <cstddef>
#include <stop_token>
#include <thread>
#include <type_traits>

struct PacketCrc {
    using type = std::uint16_t;
    static type calc(std::span<std::byte const> s) {
        boost::crc_ccitt_type crc;
        crc.process_bytes(s.data(), s.size());
        return crc.checksum();
    }
};

using packager = aglio::Packager<aglio::CrcConfig<PacketCrc>>;

template<typename Serial>
struct Display {
    std::vector<Panel> panels;
    UART<Serial>       uart;

    explicit Display(std::string serialName) : uart(serialName) {
        panels.push_back(Panel(1));
        panels.push_back(Panel(2));
    }

    void setPixel(std::size_t x, std::size_t y, bool state) {
        if(y >= 24) {
            return;
        }
        std::size_t panelNo{x / 28};
        if(panelNo > panels.size()) {
            return;
        }
        std::size_t xPos{x % 28};
        panels[panelNo].setPixel(xPos, y, state);
    }

    void fill(bool state) {
        for(Panel& p : panels) {
            p.fill(state);
        }
    }

    void flush() {
        std::stop_token st{_flushThread.get_stop_token()};
        while(!st.stop_requested()) {
            for(auto const& p : panels) {
                package::Package txPack{};
                txPack.panelID = p.panelID;
                std::memcpy(txPack.matrixData.data(), p.panelData.data(), p.panelData.size());
                std::vector<std::byte> txBuffer;
                packager::pack(txBuffer, txPack);
                uart.sendData(txBuffer);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    void setImage(std::vector<std::uint8_t> const& image) {
        for(std::size_t x{}; x < 2*28; ++x) {
            for(std::size_t y{}; y < 24; ++y) {
                std::size_t panelNo{x / 28};
                if(panelNo > panels.size()) {
                    return;
                }
                std::size_t xPos{x%28};
                std::size_t currentPixel{y * 28*2 + x};
                std::size_t currentByte{currentPixel / 8};
                std::size_t currentPosInByte{currentPixel % 8};
                bool        state = binUtil::checkBit(image[currentByte], currentPosInByte);
                //fmt::print("Setting Pixel ({},{}) on panel {} to {}\n", xPos,y,panelNo,state);
                
                panels[panelNo].setPixel(xPos, y, state);
            }
        }
        //fmt::print("Image data: {}\n", fmt::join(image, ", "));
        //fmt::print("Panel 0 Data: {}\n", fmt::join(panels[0].panelData, ", "));
        //fmt::print("Panel 1 Data: {}\n", fmt::join(panels[1].panelData, ", "));
    }

    std::jthread _flushThread{&Display::flush, this};
};

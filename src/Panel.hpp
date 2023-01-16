#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include "fmt/format.h"
#include "../external/firmdot/src/BinaryUtility.hpp"

struct Panel {
    std::uint32_t                panelID;
    std::array<std::uint8_t, 84> panelData;

    Panel(std::uint32_t panelID) : panelID(panelID) {
        panelData.fill(0);
    }

    void fill(bool state){
        std::uint8_t fillingData = 0;
        if(state){
            fillingData = 0xFF;
        }
        else{
            fillingData = 0x00;
        }
        panelData.fill(fillingData);
    }

    void setPixel(std::size_t x, std::size_t y, bool state) {
        //fmt::print("Setting Pixel ({},{}) to {}\n", x,y,state);
        if(x >= 28) {
            return;
        }
        if(y >= 24) {
            return;
        }

        std::size_t currentPixel{y * 28 + x};
        std::size_t currentByte{currentPixel / 8};
        std::size_t currentPosInByte{currentPixel % 8};
        //fmt::print("Setting Byte {} on position {}\n", currentByte, currentPosInByte);
        if(state) {
            binUtil::setBit(panelData[currentByte], currentPosInByte);
        }
        else{
            binUtil::clearBit(panelData[currentByte], currentPosInByte);
        }
        return;
    }
};
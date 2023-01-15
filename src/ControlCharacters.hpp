//
// Created by patrick on 8/28/22.
//

#pragma once


#include <cstddef>

struct AsciiControl{
    static constexpr std::byte NUL{0x00};
    static constexpr std::byte SOH{0x01};
    static constexpr std::byte STX{0x02};
    static constexpr std::byte ETX{0x03};
    static constexpr std::byte EOT{0x04};
    static constexpr std::byte ENQ{0x05};
    static constexpr std::byte ACK{0x06};
    static constexpr std::byte BEL{0x07};
    static constexpr std::byte BS{0x08};
    static constexpr std::byte HT{0x09};
    static constexpr std::byte LF{0x0A};
    static constexpr std::byte VT{0x0B};
    static constexpr std::byte FF{0x0C};
    static constexpr std::byte CR{0x0D};
    static constexpr std::byte SO{0x0E};
    static constexpr std::byte SI{0x0F};
    static constexpr std::byte DLE{0x10};
    static constexpr std::byte DC1{0x11};
    static constexpr std::byte DC2{0x12};
    static constexpr std::byte DC3{0x13};
    static constexpr std::byte DC4{0x14};
    static constexpr std::byte NAK{0x15};
    static constexpr std::byte SYN{0x16};
    static constexpr std::byte ETB{0x17};
    static constexpr std::byte CAN{0x18};
    static constexpr std::byte EM{0x19};
    static constexpr std::byte SUB{0x1A};
    static constexpr std::byte ESC{0x1B};
    static constexpr std::byte FS{0x1C};
    static constexpr std::byte GS{0x1D};
    static constexpr std::byte RS{0x1E};
    static constexpr std::byte US{0x1F};
    static constexpr std::byte DEL{0x7F};
};
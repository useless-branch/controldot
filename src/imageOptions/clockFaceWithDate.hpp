//
// Created by patrick on 1/19/23.
//
#include "../Image.hpp"
#include <chrono>
#include <ctime>

struct clockFaceWithDate{
    Image img;
    Image const& getImage(){
        img.clear();
        auto timePoint =  std::chrono::system_clock::now();
        time_t tt = std::chrono::system_clock::to_time_t(timePoint);
        tm local_tm = *localtime(&tt);
        std::string timeString = fmt::format("{:02}:{:02}:{:02}", local_tm.tm_hour, local_tm.tm_min, local_tm.tm_sec);
        std::string dateString = fmt::format("{:04}-{:02}-{:02}", local_tm.tm_year + 1900,  local_tm.tm_mon + 1, local_tm.tm_mday);
        img.renderString(5,0,"main_font_12_bold", timeString);
        img.renderString(3,15,"main_font_9", dateString);
        return img;
    }
};


#pragma once

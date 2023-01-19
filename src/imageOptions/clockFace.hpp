//
// Created by patrick on 1/19/23.
//
#pragma once
struct clockFace{
    Image img;
    Image const& getImage(){
        img.clear();
        auto timePoint =  std::chrono::system_clock::now();
        time_t tt = std::chrono::system_clock::to_time_t(timePoint);
        tm local_tm = *localtime(&tt);
        std::string timeString = fmt::format("{:02}:{:02}", local_tm.tm_hour, local_tm.tm_min);
        img.renderString(3,4,"main_font_20_bold", timeString);
        return img;
    }
};
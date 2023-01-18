#include <vector>
#include <cstdint>
#include <string>
#include <cassert>
#include "mcufont.h"
#include "main_font_9.h"
#include "main_font_11.h"
#include "main_font_16.h"
#include "main_font_16_bold.h"
#include "main_font_12_bold.h"


struct Image {
    std::size_t               width;
    std::size_t               height;
    std::vector<std::uint8_t> data;
    Image()
      : width{28*2}  
      , height{24} 
      , data(width * height / 8, std::uint8_t{0x00}) {}

    void clear() { std::fill(data.begin(), data.end(), 0x00); }

    std::vector<std::uint8_t> merged(Image const& other) {
        std::vector<std::uint8_t> ret;
        ret.resize(data.size());
        for(std::size_t i = 0; i < ret.size(); ++i) {
            ret[i] = data[i] & other.data[i];
        }
        return ret;
    }

    void putPixel(std::size_t x, std::size_t y, bool value) {
        if(x >= width || y >= height) {
            return;
        }
        std::size_t currentPixel{y * 28*2 + x};
        std::size_t byteindex{currentPixel / 8};
        assert(data.size() > byteindex);
        std::size_t   bit   = currentPixel % 8;
        std::uint8_t& block = data[byteindex];
        if(value) {
            block |= 1 << bit;
        } else {
            block &= ~(1 << bit);
        }
    }
    void renderString(
      std::size_t        startX,
      std::size_t        startY,
      std::string const& fontS,
      std::string const& s) {
        std::size_t x = startX;
        std::size_t y = startY;

        mf_font_s const* font = mf_find_font(fontS.c_str());
        if(font == nullptr) {
            return;
        }

        auto cb = [](int16_t xx, int16_t yy, uint8_t count, uint8_t alpha, void* state) {
            Image& im = *reinterpret_cast<Image*>(state);
            while(count != 0) {
                im.putPixel(xx, yy, alpha != 0);
                --count;
                ++xx;
            }
        };

        for(auto c : s) {
            if(c == '\n') {
                y += font->line_height;
                x = startX;
            } else {
                auto cwidth = mf_character_width(font, c);
                if(cwidth + x < width) {
                    mf_render_character(font, x, y, c, cb, this);
                }
                x += cwidth;
            }
        }
    }
};
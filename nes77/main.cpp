#include <iostream>
#include <array>
#include <random>
#include <algorithm>
#include <fstream>
#include <png++/png.hpp>
#include <strings.h>

const uint16_t NO_COLOR = 0;
const uint16_t COLOR1 = 0b10;
const uint16_t COLOR2 = 0b100;
const uint16_t COLOR3 = 0b1000;
const uint16_t COLOR4 = 0b10000;
const uint16_t COLOR5 = 0b100000;
const uint16_t COLOR6 = 0b1000000;
const uint16_t COLOR7 = 0x80;
const uint16_t COLOR8 = 0x100;

#define SIDE_LEN 512
#define MAX_LEN (SIDE_LEN-1)

struct graph {
    std::array<uint16_t, SIDE_LEN*SIDE_LEN> colors;

    graph() : colors() {

    }

    uint16_t get_color(uint_fast16_t x, uint_fast16_t y) {
        return colors[x + SIDE_LEN * y];
    }

    void set_color(uint_fast16_t x, uint_fast16_t y, uint16_t inp) {
        colors[x + SIDE_LEN * y] = inp;
    }

    uint16_t find_first_available_color(uint32_t col) {
        return (1 << (__builtin_ffs(col) - 1));
    }

    uint16_t get_available_colors(uint_fast16_t x, uint_fast16_t y) {
        uint16_t available_colors = 0x1FE;

        if (x != 0) {
            available_colors &= ~get_color(x-1, y);

            if (y != 0) {
                available_colors &= ~get_color(x-1, y-1);
            }

            if (y != MAX_LEN) {
                available_colors &= ~get_color(x-1, y+1);
            }
        }

        if (y != 0) {
            available_colors &= ~get_color(x, y-1);
        }

        if (y != MAX_LEN) {
            available_colors &= ~get_color(x, y+1);
        }

        if (x != MAX_LEN) {
            available_colors &= ~get_color(x + 1, y);

            if (y != 0) {
                available_colors &= ~get_color(x+1, y-1);
            }

            if (y != MAX_LEN) {
                available_colors &= ~get_color(x+1, y + 1);
            }
        }

        return available_colors;

    }

    void color_graph(uint64_t seed) {
        std::fill(colors.begin(), colors.end(), NO_COLOR);

        std::vector<uint32_t> nodes_to_check;


        nodes_to_check.reserve(SIDE_LEN*SIDE_LEN);
        for (uint32_t x = 0; x < SIDE_LEN; x++) {
            for (uint32_t y = 0; y < SIDE_LEN; y++) {
                nodes_to_check.push_back((x << 16) | y);
            }
        }

#ifndef NORANDOM
        std::minstd_rand0 rand(seed);
        std::shuffle(nodes_to_check.begin(), nodes_to_check.end(), rand);
#endif

        for (uint32_t coord : nodes_to_check) {
            uint_fast16_t x = (uint_fast16_t) coord >> 16;
            uint_fast16_t y = (uint_fast16_t) coord & 0xFFFF;

            auto cols = get_available_colors(x, y);
            auto col = static_cast<uint16_t>(find_first_available_color(cols));
            set_color(x, y, col);
        }
    }

    bool valid_coloring() {
        for (uint32_t x = 0; x < SIDE_LEN; x++) {
            for (uint32_t y = 0; y < SIDE_LEN; y++) {
                if (get_color(x, y) == 0u) {
                    return false;
                }
            }
        }

        return true;
    }
};

uint32_t color_conv(uint16_t color) {
    switch (color) {
        case COLOR1:
            return 0x17252AFF;
        case COLOR2:
            return 0x2B7A78FF;
        case COLOR3:
            return 0x3AAFA9FF;
        case COLOR4:
            return 0x9FD9D7FF;
        case COLOR5:
            return 0xDEF2F1FF;
        case COLOR6:
            return 0xEEEEEEFF;
        case COLOR7:
            return 0xB31B1BFF;
        case COLOR8:
            return 0x222222FF;

        default:
            throw 1;
    }
}

int main() {
    graph g;

    uint16_t t = 0;
    g.color_graph(t++);
    while (!g.valid_coloring()) {
        std::cout << "Try " << t << std::endl;
        g.color_graph(t++);
    }

    png::image<png::rgba_pixel> image(SIDE_LEN, SIDE_LEN);

    for (uint32_t x = 0; x < SIDE_LEN; x++) {
        for (uint32_t y = 0; y < SIDE_LEN; y++) {
            uint32_t col = g.get_color(x, y);
            col = color_conv(static_cast<uint16_t>(col));
            image.set_pixel(x, y, png::rgba_pixel(static_cast<png::byte>(col >> 24), static_cast<png::byte>(col >> 16),
                                                  static_cast<png::byte>(col >> 8), static_cast<png::byte>(col)));
        }
    }


    image.write("result.png");


}
#ifndef IMAGE_HPP
#define IMAGE_HPP
#include <string>
#include <cstdio>
#include <cstdint>

using namespace std;

class Color {
    public:
        Color();
        Color(uint8_t r, uint8_t g, uint8_t b);
        uint8_t r;
        uint8_t g;
        uint8_t b;
};


class Palette {
    public:
        Palette(uint64_t length);
        void setColor(uint64_t index, Color color);
        Color getColor(double index);
    private:
        uint64_t length;
        Color *colors;
};


class Image {
    public:
        Image(uint64_t width, uint64_t height);
        void setPixel(uint64_t x, uint64_t y, const Color& c);
        void saveImage(string filename);
    private:
        uint64_t width;
        uint64_t height;
        Color *pixels;

};
#endif

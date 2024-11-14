#ifndef IMAGE_H
#define IMAGE_H
#include <string>
#include <cstdint>
#include <cstddef>


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
        Palette(size_t length);
        void setColor(size_t index, Color color);
        void setRGB(size_t index, uint8_t r, uint8_t g, uint8_t b);
        Color getColor(double index);
    private:
        size_t length;
        double *linear_colors;
};


class Image {
    public:
        Image(uint64_t width, uint64_t height);
        void setPixel(uint64_t x, uint64_t y, const Color& c);
        void saveImage(std::string filename);
        uint64_t getWidth();
        uint64_t getHeight();
    private:
        uint64_t width;
        uint64_t height;
        uint8_t *pixels;
};
#endif

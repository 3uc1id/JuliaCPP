#ifndef IMAGE_H
#define IMAGE_H
#include <string>
#include <cstdint>
#include <cstddef>
#include <vector>


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
        void addRGB(uint8_t r, uint8_t g, uint8_t b);
        void addColor(Color rgb);
        Color getColor(double index);
    private:
        size_t rgb_length = 0;
        std::vector<double> linear_colors;
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

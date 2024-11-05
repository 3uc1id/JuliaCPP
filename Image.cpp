#include "Image.hpp"


Color::Color(){
    this->r = 0;
    this->g = 0;
    this->b = 0;
}


Color::Color(uint8_t r, uint8_t g, uint8_t b){
    this->r = r;
    this->g = g;
    this->b = b;
}


Palette::Palette(uint64_t length){
    this->length = length;
    this->colors = new Color[length];
}


void Palette::setColor(uint64_t index, Color color) {
    colors[index] = color;
}


static uint8_t mix(uint8_t a, uint8_t b, double f) {
    return a + (b - a)*f;
}


Color Palette::getColor(double index){
    uint64_t index_a = (uint64_t) index;
    uint64_t index_b = (index_a + 1) % length;
    double fraction = index - index_a;
    uint8_t r = mix(colors[index_a].r, colors[index_b].r, fraction);
    uint8_t g = mix(colors[index_a].g, colors[index_b].g, fraction);
    uint8_t b = mix(colors[index_a].b, colors[index_b].b, fraction);
    return Color(r, g, b);
}


Image::Image(uint64_t width, uint64_t height){
    this->width = width;
    this->height = height;
    this->pixels = new Color[width*height];
}


void Image::setPixel(uint64_t x, uint64_t y, const Color& color){
    
}
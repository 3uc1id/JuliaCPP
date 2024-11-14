#include "Image.h"

#include <fstream>
#include <iostream>

using namespace std;

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


Palette::Palette(size_t length){
    this->length = length;
    this->linear_colors = new double[3*length];
}


void Palette::setRGB(size_t index, uint8_t r, uint8_t g, uint8_t b){
    size_t idx = 3*index;
    linear_colors[idx++] = pow(r/255.0, 2.2);
    linear_colors[idx++] = pow(g/255.0, 2.2);
    linear_colors[idx] = pow(b/255.0, 2.2);
}


void Palette::setColor(size_t index, Color color) {
    // Gama correction
    this->setRGB(index, color.r, color.g, color.b);
}


static double mix(double a, double b, double f) {
    return a + (b - a)*f;
}


Color Palette::getColor(double index){
    uint64_t index_a = ((uint64_t) index) % length;
    uint64_t index_b = (index_a + 1) % length;
    double fraction = index - index_a;
    double r = mix(linear_colors[3*index_a], linear_colors[3*index_b], fraction);
    double g = mix(linear_colors[3*index_a+1], linear_colors[3*index_b+1], fraction);
    double b = mix(linear_colors[3*index_a+2], linear_colors[3*index_b+2], fraction);
    return Color(255*pow(r, 1/2.2), 255*pow(g, 1/2.2), 255*pow(b, 1/2.2));
}


Image::Image(uint64_t width, uint64_t height){
    this->width = width;
    this->height = height;
    this->pixels = new uint8_t[3*width*height];
}


void Image::setPixel(uint64_t x, uint64_t y, const Color& color){
    uint64_t idx = 3*(y*width + x);
    pixels[idx++] = color.r; 
    pixels[idx++] = color.g;
    pixels[idx]   = color.b;
}


void Image::saveImage(string filename) {
    // Save pixel data as PPM image
    ofstream output(filename, ios::out | ios::binary);
    output << "P6\n" << width << ' ' << height << "\n255\n";
    output.write((char*)pixels, 3*width*height);
    output.close();
}


uint64_t Image::getWidth() {
    return width;
}


uint64_t Image::getHeight() {
    return height;
}
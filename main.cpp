#include "Image.h"
#include "Julia.h"
#include <iostream>
#include <vector>
#include <thread>
#include <random>

// current example
//  ./JuliaCPP 2500 8 900 1600 100.0 0.35 0.35 -2.0 1.125 2.0 -1.125 Julia.ppm

using namespace std;

static bool isClose(double a, double b, double epsilon) {
    return abs(a - b) < epsilon;
}


int main(int argc, char **argv){
    // default values. Not const because they might be updated by options.
  //uint32_t iterations = 2500;
  //uint32_t threads = 8;
  //size_t height = 900;
  //size_t width = 1600;
  //    complex<double> c(0.35, 0.35);
  //complex<double> top_left(-2.0,1.125);
  //complex<double> bottom_right(2.0,-1.125);
  //string filename = "Julia.ppm";
    //double radius = 100.0;
  //size_t random_palette = 0;

    uint32_t iterations2 = atoi(argv[1]);
    uint32_t threads2 = atoi(argv[2]);
    size_t height2 = atoi(argv[3]);
    size_t width2 = atoi(argv[4]);
    double radius2 = atof(argv[5]);

    double c1 = atof(argv[6]);
    double c2 = atof(argv[7]);
    complex<double> c3(c1, c2);

    double top_left1 = atof(argv[8]);
    double top_left2 = atof(argv[9]);
    complex<double> top_left3(top_left1, top_left2);

    double bottom_right1 = atof(argv[10]);
    double bottom_right2 = atof(argv[11]);
    complex<double> bottom_right3(bottom_right1, bottom_right2);

    string filename2 = argv[12];

    size_t random_palette = atoi(argv[13]);

    cout << "Generating image with the following parameters:" << endl;
    cout << "Iterations: " << iterations2 << endl;
    cout << "Pixel Width: " << width2 << endl;
    cout << "Pixel Height: " << height2 << endl;
    cout << "Threads: " << threads2 << endl;
    cout << "Complex parameter: " << c3 << endl;
    cout << "Top left coordinate: " << top_left3 << endl;
    cout << "Bottom right coordinate: " << bottom_right3 << endl;
    cout << "Image Filename: " << filename2 << endl;
    cout << "Escape Radius: " << radius2 << endl;
    if(random_palette) {
        cout << "Random Palette length: " << random_palette << endl;
    }

    double plane_aspect_ratio = (bottom_right3.real() - top_left3.real()) / (top_left3.imag() - bottom_right3.imag());
    double image_aspect_ratio = (double)width2 / (double)height2;
    const double eps = 0.01;
    if(!isClose(plane_aspect_ratio, image_aspect_ratio, eps)){
        cout << "Warning: plane and image has different aspect ratios." << endl;
        cout << "Image may look squished as a result" << endl;
    }

    Palette palette;
    if(random_palette) {
        random_device dev;
        mt19937 rng(dev());
        uniform_int_distribution<mt19937::result_type> dist(0, 255);
        vector<Color> colors;
        for(int i=0; i < random_palette; i++){
            colors.emplace_back(dist(rng), dist(rng), dist(rng));
        }
        for(Color &c : colors) {
            palette.addColor(c);
        }
        // add the colors in the reverse sequence to avoid abrupt color shifts from end to start
        for(size_t i=colors.size() - 2; 0 < i; i--){
            palette.addColor(colors[i]);
        }
    }
    else {
        // Default color palette
        palette.addRGB(0x64, 0x11, 0x3F);
        palette.addRGB(0xDE, 0x4D, 0x86);
        palette.addRGB(0xF2, 0x9C, 0xA3);
        palette.addRGB(0xF7, 0xCA, 0xCD);
        palette.addRGB(0xDF, 0xF8, 0xEB);
        palette.addRGB(0xF7, 0xCA, 0xCD);
        palette.addRGB(0xF2, 0x9C, 0xA3);
        palette.addRGB(0xDE, 0x4D, 0x86);
    }

    Image img(width2, height2);
    JuliaParams params = {
        .c=c3,
        .escape_radius=radius2,
        .iterations=iterations2,
        .img=img,
        .p=palette,
        .top_left=top_left3,
        .bottom_right=bottom_right3,
    };
    vector<thread> workers;

    for(int i=0; i < threads2; i++){
        workers.emplace_back(julia_worker, &params, threads2, i);
    }

    // wait for all workers to finish
    for(auto &t : workers) {
        t.join();
    }

    img.saveImage(filename2);
    return 0;
}

#include "Image.h"
#include "Julia.h"
#include "OptionParser.h"
#include <iostream>
#include <vector>
#include <thread>
#include <random>


using namespace std;

template <typename T> void checkOption(OptionParser p, string longName, T &out) {
    if(!p.getOptionValue(longName, out)){
        cout << "Using default value for option --" << longName << endl;
    }
}


int main(int argc, char **argv){
    // default values
    uint32_t iterations = 2500;
    uint32_t threads = 8;
    size_t height = 900;
    size_t width = 1600;
    complex<double> c(0.35, 0.35);
    complex<double> top_left(-2.0,1.125);
    complex<double> bottom_right(2.0,-1.125);
    string filename = "Julia.ppm";
    double radius = 100.0;
    size_t random_palette = 0;

    OptionParser parser("JuliaCPP");
    parser.addDescription("Generates images of the Julia set and saves them in PPM format.");
    parser.addOption('I', "iterations", "Set the number of iterations per pixel. This should be a positive integer.", UINT32_T_TYPE);
    parser.addOption('W', "width", "Set the pixel width of the image.", SIZE_T_TYPE);
    parser.addOption('H', "height", "Set the pixel height of the image.", SIZE_T_TYPE);
    parser.addOption('C', "cparam", "Set the complex parameter used in the Julia iteration.", COMPLEX_DOUBLE_TYPE);
    parser.addOption('T', "topleft", "Set the top left coordinate of the area in the complex plane.", COMPLEX_DOUBLE_TYPE);
    parser.addOption('B', "bottomright", "Set the bottom right coordinate of the area in the complex plane.", COMPLEX_DOUBLE_TYPE);
    parser.addOption('N', "threads", "Set the number of threads used for image rendering.", UINT32_T_TYPE);
    parser.addOption('O', "filename", "Set the name of the image file. The extension must be .ppm .", STRING_TYPE);
    parser.addOption('R', "radius", "Set the escape radius of a point.", DOUBLE_TYPE);
    parser.addOption('r', "randompalette", "Set the length of a random color palette to use with the image.", SIZE_T_TYPE);
    parser.addEpilog("Note: complex parameters must be given as real,imag . For example, 1.0+1.0i should be entered as 1.0,1.0");
    
    if(!parser.parseOptions(argc, argv)){
        return 0;
    }

    checkOption(parser, "iterations", iterations);
    checkOption(parser, "threads", threads);
    checkOption(parser, "height", height);
    checkOption(parser, "width", width);
    checkOption(parser, "cparam", c);
    checkOption(parser, "topleft", top_left);
    checkOption(parser, "bottomright", bottom_right);
    checkOption(parser, "filename", filename);
    checkOption(parser, "radius", radius);
    checkOption(parser, "randompalette", random_palette);

    cout << "Generating image with the following parameters:" << endl;
    cout << "Iterations: " << iterations << endl;
    cout << "Pixel Width: " << width << endl;
    cout << "Pixel Height: " << height << endl;
    cout << "Threads: " << threads << endl;
    cout << "Complex parameter: " << c << endl;
    cout << "Top left coordinate: " << top_left << endl;
    cout << "Bottom right coordinate: " << bottom_right << endl;
    cout << "Image Filename: " << filename << endl;
    cout << "Escape Radius: " << radius << endl;
    if(random_palette) {
        cout << "Random Palette length: " << random_palette << endl;
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



    Image img(width, height);
    JuliaParams params = {
        .c=c,
        .escape_radius=radius,
        .iterations=iterations,
        .img=img,
        .p=palette,
        .top_left=top_left,
        .bottom_right=bottom_right,
    };
    vector<thread> workers;

    for(int i=0; i < threads; i++){
        workers.emplace_back(julia_worker, &params, threads, i);
    }

    // wait for all workers to finish
    for(auto &t : workers) {
        t.join();
    }

    img.saveImage(filename);
    return 0;
}
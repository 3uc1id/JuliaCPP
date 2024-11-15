#include "Image.h"
#include "Julia.h"
#include "OptionParser.h"
#include <iostream>
#include <vector>
#include <thread>


using namespace std;


int main(int argc, char **argv){
    // default values
    uint32_t iterations = 1000;
    uint32_t threads = 8;
    size_t heigh = 900;
    size_t width = 1600;
    complex<double> c(0.35, 0.35);
    complex<double> top_left(-1.6,0.9);
    complex<double> bottom_right(1.6,-0.9);
    string filename = "Julia.ppm";

    OptionParser parser("JuliaCPP");
    parser.addDescription("Generates images of the Julia set and saves them in PPM format.");
    parser.addOption('I', "iterations", "Set the number of iterations per pixel. This should be a positive integer.");
    parser.addOption('W', "width", "Set the pixel width of the image.");
    parser.addOption('H', "height", "Set the pixel height of the image.");
    parser.addOption('C', "cparam", "Set the complex parameter used in the Julia iteration.");
    parser.addOption('T', "topleft", "Set the top left coordinate of the area in the complex plane.");
    parser.addOption('B', "bottomright", "Set the bottom right coordinate of the area in the complex plane.");
    parser.addOption('N', "threads", "Set the number of threads used for image rendering.");
    parser.addOption('O', "filename", "Set the name of the image file. The extension must be .ppm .");
    
    if(!parser.parseOptions(argc, argv)){
        return 0;
    }

    string optionVal;
    optionVal = parser.getOptionValue('I');
    if(optionVal.length()){
        
    }

    cout << "Generating image with the following parameters:" << endl;
    cout << "Iterations: " << options.iterations << endl;
    cout << "Pixel Width: " << options.pixel_width << endl;
    cout << "Pixel Height: " << options.pixel_height << endl;
    cout << "Threads: " << options.thread_count << endl;
    cout << "Complex parameter: " << options.c << endl;
    cout << "Top left coordinate: " << options.top_left << endl;
    cout << "Bottom right coordinate: " << options.bottom_right << endl;
    cout << "Image Filename: " << options.output_filename << endl;

    Palette palette(2); // TODO: let the users define a palette or generate a random one.
    palette.setRGB(0, 0x64, 0x11, 0x3F);
    palette.setRGB(1, 0xDE, 0x4D, 0x86);
    //palette.setRGB(2, 0xF2, 0x9C, 0xA3);
    //palette.setRGB(3, 0xF7, 0xCA, 0xCD);
    //palette.setRGB(4, 0xDF, 0xF8, 0xEB);
    //palette.setRGB(5, 0xF7, 0xCA, 0xCD);
    //palette.setRGB(6, 0xF2, 0x9C, 0xA3);
    //palette.setRGB(7, 0xDE, 0x4D, 0x86);


    Image img(options.pixel_width, options.pixel_height);
    JuliaParams params = {
        .c=options.c,
        .escape_radius=1000.0,
        .iterations=options.iterations,
        .img=img,
        .p=palette,
        .top_left=options.top_left,
        .bottom_right=options.bottom_right,
    };
    vector<thread> workers;

    for(int i=0; i<options.thread_count; i++){
        workers.emplace_back(julia_worker, &params, options.thread_count, i);
    }

    // wait for all workers to finish
    for(auto &t : workers) {
        t.join();
    }

    img.saveImage(options.output_filename);
    return 0;
}
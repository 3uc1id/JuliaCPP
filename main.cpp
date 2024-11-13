#include "Image.h"
#include "Julia.h"
#include "Option.h"
#include <iostream>
#include <vector>
#include <thread>


using namespace std;

int main(int argc, char **argv){
    CLIOptions options = {
        // Set the default option values here.
        .iterations=100,
        .pixel_width=1600,
        .pixel_height=900,
        .thread_count=8,
        .c={-0.8,0.156},
        .top_left={-1.6,0.9},
        .bottom_right={1.6,-0.9},
        .output_filename="JuliaCPP.ppm"
    };

    if(!parseOptions(argc, argv, options)){
        return 1;
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

    Palette palette(5); // TODO: let the users define a palette or generate a random one.
    palette.setRGB(0, 0x64, 0x11, 0x3F);
    palette.setRGB(1, 0xDE, 0x4D, 0x86);
    palette.setRGB(2, 0xF2, 0x9C, 0xA3);
    palette.setRGB(3, 0xF7, 0xCA, 0xCD);
    palette.setRGB(4, 0xA4, 0xBF, 0xEB);

    Image img(options.pixel_width, options.pixel_height);
    JuliaParams params = {
        .c=options.c,
        .escape_radius=2.0,
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
#ifndef OPTION_H
#define OPTION_H

#include <string>
#include <complex>
#include <cstdint>



#ifndef EXE_NAME
const std::string NAME = "JuliaCPP";
#else
const string NAME = EXE_NAME;
#endif

typedef struct CLIOptions_s {
    uint32_t iterations;
    uint32_t pixel_width;
    uint32_t pixel_height;
    uint32_t thread_count;
    std::complex<double> c;
    std::complex<double> top_left;
    std::complex<double> bottom_right;
    std::string output_filename;
} CLIOptions;

bool parseOptions(int argc, char **argv, CLIOptions &options);
#endif
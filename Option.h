#ifndef OPTION_H
#define OPTION_H
#include <cstdint>
#include <cstdlib>
#include <cerrno>
#include <climits>
#include <cstdio>
#include <cstring>
#include <complex>
#include <string>
#include <iostream>
#include <unistd.h>

using namespace std;

#ifndef EXE_NAME
const string NAME = "JuliaCPP";
#else
const string NAME = EXE_NAME;
#endif

typedef struct CLIOptions_s {
    uint32_t iterations;
    uint32_t pixel_width;
    uint32_t pixel_height;
    uint32_t thread_count;
    complex<double> c;
    complex<double> top_left;
    complex<double> bottom_right;
    string output_filename;
} CLIOptions;

bool parseOptions(int argc, char **argv, CLIOptions &options);
#endif
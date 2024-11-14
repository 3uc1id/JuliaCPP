#ifndef JULIA_H
#define JULIA_H
#include <complex>
#include <cstdint>
#include "Image.h"

const double LOG2 = 0.6931471805599453;

typedef struct JuliaParams_s {
    std::complex<double> c;
    double escape_radius;
    uint32_t iterations;
    Image &img;
    Palette &p;
    std::complex<double> top_left;
    std::complex<double> bottom_right;
} JuliaParams;


void julia_worker(JuliaParams *params, uint64_t row_step, uint64_t row_start);

#endif
#include "Julia.h"

void julia_worker(JuliaParams *params, uint64_t row_step, uint64_t row_start){
    /* 
    Worker for threading.
    Each pixel in the image represents the result of the Julia iteration
    at the point at the center of a square cell on the complex plane.

                                    cell width
     top_left                      /
    /                   |---------|
    +---------+---------+---------+---------+
    |         |         |         |         |
    |    *    |    *    |    *    |    *    |         ^
    |         |         |         |         |         |
    +---------+---------+---------+---------+   imaginary axis
    |         |         |         |         |         |
    |    *    |    *    |    *    |    *    |         v
    |         |         |         |         |
    +---------+---------+---------+---------+
                <-- real axis -->            \
                                              bottom right


    The * in each cell represents the point tested.
    Since the points are in the center of the cells,
    their numerical values are offset by half the cell width,
    but they are still separated by one cell width.

    In order for the cells to be squares, the following must be true:
       
    (real_end - real_start) / (imag_end - imag_start) = pixel_width / pixel_height

    So display aspect ratio should be kept in mind. For common displays the
    aspect ratio is usually 16:9 or 4:3. 

    */

    double real_start = params->top_left.real();
    double real_end = params->bottom_right.real();
    double cell_width = (real_end - real_start)/params->img.getWidth();

    /*  
        Parallelism is achieved by assigning each thread non-overlapping rows to compute.
        Thread t_i starts by working on row i, then works on row i+n where n is the
        number of threads running.
    */
    double i_part = params->top_left.imag() - cell_width/2.0;
    Color interior; // default is black
    for(uint64_t row=row_start; row < params->img.getHeight(); row+=row_step){
        double real_part = real_start + cell_width/2.0;
        for(uint64_t col=0; col < params->img.getWidth(); col++){
            complex<double> z(real_part, i_part);
            uint32_t i=0;
            for(; i < params->iterations && abs(z) < params->escape_radius; i++){
                z = z*z + params->c;
            }
            if(abs(z) >= params->escape_radius){
                // cout << '.';
                double smoothed_iteration = i + 1 - log(log(abs(z)))/LOG2;
                Color c = params->p.getColor(smoothed_iteration);
                params->img.setPixel(col, row, c);
            }
            else {
                cout << '+';
                params->img.setPixel(col, row, interior);
            }
            real_part += cell_width;
        }
        i_part += cell_width*row_step;
    }
}

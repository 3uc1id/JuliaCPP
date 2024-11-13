#include "Option.h"


static void usage() {
    const string usage_string = " [-I iterations] [-W pixel_width] [-H pixel_height] [-C complex_parameter] [-T top_left] [-B bottom_right] [-N threads] [-O filename] [-h]";
    const string option_details = R"(
Options Details:
    -h Show this help message.
    -I Set the number of iterations poer pixel. Should be a positive int.
    -W Set the pixel width of the image.
    -H Set the pixel height of the image.
    -C Set the complex parameter used for the iteration.
    -T Set the top left coordinate of the area in the complex plane.
    -B Set the bottom right coordinate of the area in the complex plane.
    -N Set the number of threads to use for image rendering.
    -O Set the filename used to save the image. The extension must be PPM.

Note: complex values should be entered as real,imaginary. For example 1.0+1.0i should be entered as 1.0,1.0
)";
    cout << NAME << usage_string << endl;
    cout << option_details;
}


static bool parse_uint32_t(const char* str, char name, uint32_t &value){
    char *end = nullptr;
    errno = 0;
    uint32_t result = strtoul(str, &end, 0);
    if(errno == ERANGE) {
        fprintf(stderr, "Value for option -%c is too large.\n", name);
        return false;
    }
    else if(errno == EINVAL) {
        fprintf(stderr, "Value for option -%c has no valid characters.\n", name);
        return false;
    }
    else if(*str != '\0' && *end == '\0') {
        value = result;
        return true;
    }
    else {
        fprintf(stderr, "Value for option -%c contains invalid characters.\n", name);
        return false;
    }
}


static bool parse_double(const char *str, char name, double &value) {
    char *end = nullptr;
    errno = 0;
    double result = strtod(str, &end);
    if((errno == ERANGE) && (result == HUGE_VAL)){
        fprintf(stderr, "Value for option -%c is too big.\n", name);
        return false;
    }
    else if (errno == ERANGE){
        fprintf(stderr, "Value for option -%c is too small.\n", name);
        return false;
    }
    value = result;
    return true;
}


static bool parse_complex_double(const char *str, char name, complex<double> &value) {
    size_t input_len = strlen(str);
    char *input = (char *)calloc(input_len, 1);
    memcpy(input, str, input_len);
    char *part = strtok(input, ",");
    double real_part;
    if(!parse_double(part, name, real_part))
        return false;
    part = strtok(nullptr, ",");
    double imag_part;
    if(!parse_double(part, name, imag_part))
        return false;
    value = {real_part, imag_part};
    free(input);
    return true;
}


bool parseOptions(int argc, char **argv, CLIOptions &options) {
    const char *params = ":I:W:H:C:T:B:O:h";
    int ch;
    bool result = true;
    while((ch = getopt(argc, argv, params)) != -1){
        switch(ch) {
            case 'I':
                result &= parse_uint32_t(optarg, ch, options.iterations);
                break;
            case 'W':
                result &= parse_uint32_t(optarg, ch, options.pixel_width);
                break;
            case 'H':
                result &= parse_uint32_t(optarg, ch, options.pixel_height);
                break;
            case 'N':
                result &= parse_uint32_t(optarg, ch, options.thread_count);
                break;
            case 'C':
                result &= parse_complex_double(optarg, ch, options.c);
                break;
            case 'T':
                result &= parse_complex_double(optarg, ch, options.top_left);
                break;
            case 'B':
                result &= parse_complex_double(optarg, ch, options.bottom_right);
                break;
            case 'O':
                options.output_filename = optarg;
                break;
            case 'h':
                usage();
                result &= false;
                break;
            case ':':
                fprintf(stderr, "Missing value for option -%c.\n", optopt);
                result &= false;
                break;
            case '?':
                fprintf(stderr, "Undefined option detected: -%c.\n", optopt);
                result &= false;
                break;
            default:
                break;
        }
    }
    return result;
}
#ifndef OPTIONPARSER_H
#define OPTIONPARSER_H

#include <cstddef>
#include <string>
#include <complex>
#include <map>
#include <vector>

const size_t MAX_LONGNAME_LENGTH = 20;

enum OptionType {
    DOUBLE_TYPE,
    COMPLEX_DOUBLE_TYPE,
    SIZE_T_TYPE,
    UINT32_T_TYPE,
    STRING_TYPE,
};

class OptionParser {
    public:
        OptionParser(std::string progName);
        void addDescription(std::string description);
        void addEpilog(std::string epilog);
        bool addOption(char shortName, std::string longName, std::string helpMsg, enum OptionType t);
        bool parseOptions(int argc, char **argv);
        bool getOptionValue(std::string longName, std::string &out);
        bool getOptionValue(std::string longName, double &out);
        bool getOptionValue(std::string longName, std::complex<double> &out);
        bool getOptionValue(std::string longName, size_t &out);
        bool getOptionValue(std::string longName, uint32_t &out);
    private:
        struct Private;
    
        std::string progName;
        std::string description;
        std::string epilog;
        std::map<std::string, char> longToShort;
        std::vector<struct option> options;
        std::string shortOpts;
        std::map<std::string, std::string> helpMessages;
        std::map<char, enum OptionType> optionTypes;

        std::map<char, std::string> stringOptions;
        std::map<char, double> doubleOptions;
        std::map<char, std::complex<double>> complexDoubleOptions;
        std::map<char, size_t> sizetOptions;
        std::map<char, uint32_t> uint32tOptions;
};

bool parseComplexDouble(std::string val, std::complex<double> &out);
bool parseUint32t(std::string val, uint32_t &out);
bool parseDouble(std::string val, double &out);

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

// bool parseOptions(int argc, char **argv, CLIOptions &options);
#endif
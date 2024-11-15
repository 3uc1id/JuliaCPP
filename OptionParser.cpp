#include "OptionParser.h"
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <iostream>
#include <getopt.h>


using namespace std;

static bool parse_size_t(const char* str, char name, size_t &value){
    char *end = nullptr;
    errno = 0;
    size_t result = strtoul(str, &end, 0);
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


static bool parse_uint32_t(const char *str, char name, uint32_t &value){
    size_t result;
    if(parse_size_t(str, name, result) && (result <= UINT32_MAX)){
        value = (uint32_t)result;
        return true;
    }
    else {
        fprintf(stderr, "Value for option -%c is too large.\n", name);
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
    char *input = (char *)calloc(input_len+1, 1);
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


struct OptionParser::Private {
    static void usage(OptionParser &self) {
        // Prints a help message generated from the option data supplied.
        cout << self.progName << ' ';
        for(const auto &kv : self.longToShort) {
            cout << "[-" << kv.second << ' ';
            for(const auto &c : kv.first) {
                cout << (char)toupper(c);
            }
            cout << "] ";
        }
        cout << endl;
        if(self.description.length()){
            cout << self.description << endl;
        }
        cout << endl;
        cout << "Option Details:" << endl;

        for(const auto &kv : self.longToShort) {
            cout << "  ";
            cout << '-' << kv.second << ' ';
            cout << "--" << kv.first;
            for(int i=0; i< (MAX_LONGNAME_LENGTH - kv.first.length() + 1UL); i++){
                cout << ' ';
            }
            cout << self.helpMessages[kv.first] << endl;
        }
        if(self.epilog.length()){
            cout << endl << self.epilog << endl;
        }
    }
};

OptionParser::OptionParser(string progName) {
    // Initialize the OptionParser with a name that is shown in the help message.
    this->progName = progName;
    // By default any OptionParser can print a help message,
    // so their needs to be a corresponding option for getopt_long.
    struct option helpOp = {
        .name = "help",
        .has_arg = no_argument,
        .flag = nullptr,
        .val = 'h'
    };
    options.push_back(helpOp);
    // The short option string for getopt_long.
    // Leading + tells getopt_long to stop processing when an error is encountered.
    // Leading : (after the +) tells getopt_long to signal the difference bewteen a
    // missing expected argument and an unknown argument.
    shortOpts += "+:h";
}

void OptionParser::addDescription(string description) {
    // Add an optional description to the help output.
    this->description = description;
}

void OptionParser::addEpilog(string epilog) {
    this->epilog = epilog;
}

bool OptionParser::addOption(char shortName, string longName, string helpMsg, enum OptionType t) {
    // adds an option to be parsed.
    // Example : parser.addOption('f', "foo", "Computes foo.");
    // This means the parser expects either -f or --foo in the arguments.
    if(longToShort.find(longName) != longToShort.end()){
        return false;
    }
    if(longName.length() > MAX_LONGNAME_LENGTH){
        return false;
    }
    struct option op = {
        .name = longName.c_str(),
        .has_arg = required_argument,
        .flag = nullptr,
        .val = shortName,
    };
    helpMessages[longName] = helpMsg;
    longToShort[longName] = shortName;
    optionTypes[shortName] = t;
    options.push_back(op);
    shortOpts += shortName;
    shortOpts += ':';
    return true;
}


bool OptionParser::parseOptions(int argc, char **argv){
    struct option blankOption = {
        .name = nullptr,
        .has_arg = 0,
        .flag = nullptr,
        .val = 0
    };
    options.push_back(blankOption);

    int ch;
    const char *shortOptsCStr = shortOpts.c_str();
    while((ch = getopt_long(argc, argv, shortOptsCStr, options.data(), nullptr)) != -1) {
        switch(ch){
            case ':':
                cout << "Missing value for option " << (char)optopt << endl;
                return false;
            case '?':
                cout << "Invalid option: " << (char)optopt << endl;
                return false;
            case 'h':
                Private::usage(*this);
                return false;
            default:
                break;
        }
        switch(optionTypes[ch]){
            case DOUBLE_TYPE: {
                double result;
                if(parse_double(optarg, ch, result)){
                    doubleOptions[ch] = result;
                }
                else {
                    return false;
                }
                break;
            }
            case COMPLEX_DOUBLE_TYPE: {
                complex<double> result;
                if(parse_complex_double(optarg, ch, result)){
                    complexDoubleOptions[ch] = result;
                }
                else {
                    return false;
                }
                break;
            }
            case SIZE_T_TYPE: {
                size_t result;
                if(parse_size_t(optarg, ch, result)){
                    sizetOptions[ch] = result;
                }
                else {
                    return false;
                }
                break;
            }
            case UINT32_T_TYPE: {
                uint32_t result;
                if(parse_uint32_t(optarg, ch, result)){
                    uint32tOptions[ch] = result;
                }
                else {
                    return false;
                }
                break;
            }
            case STRING_TYPE:
                // no parsing to do
                stringOptions[ch] = optarg;
                break;
            default:
                fprintf(stderr, "Invalid type enum for option %c.\n", ch);
                break;
        }
    }
    return true;
}


static bool lookUp(map<string,char> m, string k, char &out){
    auto findResult = m.find(k);
    if(findResult == m.end()){
        return false;
    }
    out = findResult->second;
    return true;
}


bool OptionParser::getOptionValue(string longName, string &out){
    char shortName;
    if(!lookUp(longToShort, longName, shortName)){
        return false;
    }
    auto result = stringOptions.find(shortName);
    if(result == stringOptions.end()){
        return false;
    }
    out = result->second;
    return true;
}


bool OptionParser::getOptionValue(string longName, double &out){
    char shortName;
    if(!lookUp(longToShort, longName, shortName)){
        return false;
    }
    auto result = doubleOptions.find(shortName);
    if(result == doubleOptions.end()){
        return false;
    }
    out = result->second;
    return true;
}


bool OptionParser::getOptionValue(string longName, complex<double> &out){
    char shortName;
    if(!lookUp(longToShort, longName, shortName)){
        return false;
    }
    auto result = complexDoubleOptions.find(shortName);
    if(result == complexDoubleOptions.end()){
        return false;
    }
    out = result->second;
    return true;
}


bool OptionParser::getOptionValue(string longName, size_t &out){
    char shortName;
    if(!lookUp(longToShort, longName, shortName)){
        return false;
    }
    auto result = sizetOptions.find(shortName);
    if(result == sizetOptions.end()){
        return false;
    }
    out = result->second;
    return true;
}


bool OptionParser::getOptionValue(string longName, uint32_t &out){
    char shortName;
    if(!lookUp(longToShort, longName, shortName)){
        return false;
    }
    auto result = uint32tOptions.find(shortName);
    if(result == uint32tOptions.end()){
        return false;
    }
    out = result->second;
    return true;
}

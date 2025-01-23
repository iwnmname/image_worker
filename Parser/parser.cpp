#include "parser.h"

Parser Parse(int argc, char** argv) {
    if (argc < 3) {
        throw std::invalid_argument(
            "Incorrect number of arguments. Path must look like:"
            "{program name} {path to input file} {path to output file} "
            "[-{filter name} [filter parameter 1] [filter parameter 2] ...] ");
    }
    Parser parsing_file;
    parsing_file.file_name_ = argv[0];
    parsing_file.file_in_ = argv[1];
    parsing_file.file_out_ = argv[2];
    for (auto i = 3; i < argc;) {
        if (argv[i][0] == '-') {
            std::string curr = argv[i];
            if (parsing_file.working_filters.find(curr) == parsing_file.working_filters.end()) {
                throw std::invalid_argument("Invalid filter");
            }
            parsing_file.filters_.push_back(curr);
            ++i;
            while (i < argc && argv[i][0] != '-') {
                parsing_file.filters_args_[curr].push_back(argv[i]);
                ++i;
            }
        }
    }
    return parsing_file;
}
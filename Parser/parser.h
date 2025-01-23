#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <stdexcept>
#include <map>
#include <set>

class Parser {
public:
    std::string file_name_;
    std::string file_in_;
    std::string file_out_;
    std::vector<std::string> filters_;
    std::map<std::string, std::vector<std::string>> filters_args_;
    std::set<std::string> working_filters = {"-crop", "-gs", "-neg", "-sharp", "-edge", "-blur"};
};

Parser Parse(int argc, char* argv[]);

#endif  // PARSER_H
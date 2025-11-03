#ifndef DATA_READER_H
#define DATA_READER_H


#include <string_view>
#include <vector>


std::vector<double> read_file(std::string_view file_name);
void parser_error(std::string_view argv0);
void print_help();

#endif

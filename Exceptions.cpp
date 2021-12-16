#include "Exceptions.h"

FileNotOpen::FileNotOpen(const std::string &file_name) {
    _message = "Error: file - " + file_name + " is not open";
    _file_name = file_name;
}

const char *FileNotOpen::what() {
    return _message.c_str();
}

const std::string& FileNotOpen::getFileName() {
    return _file_name;
}

InvalidLineFormat::InvalidLineFormat(size_t line_num) {
    _message = "Error: invalid line format (line - " + std::to_string(line_num) + ")";
    _line_num = line_num;
}

const char *InvalidLineFormat::what() {
    return _message.c_str();
}

size_t InvalidLineFormat::getLineNum() {
    return _line_num;
}
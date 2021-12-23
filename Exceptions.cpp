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

InvalidLineFormat::InvalidLineFormat(int line_num) {
    _message = "Error: invalid line format (line - " + std::to_string(line_num) + ")";
    _line_num = line_num;
}

const char *InvalidLineFormat::what() {
    return _message.c_str();
}

int InvalidLineFormat::getLineNum() {
    return _line_num;
}

IndexOutOfRange::IndexOutOfRange() {
    _message = "Error: index out of range";
}

const char *IndexOutOfRange::what() {
    return _message.c_str();
}

UnknownType::UnknownType(int line_number, int cell_number) {
    _message = "Error: unknown type of the cell in line - " + std::to_string(line_number) +
               ", cell - " + std::to_string(cell_number);
    _line_number = line_number;
    _cell_number = cell_number;
}

const char *UnknownType::what() {
    return _message.c_str();
}

int UnknownType::getLineNumber() {
    return _line_number;
}

int UnknownType::getCellNumber() {
    return _cell_number;
}

InvalidArgument::InvalidArgument(const std::string &func_name) {
    _message = "Error: invalid argument in function - " + func_name;
    _function_name = func_name;
}

const char *InvalidArgument::what() {
    return _message.c_str();
}

const std::string &InvalidArgument::getFunctionName() {
    return _function_name;
}
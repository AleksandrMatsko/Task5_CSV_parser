#ifndef TASK5_CSV_PARSER_EXCEPTIONS_H
#define TASK5_CSV_PARSER_EXCEPTIONS_H

#include <exception>
#include <string>

class FileNotOpen : public std::exception {
private:
    std::string _message;
    std::string _file_name;

public:
    FileNotOpen(const std::string& file_name);
    const char* what();
    const std::string& getFileName();
};

class InvalidLineFormat : public std::exception {
private:
    std::string _message;
    size_t _line_num;

public:
    InvalidLineFormat(size_t line_num);
    const char* what();
    size_t getLineNum();
};

#endif //TASK5_CSV_PARSER_EXCEPTIONS_H

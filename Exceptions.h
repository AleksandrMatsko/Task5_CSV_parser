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
    int _line_num;

public:
    InvalidLineFormat(int line_num);
    const char* what();
    int getLineNum();
};

class IndexOutOfRange : public std::exception {
private:
    std::string _message;

public:
    IndexOutOfRange();
    const char* what();
};

class UnknownType : public std::exception {
private:
    std::string _message;
    int _line_number;
    int _cell_number;

public:
    UnknownType(int line_number, int cell_number);
    const char* what();
    int getLineNumber();
    int getCellNumber();
};

class InvalidArgument : public std::exception {
private:
    std::string _message;
    std::string _function_name;

public:
    InvalidArgument(const std::string& func_name);
    const char* what();
    const std::string& getFunctionName();
};

#endif //TASK5_CSV_PARSER_EXCEPTIONS_H

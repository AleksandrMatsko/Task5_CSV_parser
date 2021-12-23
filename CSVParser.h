#ifndef TASK5_CSV_PARSER_CSVPARSER_H
#define TASK5_CSV_PARSER_CSVPARSER_H

#include "TuplePrinter.h"
#include "Exceptions.h"
#include "TupleAdder.h"
#include <fstream>

const char LINE_DELIMITER = '\n';
const char CELL_DELIMITER = ',';

template<typename... Types>
class CSVParser {
private:
    const std::string& _csv_file_name;
    int _lines_to_skip;
    int _max_num_lines;

    int getMaxNumLines(const std::string& file_name);

public:
    class Iterator {
    private:
        int _line_num;
        const std::string& _file_name;
        std::ifstream _in_it;
        std::tuple<Types...> _curr_tup;

        std::tuple<Types...> parseLine(std::string &str);

    public:
        explicit Iterator(const std::string& file_name, int line_num);

        const std::tuple<Types...>& operator*();
        const std::tuple<Types...>* operator->();
        Iterator& operator++();
        const Iterator operator++(int);
        bool operator==(const Iterator &it);
        bool operator!=(const Iterator &it);
    };

    explicit CSVParser(const std::string& file_name, int lines_to_skip = 0);

    template<typename... T>
    friend std::ostream& operator<<(std::ostream& out, const std::tuple<T...>& tuple);

    Iterator begin();
    Iterator end();
};

template<typename... Types>
std::tuple<Types...> CSVParser<Types...>::Iterator::parseLine(std::string &str) {
    std::tuple<Types...> tup;
    std::string tmp;
    std::istringstream stream(str);
    size_t index = 0;
    while (std::getline(stream, tmp, CELL_DELIMITER)) {
        if (index == sizeof...(Types)) {
            throw InvalidLineFormat(0);
        }
        TupleAdder<std::tuple<Types...>, std::string, sizeof...(Types) - 1>::add(tup, tmp, index);
        index += 1;
    }
    return tup;
}

template<typename... Types>
int CSVParser<Types...>::getMaxNumLines(const std::string &file_name) {
    std::ifstream in(file_name);
    if (!in.is_open()) {
        throw FileNotOpen(file_name);
    }
    std::string line;
    int ret = 0;
    while (std::getline(in, line, LINE_DELIMITER)) {
        ret += 1;
    }
    return ret;
}

template<typename... Types>
CSVParser<Types...>::CSVParser(const std::string& file_name, int lines_to_skip) :
                               _csv_file_name(file_name) {
    if (lines_to_skip < 0) {
        throw InvalidArgument("CSVParser<Types...>::CSVParser(const std::string&, int)");
    }
    _max_num_lines = getMaxNumLines(file_name);
    std::ifstream in(file_name);
    if (!in.is_open()) {
        throw FileNotOpen(_csv_file_name);
    }
    _lines_to_skip = lines_to_skip;
}

template<typename... Types>
CSVParser<Types...>::Iterator::Iterator(const std::string& file_name, int line_num) :
                                        _file_name(file_name), _in_it(file_name),
                                        _line_num(line_num) {
    if (!_in_it.is_open()) {
        throw FileNotOpen(_file_name);
    }
    std::string line;
    for (int i = 0; i < line_num; i++) {
        std::getline(_in_it, line, LINE_DELIMITER);
    }
    std::getline(_in_it, line, LINE_DELIMITER);
    try {
        _curr_tup = parseLine(line);
    }
    catch (InvalidLineFormat &ex) {
        throw InvalidLineFormat(_line_num);
    }
    catch (UnknownType &ex) {
        throw UnknownType(_line_num, ex.getCellNumber());
    }
    _line_num += 1;
}

template<typename... Types>
typename CSVParser<Types...>::Iterator CSVParser<Types...>::begin() {
    if (_lines_to_skip >= _max_num_lines) {
        return end();
    }
    return Iterator(_csv_file_name, _lines_to_skip);
}

template<typename... Types>
typename CSVParser<Types...>::Iterator CSVParser<Types...>::end() {
    return Iterator(_csv_file_name, _max_num_lines);
}

template<typename... Types>
const std::tuple<Types...>& CSVParser<Types...>::Iterator::operator*() {
    return _curr_tup;
}

template<typename... Types>
const std::tuple<Types...>* CSVParser<Types...>::Iterator::operator->() {
    return &_curr_tup;
}

template<typename... Types>
typename CSVParser<Types...>::Iterator& CSVParser<Types...>::Iterator::operator++() {
    if (!_in_it.is_open()) {
        throw FileNotOpen(_file_name);
    }
    std::string line;
    std::getline(_in_it, line, LINE_DELIMITER);
    try {
        _curr_tup = parseLine(line);
    }
    catch (InvalidLineFormat& ex) {
        throw InvalidLineFormat(_line_num);
    }
    catch (UnknownType& ex) {
        throw UnknownType(_line_num, ex.getCellNumber());
    }
    _line_num += 1;
    return *this;
}

template<typename... Types>
const typename CSVParser<Types...>::Iterator CSVParser<Types...>::Iterator::operator++(int) {
    Iterator tmp(_file_name, _line_num);
    *this++;
    return tmp;
}

template<typename... Types>
bool CSVParser<Types...>::Iterator::operator==(const Iterator &it) {
    return _line_num == it._line_num;
}

template<typename... Types>
bool CSVParser<Types...>::Iterator::operator!=(const Iterator &it) {
    return _line_num != it._line_num;
}

#endif //TASK5_CSV_PARSER_CSVPARSER_H

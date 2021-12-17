#ifndef TASK5_CSV_PARSER_CSVPARSER_H
#define TASK5_CSV_PARSER_CSVPARSER_H

#include "TuplePrinter.h"
#include "Exceptions.h"
#include <string>
#include <sstream>
#include <typeinfo>

const char LINE_DELIMITER = '\n';
const char CELL_DELIMITER = ',';

template<typename... Types>
class CSVParser {
private:
    std::tuple<Types...>* _lines;
    size_t _filled_size;
    size_t _max_size;

    void parseLine(std::string &str);
    void reallocate();

public:
    class Iterator {
    private:
        const std::tuple<Types...>* _lines_ptr;

    public:
        explicit Iterator(const std::tuple<Types...>* lines_ptr);

        const std::tuple<Types...>& operator*();
        Iterator& operator++();
        const Iterator operator++(int);
        bool operator==(const Iterator &it);
        bool operator!=(const Iterator &it);
    };

    explicit CSVParser(std::ifstream& in, size_t lines_to_skip = 0);
    ~CSVParser();

    template<typename... T>
    friend std::ostream& operator<<(std::ostream& out, const std::tuple<T...>& tuple);

    Iterator begin();
    Iterator end();

};


template<typename T_element>
struct converter {
    static bool check(const std::string &str) {
        T_element t;
        std::istringstream stream(str);
        stream >> t;
        stream >> std::ws;
        return stream.eof();
    }

    static T_element convert(const std::string &str) {
        T_element t;
        std::istringstream stream(str);
        stream >> t;
        stream >> std::ws;
        if (!stream.eof()) {
            throw InvalidLineFormat(0);
        }
        return t;
    }
};

template<typename T_tuple, typename T_element, size_t N>
struct tuple_adder {
    static void add(T_tuple& tup, T_element new_el, size_t index) {
        if (N >= sizeof(tup)) {
            throw IndexOutOfRange();
        }
        if (index == N && converter<typeof std::get<N>(tup)>::check(new_el)) {
            std::get<N>(tup) = converter<typeof std::get<N>(tup)>::convert(new_el);
            return;
        }
        else if (index == N && !converter<typeof std::get<N>(tup)>::check(new_el)) {
            throw UnknownType(0, index + 1);
        }
        tuple_adder<T_tuple, T_element, N - 1>::add(tup, new_el, index);
    }
};

template<typename T_tuple, typename T_element>
struct tuple_adder<T_tuple, T_element, 0> {
    static void add(T_tuple& tup, T_element new_el, size_t index) {
        if (index == 0 && converter<typeof std::get<0>(tup)>::check(new_el)) {
            std::get<0>(tup) = converter<typeof std::get<0>(tup)>::convert(new_el);
        }
        else if (index == 0 && !converter<typeof std::get<0>(tup)>::check(new_el)) {
            throw UnknownType(0, index + 1);
        }
    }
};

template<typename... Types>
void CSVParser<Types...>::reallocate() {
    size_t new_max_size = _max_size * 2;
    auto new_lines = new std::tuple<Types...>[new_max_size];
    for (int i = 0; i < _max_size; i++) {
        new_lines[i] = _lines[i];
    }
    delete _lines;
    _lines = new_lines;
    _max_size = new_max_size;
}

template<typename... Types>
void CSVParser<Types...>::parseLine(std::string &str) {
    std::tuple<Types...> tup;
    std::string tmp;
    std::istringstream stream(str);
    size_t index = 0;
    while (std::getline(stream, tmp, CELL_DELIMITER)) {
        if (index == sizeof...(Types)) {
            throw InvalidLineFormat(0);
        }
        tuple_adder<std::tuple<Types...>, std::string, sizeof...(Types) - 1>::add(tup, tmp, index);
        index += 1;
    }
    _lines[_filled_size] = tup;
    if (_filled_size == _max_size - 1) {
        reallocate();
    }
    _filled_size += 1;
}

template<typename... Types>
CSVParser<Types...>::CSVParser(std::ifstream &in, size_t lines_to_skip) {
    _max_size = 2;
    _lines = new std::tuple<Types...>[_max_size];
    _filled_size = 0;
    std::string line;
    for (size_t i = 0; i < lines_to_skip; i++) {
        std::getline(in, line, LINE_DELIMITER);
    }
    size_t line_num = lines_to_skip + 1;
    while (std::getline(in, line, LINE_DELIMITER)) {
        try {
            parseLine(line);
        }
        catch (InvalidLineFormat& ex) {
            throw InvalidLineFormat(line_num);
        }
        catch (UnknownType& ex) {
            throw UnknownType(line_num, ex.getCellNumber());
        }
        line_num += 1;
    }
}

template<typename... Types>
CSVParser<Types...>::~CSVParser() {
    delete _lines;
}

template<typename... Types>
CSVParser<Types...>::Iterator::Iterator(const std::tuple<Types...>* lines_ptr) : _lines_ptr(lines_ptr) {}

template<typename... Types>
typename CSVParser<Types...>::Iterator CSVParser<Types...>::begin() {
    return Iterator(_lines);
}

template<typename... Types>
typename CSVParser<Types...>::Iterator CSVParser<Types...>::end() {
    return Iterator(_lines + _filled_size);
}

template<typename... Types>
const std::tuple<Types...>& CSVParser<Types...>::Iterator::operator*() {
    return *_lines_ptr;
}

template<typename... Types>
typename CSVParser<Types...>::Iterator& CSVParser<Types...>::Iterator::operator++() {
    _lines_ptr++;
    return *this;
}

template<typename... Types>
const typename CSVParser<Types...>::Iterator CSVParser<Types...>::Iterator::operator++(int) {
    Iterator tmp(_lines_ptr);
    _lines_ptr++;
    return tmp;
}

template<typename... Types>
bool CSVParser<Types...>::Iterator::operator==(const Iterator &it) {
    return _lines_ptr == it._lines_ptr;
}

template<typename... Types>
bool CSVParser<Types...>::Iterator::operator!=(const Iterator &it) {
    return _lines_ptr != it._lines_ptr;
}

#endif //TASK5_CSV_PARSER_CSVPARSER_H

#ifndef TASK5_CSV_PARSER_CSVPARSER_H
#define TASK5_CSV_PARSER_CSVPARSER_H


#include <string>
#include <sstream>
#include <tuple>

const char LINE_DELIMITER = '\n';
const char CELL_DELIMITER = ',';

template<typename... Types>
class CSVParser {
private:
    std::tuple<Types...> *_lines;
    size_t _filled_size;
    size_t _max_size;

    void parseLine(std::string &str);
    void reallocate();


public:
    class Iterator {
    private:
        const std::tuple<Types...>* _elements_ptr;
    public:
        explicit Iterator(const std::tuple<Types...>* elements_ptr);

        const std::tuple<Types...>& operator*();
        Iterator operator++();
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
        if (!stream) {
            //exception
        }
        return t;
    }
};

template<typename T_tuple, size_t N>
struct tuple_printer {
    static void print(std::ostream& out, const T_tuple& value) {
        tuple_printer<T_tuple, N - 1>::print(out, value);
        out << ", " << std::get<N>(value);
    }
};

template<typename T_tuple>
struct tuple_printer<T_tuple, 0> {
    static void print(std::ostream& out, const T_tuple& value) {
        out << std::get<0>(value);
    }
};

template<typename T_tuple, typename T_element, size_t N>
struct tuple_adder {
    static void add(T_tuple& tup, T_element& new_el, size_t index) {
        tuple_adder<T_tuple, T_element, N - 1>::add(tup, new_el, index);
        if (index == N) {
            std::get<N>(tup) = new_el;
        }
    }
};

template<typename T_tuple, typename T_element>
struct tuple_adder<T_tuple, T_element, 0> {
    static void add(T_tuple& tup, T_element& new_el, size_t index) {
        if (index == 0) {
            std::get<0>(tup) = new_el;
        }
    }
};

template<typename... Types>
std::ostream& operator<<(std::ostream& out, const std::tuple<Types...>& tup) {
    out << "(";
    tuple_printer<std::tuple<Types...>, sizeof...(Types) - 1>::print(out, tup);
    out << ")";
    return out;
}

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
            //exception
        }
        if (converter<int>::check(tmp)) {
            auto element = converter<int>::convert(tmp);
            tuple_adder<std::tuple<Types...>, int, sizeof...(Types) - 1>::add(tup, element, index);
        }
        else if (converter<char>::check(tmp)) {
            auto element = converter<char>::convert(tmp);
            tuple_adder<std::tuple<Types...>, char, sizeof...(Types) - 1>::add(tup, element, index);
        }
        else if (converter<double>::check(tmp)) {
            auto element = converter<double>::convert(tmp);
            tuple_adder<std::tuple<Types...>, double, sizeof...(Types) - 1>::add(tup, element, index);
        }
        else if (converter<float>::check(tmp)) {
            auto element = converter<float>::convert(tmp);
            tuple_adder<std::tuple<Types...>, float, sizeof...(Types) - 1>::add(tup, element, index);
        }
        /*else if (converter<std::string>::check(tmp)) {
            auto element = tmp;
            tuple_adder<std::tuple<Types...>, std::string, sizeof...(Types) - 1>::add(tup, element, index);
            //add<0>(tup, element, index);
            //continue;
        }*/
        index += 1;
    }
    _lines[_filled_size] = tup;
    if (_filled_size == _max_size - 1) {
        //exception
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
    if (in.eof()) {
        //exception
    }
    while (std::getline(in, line, LINE_DELIMITER)) {
        parseLine(line);
    }
}

template<typename... Types>
CSVParser<Types...>::~CSVParser() {
    delete _lines;
}

template<typename... Types>
CSVParser<Types...>::Iterator::Iterator(const std::tuple<Types...>* elements_ptr) : _elements_ptr(elements_ptr) {
    //_elements_ptr = elements_ptr;
}

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
    return *_elements_ptr;
}

template<typename... Types>
typename CSVParser<Types...>::Iterator CSVParser<Types...>::Iterator::operator++() {
    _elements_ptr++;
    return *this;
}

template<typename... Types>
const typename CSVParser<Types...>::Iterator CSVParser<Types...>::Iterator::operator++(int) {
    Iterator tmp(_elements_ptr);
    _elements_ptr++;
    return tmp;
}

template<typename... Types>
bool CSVParser<Types...>::Iterator::operator==(const Iterator &it) {
    return _elements_ptr == it._elements_ptr;
}

template<typename... Types>
bool CSVParser<Types...>::Iterator::operator!=(const Iterator &it) {
    return _elements_ptr != it._elements_ptr;
}

#endif //TASK5_CSV_PARSER_CSVPARSER_H

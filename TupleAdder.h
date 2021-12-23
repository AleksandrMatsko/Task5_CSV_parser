#ifndef TASK5_CSV_PARSER_TUPLEADDER_H
#define TASK5_CSV_PARSER_TUPLEADDER_H

#include <tuple>
#include <string>
#include <sstream>
#include "Exceptions.h"

template<typename T_element>
struct Converter {
    static T_element convert(const std::string &str) {
        T_element t;
        std::istringstream stream(str);
        stream >> t;
        stream >> std::ws;
        if (!stream) {
            throw UnknownType(0, 0);
        }
        return t;
    }
};

template<>
struct Converter<std::string> {
    static std::string convert(const std::string &str) {
        return str;
    }
};

template<typename T_tuple, typename T_element, size_t N>
struct TupleAdder {
    static void add(T_tuple& tup, T_element new_el, size_t index) {
        if (N >= sizeof(tup)) {
            throw IndexOutOfRange();
        }
        if (index == N) {
            try {
                std::get<N>(tup) = Converter<typeof std::get<N>(tup)>::convert(new_el);
            }
            catch (UnknownType& ex) {
                throw UnknownType(0, index + 1);
            }
            return;
        }
        TupleAdder<T_tuple, T_element, N - 1>::add(tup, new_el, index);
    }
};

template<typename T_tuple, typename T_element>
struct TupleAdder<T_tuple, T_element, 0> {
    static void add(T_tuple& tup, T_element new_el, size_t index) {
        if (index == 0) {
            try {
                std::get<0>(tup) = Converter<typeof std::get<0>(tup)>::convert(new_el);
            }
            catch (UnknownType& ex) {
                throw UnknownType(0, index + 1);
            }
        }
    }
};



#endif //TASK5_CSV_PARSER_TUPLEADDER_H

#ifndef TASK5_CSV_PARSER_TUPLEPRINTER_H
#define TASK5_CSV_PARSER_TUPLEPRINTER_H

#include <ostream>
#include <tuple>

template<typename T_tuple, size_t N>
struct TuplePrinter {
    static void print(std::ostream& out, const T_tuple& value) {
        TuplePrinter<T_tuple, N - 1>::print(out, value);
        out << ", " << std::get<N>(value);
    }
};

template<typename T_tuple>
struct TuplePrinter<T_tuple, 0> {
    static void print(std::ostream& out, const T_tuple& value) {
        out << std::get<0>(value);
    }
};

template<typename... Types>
std::ostream& operator<<(std::ostream& out, const std::tuple<Types...>& tup) {
    out << "(";
    TuplePrinter<std::tuple<Types...>, sizeof...(Types) - 1>::print(out, tup);
    out << ")";
    return out;
}


#endif //TASK5_CSV_PARSER_TUPLEPRINTER_H

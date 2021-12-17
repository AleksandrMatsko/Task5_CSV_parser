#include <fstream>
#include <iostream>
#include "CSVParser.h"


int main() {
    std::string file_name = "test.csv";
    std::ifstream file(file_name);
    if (!file.is_open()) {
        throw FileNotOpen(file_name);
    }
    try {
        CSVParser<int, char, double, int, std::string> parser(file, 1);
        for (std::tuple<int, char, double, int, std::string> rs : parser) {
            std::cout << rs <<  std::endl;
        }
    }
    catch (InvalidLineFormat& ex) {
        std::cerr << ex.what() << std::endl;
    }
    catch (UnknownType& ex) {
        std::cerr << ex.what() << std::endl;
    }
    catch (IndexOutOfRange& ex) {
        std::cerr << ex.what() << std::endl;
    }
    catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }
    return 0;
}


#include <fstream>
#include <iostream>
#include "CSVParser.h"


int main() {
    std::ifstream file("test.csv");
    if (!file.is_open()) {
        //exception
        return 1;
    }
    CSVParser<int, char, double> parser(file, 0);
    for (std::tuple<int, char, double> rs : parser) {
        std::cout << rs <<  std::endl;
    }
    return 0;
}


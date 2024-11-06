// globals.hpp
#ifndef GLOBALS_HPP
#define GLOBALS_HPP
#include <variant>
#include <fstream>

extern std::ifstream inputFile;
extern std::ofstream outputFile;
using ColumnType = std::variant<int, double, std::string>;
std::ostream& operator<<(std::ostream& os, const ColumnType& column);
#endif // GLOBALS_HPP
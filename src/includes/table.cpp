// table.cpp
#include "table.hpp"
#include <iostream>
#include <fstream>
#include <variant>
#include <unordered_map>
#include "globals.hpp"
Table::Table() :name(""){}
Table::Table(const std::string& name) : name(name) {}
void Table::insertRow(const std::unordered_map<ColumnType, ColumnType>& row) {
    rows.push_back(row);
    std::cout << "Row inserted into " << name << "." << std::endl;
}

void Table::deleteRow(int id) {
    if (id >= 0 && id < rows.size()) {
        rows.erase(rows.begin() + id);
        std::cout << "Row " << id << " deleted from " << name << "." << std::endl;
    } else {
        std::cout << "Row " << id << " does not exist in " << name << "." << std::endl;
    }
}

void Table::queryTable() const {
    std::cout << "Table " << name << " contents:" << std::endl;
    for (const auto& row : rows) {
        for (const auto& [key, value] : row) {
            std::cout << key << ": " << value << " ";
        }
        std::cout << std::endl;
    }
}
void Table::save(std::ofstream& file) const {
    auto rowCount = rows.size();
    file.write((char*)&rowCount, sizeof(rowCount));
    for (const auto& row : rows) {
        auto rowSize = row.size();
        file.write((char*)&rowSize, sizeof(rowSize));
        for (const auto& [key, value] : row) {
            std::visit([&file](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, int>) {
                    char type = 'i';
                    file.write(&type, sizeof(type));
                    file.write((char*)&arg, sizeof(arg));
                } else if constexpr (std::is_same_v<T, double>) {
                    char type = 'd';
                    file.write(&type, sizeof(type));
                    file.write((char*)&arg, sizeof(arg));
                } else if constexpr (std::is_same_v<T, std::string>) {
                    char type = 's';
                    file.write(&type, sizeof(type));
                    auto length = arg.size();
                    file.write((char*)&length, sizeof(length));
                    file.write(arg.c_str(), length);
                }
            }, key);

            std::visit([&file](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, int>) {
                    char type = 'i';
                    file.write(&type, sizeof(type));
                    file.write((char*)&arg, sizeof(arg));
                } else if constexpr (std::is_same_v<T, double>) {
                    char type = 'd';
                    file.write(&type, sizeof(type));
                    file.write((char*)&arg, sizeof(arg));
                } else if constexpr (std::is_same_v<T, std::string>) {
                    char type = 's';
                    file.write(&type, sizeof(type));
                    auto length = arg.size();
                    file.write((char*)&length, sizeof(length));
                    file.write(arg.c_str(), length);
                }
            }, value);
        }
    }
}

void Table::load(std::ifstream& file) {
    size_t rowCount;
    file.read((char*)&rowCount, sizeof(rowCount));
    for (size_t i = 0; i < rowCount; ++i) {
        size_t rowSize;
        file.read((char*)&rowSize, sizeof(rowSize));
        std::unordered_map<ColumnType, ColumnType> row;
        for (size_t j = 0; j < rowSize; ++j) {
            ColumnType key, value;

            char type;
            file.read(&type, sizeof(type));
            if (type == 'i') {
                int temp;
                file.read((char*)&temp, sizeof(temp));
                key = temp;
            } else if (type == 'd') {
                double temp;
                file.read((char*)&temp, sizeof(temp));
                key = temp;
            } else if (type == 's') {
                size_t length;
                file.read((char*)&length, sizeof(length));
                std::string temp(length, '\0');
                file.read(&temp[0], length);
                key = temp;
            }

            file.read(&type, sizeof(type));
            if (type == 'i') {
                int temp;
                file.read((char*)&temp, sizeof(temp));
                value = temp;
            } else if (type == 'd') {
                double temp;
                file.read((char*)&temp, sizeof(temp));
                value = temp;
            } else if (type == 's') {
                size_t length;
                file.read((char*)&length, sizeof(length));
                std::string temp(length, '\0');
                file.read(&temp[0], length);
                value = temp;
            }

            row[key] = value;
        }
        rows.push_back(row);
    }
}

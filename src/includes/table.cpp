// table.cpp
#include "table.hpp"
#include <iostream>
#include <fstream>
#include <variant>
#include <unordered_map>
#include "globals.hpp"
Table::Table() : name(""), columnsNT() {}

Table::Table(const std::string& name) : name(name), columnsNT() {}

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
// not qualified
void Table::queryTable() const {
    std::cout << "Table " << name << " contents:" << std::endl;
    for (const auto& row : rows) {
        for (const auto& [key, value] : row) {
            std::visit([](auto&& arg) {
                std::cout << arg << " ";
            }, key);
            std::cout << ": ";
            std::visit([](auto&& arg) {
                std::cout << arg << " ";
            }, value);
        }
        std::cout << std::endl;
    }
}
void Table::save(std::ofstream& file) const {
    auto rowCount = rows.size();
    file.write((char*)&rowCount, sizeof(rowCount));
    
    auto columnCount = columnsNT.size();
    file.write((char*)&columnCount, sizeof(columnCount));
    for (const auto& column : columnsNT) {
        auto nameLength = column.first.size();
        file.write((char*)&nameLength, sizeof(nameLength));
        file.write(column.first.c_str(), nameLength);
        
        auto typeLength = column.second.size();
        file.write((char*)&typeLength, sizeof(typeLength));
        file.write(column.second.c_str(), typeLength);
    }

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
    
    size_t columnCount;
    file.read((char*)&columnCount, sizeof(columnCount));
    columnsNT.clear();
    for (size_t i = 0; i < columnCount; ++i) {
        size_t nameLength;
        file.read((char*)&nameLength, sizeof(nameLength));
        std::string name(nameLength, '\0');
        file.read(&name[0], nameLength);
        
        size_t typeLength;
        file.read((char*)&typeLength, sizeof(typeLength));
        std::string type(typeLength, '\0');
        file.read(&type[0], typeLength);
        
        columnsNT.push_back({name, type});
    }

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
void Table::addColumn(const std::string& name, const std::string& type) {
    columnsNT.push_back({name, type});
    std::cout << "Column " << name << " of type " << type << " added to " << this->name << "." << std::endl;
}
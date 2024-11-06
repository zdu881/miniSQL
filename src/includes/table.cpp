// table.cpp
#include "table.hpp"
#include <iostream>
#include <fstream>
Table::Table() :name(""){}
Table::Table(const std::string& name) : name(name) {}

void Table::insertRow(const std::unordered_map<std::string, std::string>& row) {
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
void Table::save(std::ofstream& file) const{
    auto rowCount = rows.size();
    file.write((char*)&rowCount, sizeof(rowCount));
    for (const auto& row : rows){
        auto rowSize = row.size();
        file.write((char*)&rowSize, sizeof(rowSize));
        for (const auto& [key,value] : row){
            auto keyLength = key.size();
            file.write((char*)&keyLength, sizeof(keyLength));
            file.write(key.c_str(), keyLength);
            auto valueLength = value.size();
            file.write((char*)&valueLength, sizeof(valueLength));
            file.write(value.c_str(), valueLength);
        }
    }
}
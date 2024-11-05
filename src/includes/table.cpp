// table.cpp
#include "table.hpp"
#include <iostream>

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
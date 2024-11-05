// database.cpp
#include "database.hpp"
#include <iostream>

void Database::createTable(const std::string& name) {
    if (tables.find(name) == tables.end()) {
        tables[name] = Table(name);
        std::cout << "Table " << name << " created." << std::endl;
    } else {
        std::cout << "Table " << name << " already exists." << std::endl;
    }
}

void Database::dropTable(const std::string& name) {
    if (tables.erase(name)) {
        std::cout << "Table " << name << " dropped." << std::endl;
    } else {
        std::cout << "Table " << name << " does not exist." << std::endl;
    }
}

Table* Database::getTable(const std::string& name) {
    auto it = tables.find(name);
    if (it != tables.end()) {
        return &it->second;
    }
    return nullptr;
}
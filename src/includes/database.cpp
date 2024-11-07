// database.cpp
#include "database.hpp"
#include <iostream>
#include <fstream>
void Database::createTable(const std::string& name, Table& table) {
    if (tables.find(name) == tables.end()) {
        tables[name] = std::move(table);
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

void Database::save(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return;
    }
    size_t tableCount = tables.size();
    file.write((char*)&tableCount, sizeof(tableCount));
    for (const auto& [name, table] : tables) {
        size_t nameLength = name.size();
        file.write((char*)&nameLength, sizeof(nameLength));
        file.write(name.c_str(), nameLength);
        table.save(file);
    }
    file.close();
}

void Database::load(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return;
    }
    size_t tableCount;
    file.read((char*)&tableCount, sizeof(tableCount));
    tables.clear();
    for (size_t i = 0; i < tableCount; ++i) {
        size_t nameLength;
        file.read((char*)&nameLength, sizeof(nameLength));
        std::string name(nameLength, '\0');
        file.read(&name[0], nameLength);
        Table table(name);
        table.load(file);
        tables[name] = table;
    }
}
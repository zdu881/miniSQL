// database.cpp
#include "database.hpp"
#include <iostream>
#include <fstream>
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

void Database::save(const std::string& filename){
    std::ofstream file(filename, std::ios::out);
    if (!file){
        std::cerr<<"Cannot open the file for saving:"<<filename<<std::endl;
        return;
    }

    auto tableCount= tables.size();
    file.write((char*)&tableCount, sizeof(tableCount));
    for (const auto& [name,table] : tables){
        auto nameLength = name.size(); 
        file.write((char*)&nameLength, sizeof(nameLength));
        file.write(name.c_str(), nameLength);
        table.save(file);
    }

    file.close();
}
void Database::load(const std::string& filename){
    std::ifstream input(filename, std::ios::in);
    if (!input){
        std::cerr<<"Cannot open the file for loading:"<<filename<<std::endl;
        return;
    }

    size_t tableCount;
    input.read((char*)&tableCount, sizeof(tableCount));

    tables.clear(); // 清空当前的表

    for (size_t i = 0; i < tableCount; ++i){
        size_t nameLength;
        input.read((char*)&nameLength, sizeof(nameLength));

        std::string name(nameLength, ' ');
        input.read(&name[0], nameLength);

        Table table;
        table.load(input);
    }
    input.close();
}
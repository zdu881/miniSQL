// table.cpp
#include "table.hpp"
#include <iostream>
#include <fstream>
#include <variant>
#include <algorithm>
#include <unordered_map>
#include <string>
#include "globals.hpp"
Table::Table() : name(""), columnsNT() {}

Table::Table(const std::string& name) : name(name), columnsNT() {}

void Table::insertRow(const std::vector<ColumnType>& values) {
    if (values.size() != columnsNT.size()) {
        std::cerr << "Column count mismatch. Expected " << columnsNT.size() << " values." << std::endl;
        return;
    }
    for (size_t i = 0; i < columnsNT.size(); ++i) {
        columns[columnsNT[i].first].push_back(values[i]);
    }
    std::cout << "Row inserted into " << name << "." << std::endl;
}


void Table::queryTable() const {
    std::cout << "Table " << name << " contents:" << std::endl;
    for (size_t i = 0; i < columns.begin()->second.size(); ++i) {
        for (const auto& column : columnsNT) {
            const auto& colData = columns.at(column.first);
            std::visit([](auto&& arg) {
                if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, std::string>) {
                    std::cout << "\"" << arg << "\" ";
                } else {
                    std::cout << arg << " ";
                }
            }, colData[i]);
        }
        std::cout << std::endl;
    }
}

void Table::queryTable(const std::vector<std::string>& columns) const {
    std::cout << "Table " << name << " contents:" << std::endl;
    size_t rowCount = this->columns.begin()->second.size();
    for (size_t i = 0; i < rowCount; ++i) {
        for (const auto& column : columns) {
            const auto& colData = this->columns.at(column);
            std::visit([](auto&& arg) {
                if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, std::string>) {
                    std::cout << "\"" << arg << "\" ";
                } else {
                    std::cout << arg << " ";
                }
            }, colData[i]);
        }
        std::cout << std::endl;
    }
}

void Table::queryTable(const std::vector<std::string>& columns, const std::vector<Condition>& conditions) const {
    std::cout << "Table " << name << " contents with conditions:" << std::endl;
 
}
void Table::deleteRow(const std::vector<Condition>& conditions) {
    std::vector<size_t> rowsToDelete;
    for (size_t i = 0; i < columns.begin()->second.size(); ++i) {
        bool deleteRow = true;
        for (const auto& condition : conditions) {
            const auto& colData = columns.at(condition.column);
            const auto& colType = std::find_if(columnsNT.begin(), columnsNT.end(), 
                                               [&condition](const auto& pair) { return pair.first == condition.column; })->second;
        //wait to write
        }
        if (deleteRow) {
            rowsToDelete.push_back(i);
        }
    }
    for (auto& [columnName, columnData] : columns) {
        for (size_t i = 0; i < rowsToDelete.size(); ++i) {
            columnData.erase(columnData.begin() + rowsToDelete[i]);
        }
    }
    std::cout << rowsToDelete.size() << " rows deleted from " << name << "." << std::endl;
}
void Table::updateRow(const std::vector<std::pair<std::string, ColumnType>>& setConfigs, const std::vector<Condition>& conditions){
    std::vector<size_t> rowsToUpdate;
    for (size_t i = 0; i < columns.begin()->second.size(); ++i) {
        bool updateRow = true;
        for (const auto& condition : conditions) {
            const auto& colData = columns.at(condition.column);
            const auto& colType = std::find_if(columnsNT.begin(), columnsNT.end(), 
                                               [&condition](const auto& pair) { return pair.first == condition.column; })->second;
            //wait to write
        }
        if (updateRow) {
            rowsToUpdate.push_back(i);
        }
    }
    for ( auto& [columnName, columnData] : columns) {
        for (size_t i = 0; i < rowsToUpdate.size(); ++i) {
            columnData[rowsToUpdate[i]] = std::find_if(setConfigs.begin(), setConfigs.end(), 
                                                       [&columnName](const auto& pair) { return pair.first == columnName; })->second;
        }
    }
    std::cout << rowsToUpdate.size() << " rows updated in " << name << "." << std::endl;
}
void Table::save(std::ofstream& file) const {

}

void Table::load(std::ifstream& file) {
    
}

void Table::addColumn(const std::string& name, const Data_type& type) {
    columnsNT.push_back({name, type});
    std::cout << "Column " << name << " of type " << type << " added to " << this->name << "." << std::endl;
}

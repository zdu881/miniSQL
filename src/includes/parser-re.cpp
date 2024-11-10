#include "parser-re.hpp"
#include "query.hpp"
#include <string>
#include <unordered_map>
#include <globals.hpp>
#include <vector>
#include <fstream>
#include <iostream>
// "(",")"has not benn removed
// input:command_type = CREATE_TABLE, paratokens = {"table_name", "(", "column_name1", "column_type1", ",", "column_name2", "column_type2", ",", ... , ")"}

void Parser::parse(Command_line command_line, std::unordered_map<std::string, Database>* databases, std::string* currentDatabase){
    std::vector<std::string>paratokens = command_line.get_paratokens();
    Command_type command_type = command_line.get_command_type();
    if(command_line.get_command_type() == CREATE_DATABASE){
        std::string dbName = paratokens[0];
        (*databases)[dbName] = Database();
        // 创建 dbName.db 文件
        std::ofstream dbFile(dbName + ".db");
        if (dbFile) {
            dbFile.close();
            std::cout << "Database " << dbName << " created." << std::endl;
        } else {
            std::cerr << "Failed to create database file: " << dbName << ".db" << std::endl;
        }
    } else if (command_type == USE_DATABASE) {
        std::string dbName = paratokens[0];
        if (databases->find(dbName) != databases->end()) {
            *currentDatabase = dbName;
            (*databases)[dbName].load(dbName + ".db");
        }
    } else if (command_type == CREATE_TABLE) {
        std::string tableName = paratokens[0];
        Table table(tableName);
        for (size_t i = 2; i < paratokens.size() - 1; i += 2) {
            table.addColumn(paratokens[i], fromrawStringtoData_type(paratokens[i + 1]));
        }
        databases->at(*currentDatabase).createTable(tableName, table);
    } else if (command_type==DROP_TABLE){
        std::string tableName = paratokens[0];
        databases->at(*currentDatabase).dropTable(tableName);
    } else if (command_type == INSERT_INTO) {
        std::string tableName = paratokens[0];
        if (paratokens[1] == "VALUES" && paratokens[2] == "(" && paratokens.back() == ")") {
            Table* table = databases->at(*currentDatabase).getTable(tableName);
                if (table) {
                    std::vector<ColumnType> values;
                    const auto& columns = table->columnsNT;
                    size_t colIndex = 0;
                    for (size_t i = 3; i < paratokens.size() - 1; ++i) {
                        if (paratokens[i] == ",") continue; // Ignore commas
                        ColumnType value;
                        const auto& columnType = columns[colIndex].second;
                        if (columnType == TEXT) {
                            std::string strValue;
                            while (i < paratokens.size() - 1 && paratokens[i] != "\"") {
                                strValue += paratokens[i] + " ";
                                ++i;
                            }
                            if (i < paratokens.size() - 1 && paratokens[i] == "\"") {
                                strValue.pop_back(); // Remove trailing space
                            }
                            value = strValue;
                        } else if (columnType == INTEGER) {
                            try {
                                value = std::stoi(paratokens[i]);
                            } catch (const std::invalid_argument& e) {
                                std::cerr << "Invalid integer value: " << paratokens[i] << std::endl;
                                return;
                            } catch (const std::out_of_range& e) {
                                std::cerr << "Integer value out of range: " << paratokens[i] << std::endl;
                                return;
                            }
                        } else if (columnType == FLOAT) {
                            try {
                                value = std::stod(paratokens[i]);
                            } catch (const std::invalid_argument& e) {
                                std::cerr << "Invalid float value: " << paratokens[i] << std::endl;
                                return;
                            } catch (const std::out_of_range& e) {
                                std::cerr << "Float value out of range: " << paratokens[i] << std::endl;
                                return;
                            }
                        }
                        values.push_back(value);
                        ++colIndex;
                    }
                    table->insertRow(values);
                } else {
                    std::cerr << "Table " << tableName << " does not exist." << std::endl;
                }
            } else {
                std::cerr << "Invalid INSERT INTO syntax." << std::endl;
            } 
    } else if (command_type == SELECT_FROM) {
        // (SELECT) ID, NAME, AGE FROM table_name WHERE ID = 1 AND NAME = "John"
        // paratokens = {"ID", "NAME", "AGE", "FROM", "table_name", "WHERE", "ID", "=", "1", "AND", "NAME", "=", "John"}
        std::vector<std::string> columns;
        std::string tableName;
        std::vector<Condition> conditions;
        BOOL_OP pre_bool_op = AND;
        size_t i = 0;
        for (; i < paratokens.size(); ++i) {
            if (paratokens[i] == "FROM") {
                tableName = paratokens[++i];
                break;
            }else if(paratokens[i] == "*") {
                for (auto& column : databases->at(*currentDatabase).getTable(tableName)->columnsNT) {
                    columns.push_back(column.first);
                }
                
            }
            else {
                columns.push_back(paratokens[i]);
            }
        }
        ++i;//skip "WHERE"
        for (; i +3< paratokens.size(); i += 4) {
                std::string column = paratokens[i] ;
                std::string op = paratokens[i + 1];
                std::string value = paratokens[i + 2];
                conditions.push_back(Condition(pre_bool_op, column, value, op));
                if (paratokens[i + 3] == "AND") {
                    pre_bool_op = AND;
                } else if (paratokens[i + 3] == "OR") {
                    pre_bool_op = OR;
                }
        }
        if (columns.size() == 0) {
            std::cerr << "No columns specified." << std::endl;
            return;
        }
        Table* table = databases->at(*currentDatabase).getTable(tableName);
        if (table) {
            if (conditions.size() == 0) {
                table->queryTable(columns);
            } else {
                table->queryTable(columns, conditions);
            }
        } else {
            std::cerr << "Table " << tableName << " does not exist." << std::endl;
        }

    } else if (command_type == DELETE_FROM_WHERE){
        //input::DELETE FROM table_name WHERE ID = 1 AND NAME = "John"
        //paratokens = {"table_name", "WHERE", "ID", "=", "1", "AND", "NAME", "=", "John"}
        std::string tableName = paratokens[0];
        std::vector<Condition> conditions;
        BOOL_OP pre_bool_op = AND;
        for (size_t i = 2; i < paratokens.size(); i += 4) {
            std::string column = paratokens[i];
            std::string op = paratokens[i + 1];
            std::string value = paratokens[i + 2];
            conditions.push_back(Condition(pre_bool_op, column, value, op));
            if (i + 3 < paratokens.size() && paratokens[i + 3] == "AND") {
                pre_bool_op = AND;
            } else if (i + 3 < paratokens.size() && paratokens[i + 3] == "OR") {
                pre_bool_op = OR;
            }
        }
        Table* table = databases->at(*currentDatabase).getTable(tableName);
        if (table) {
            table->deleteRow(conditions);
        } else {
            std::cerr << "Table " << tableName << " does not exist." << std::endl;
        }


    }else if(command_type == UPDATE_SET_WHERE){
        //input::UPDATE table_name SET column_name1 = value1, column_name2 = value2 WHERE ID = 1 AND NAME = "John"
        //paratokens = {"table_name", "SET", "column_name1", "=", "value1", ",", "column_name2", "=", "value2", "WHERE", "ID", "=", "1", "AND", "NAME", "=", "John"}
        //doing
        std::string tableName = paratokens[0];
        std::vector<Condition> conditions;
        BOOL_OP pre_bool_op = AND;
        std::vector<std::pair<std::string, ColumnType>> setConfigs;
        size_t i = 2;
        for (; i < paratokens.size(); i += 3) {
            std::string column = paratokens[i];
            ColumnType value;
            if (paratokens[i + 1] == "=") {
                if (paratokens[i + 2] == "NULL") {
                    value = "NULL";
                } else {
                    try {
                        value = std::stoi(paratokens[i + 2]);
                    } catch (const std::invalid_argument& e) {
                        try {
                            value = std::stod(paratokens[i + 2]);
                        } catch (const std::invalid_argument& e) {
                            value = paratokens[i + 2];
                        }
                    }
                }
            }
            setConfigs.push_back({column, value});
        }
        i += 1;//skip "WHERE"
        for (; i + 3 < paratokens.size(); i += 4) {
            std::string column = paratokens[i];
            std::string op = paratokens[i + 1];
            std::string value = paratokens[i + 2];
            conditions.push_back(Condition(pre_bool_op, column, value, op));
            if (paratokens[i + 3] == "AND") {
                pre_bool_op = AND;
            } else if (paratokens[i + 3] == "OR") {
                pre_bool_op = OR;
            }
        }
        Table* table = databases->at(*currentDatabase).getTable(tableName);
        if (table) {
            table->updateRow(setConfigs, conditions);
        } else {
            std::cerr << "Table " << tableName << " does not exist." << std::endl;
        }
    } else if (1 ){

    }
}

ColumnType fromrawStringtoInt(ColumnType str){
    return std::stoi(std::get<std::string>(str));
}

ColumnType fromrawStringtoDouble(ColumnType str){
    return std::stod(std::get<std::string>(str));
}

Data_type fromrawStringtoData_type(const std::string &type){
    if(type == "TEXT") return TEXT;
    else if(type == "INTEGER") return INTEGER;
    else if(type == "FLOAT") return FLOAT;
    else return ERROR_TYPE;
}
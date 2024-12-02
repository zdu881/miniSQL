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
void Parser::parse(Command_line command_line, std::unordered_map<std::string, Database>* databases, std::string* currentDatabase){
    std::vector<std::string>paratokens = command_line.get_paratokens();
    Command_type command_type = command_line.get_command_type();
    if(command_line.get_command_type() == CREATE_DATABASE){
        std::string dbName = paratokens[0];
        if (databases->find(dbName) != databases->end()) {
            // std::cerr << "Command " << linenumber<<": " << "Database " << dbName << " already exists." << std::endl;
            return;
        }
        (*databases)[dbName] = Database();
        // 创建 dbName.db 文件
        (*databases)[dbName].save(dbName + ".db");
    } else if (command_type == USE_DATABASE) {
        std::string dbName = paratokens[0];
        if (databases->find(dbName) != databases->end()) {
            *currentDatabase = dbName;
            (*databases)[dbName].load(dbName + ".db");
        }
    } else if (command_type == CREATE_TABLE) {
        std::string tableName = paratokens[0];
        Table table(tableName);
        for (size_t i = 1; i < paratokens.size() - 1; i += 2) {
            table.addColumn(paratokens[i], fromrawStringtoData_type(paratokens[i + 1]));
        }
        databases->at(*currentDatabase).createTable(tableName, table);
    } else if (command_type==DROP_TABLE){
        std::string tableName = paratokens[0];
        databases->at(*currentDatabase).dropTable(tableName);
    } else if (command_type == INSERT_INTO) {
        std::string tableName = paratokens[0];
        // for(auto i:paratokens) 
        // std::cout << i << " ";
        // std::cout << std::endl;
        //if (paratokens[1] == "VALUES" && paratokens[2] == "(" && paratokens.back() == ")") {
        if (1){
            Table* table = databases->at(*currentDatabase).getTable(tableName);
                if (table) {
                    std::vector<ColumnType> values;
                    const auto& columns = table->columnsNT;
                    //output columnsNT
                    // for(auto i:columns) std::cout<<i.first<<" "<<i.second<<std::endl;
                    size_t colIndex = 0;
                    for (size_t i = 1; i < paratokens.size() ; ++i) {
                        if (paratokens[i] == ",") continue; // Ignore commas
                        ColumnType value;
                        const auto& columnType = columns[colIndex].second;
                        // std::cout << columnType << " " << paratokens[i] << std::endl;
                        if (columnType == TEXT) {
                            //std::cout<<paratokens[i]<<std::endl;
                            value = paratokens[i];

                        } else if (columnType == INTEGER) {
                            try {
                                value = std::stoi(paratokens[i]);
                            } catch (const std::invalid_argument& e) {
                                std::cerr << "Command " << linenumber<<": " << "Invalid integer value: " << paratokens[i] << std::endl;
                                return;
                            } catch (const std::out_of_range& e) {
                                std::cerr << "Command " << linenumber<<": " << "Integer value out of range: " << paratokens[i] << std::endl;
                                return;
                            }
                        } else if (columnType == FLOAT) {
                            try {
                                value = std::stod(paratokens[i]);
                            } catch (const std::invalid_argument& e) {
                                std::cerr << "Command " << linenumber<<": " << "Invalid float value: " << paratokens[i] << std::endl;
                                return;
                            } catch (const std::out_of_range& e) {
                                std::cerr << "Command " << linenumber<<": " << "Float value out of range: " << paratokens[i] << std::endl;
                                return;
                            }
                        }
                        values.push_back(value);
                        ++colIndex;
                    }
                    //output values
                    // for(auto i:values) std::cout<<i<<" ";
                    table->insertRow(values);
                    //table 
                } else {
                    std::cerr << "Command " << linenumber<<": " << "Table " << tableName << " does not exist." << std::endl;
                }
            } else {
                std::cerr << "Command " << linenumber<<": " << "Invalid INSERT INTO syntax." << std::endl;
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
                tableName = paratokens[i+2];
                for (auto& column : databases->at(*currentDatabase).getTable(tableName)->columnsNT) {
                    columns.push_back(column.first);
                }
                i+=2;
                break;
                //output columns
                
            }
            else {
                if(paratokens[i]!=",")columns.push_back(paratokens[i]);
            }
        }
        // std::cout << "BEFORE WHERE" << std::endl;
        i+=2;//skip "WHERE"
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
        // std::cout << "JUST AFTER WHERE" << std::endl;
        if (columns.size() == 0) {
            std::cerr << "Command " << linenumber<<": " << "No columns specified." << std::endl;
            return;
        }
 
        //Table *table = nullptr;
        Table* table = &(databases->at(*currentDatabase).tables[tableName]);
            //std::cout<<"JUST AFTER GET TABLE"<<std::endl;
        if (table) {
            if (conditions.size() == 0) {

                table->queryTable(columns);
            } else {
                table->queryTable(columns, conditions);
            }
        } else {
            std::cerr << "Command " << linenumber<<": " << "Table " << tableName << " does not exist." << std::endl;
        }
    } else if (command_type == SELECT_FROM_INNER_JOIN_ON) {
        //SELECT column_name1, column_name2 FROM table_name1 INNER JOIN table_name2 ON table_name1.column_name1 = table_name2.column_name2
        //paratokens = {"column_name1", "column_name2", "FROM", "table_name1", "INNER", "JOIN", "table_name2", "ON", "table_name1.column_name1", "=", "table_name2.column_name2"}
        std::vector<std::string> columns;
        std::string tableName1;
        std::string tableName2;
        std::string column1;
        std::string column2;
        size_t i = 0;
        for (; i < paratokens.size(); ++i) {
            if (paratokens[i] == "FROM") {
                tableName1 = paratokens[++i];
                break;
            } else {
                if(paratokens[i]!=",")columns.push_back(paratokens[i]);
            }
        }
        //對columns取.之後的字符串
        //for(auto& i:columns) i = i.substr(i.find('.') + 1);
        i += 3; // Skip "INNER JOIN"
        tableName2 = paratokens[i];
        i += 2; // Skip "ON"
        column1 = paratokens[i].substr(paratokens[i].find('.') + 1);
        i += 2; // Skip "="
        column2 = paratokens[i].substr(paratokens[i].find('.') + 1);
        i+=2;
        if(i+3<paratokens.size()){
            std::vector<Condition> conditions;
            BOOL_OP pre_bool_op = AND;
            for (; i + 3 < paratokens.size(); i += 4) {
                std::string column = paratokens[i];
                //取. 之後的字符串
                column = column.substr(column.find('.') + 1);
                std::string op = paratokens[i + 1];
                std::string value = paratokens[i + 2];
                conditions.push_back(Condition(pre_bool_op, column, value, op));
                if (paratokens[i + 3] == "AND") {
                    pre_bool_op = AND;
                } else if (paratokens[i + 3] == "OR") {
                    pre_bool_op = OR;
                }
            }
            databases->at(*currentDatabase).innerJoinQueryWHERE(columns, tableName1, tableName2, column1, column2, conditions);
        } else databases->at(*currentDatabase).innerJoinQuery(columns, tableName1, tableName2, column1, column2);
    } else if (command_type == DELETE_FROM_WHERE){
        //input::DELETE FROM table_name WHERE ID = 1 AND NAME = "John"
        //paratokens = {"table_name", "WHERE", "ID", "=", "1", "AND", "NAME", "=", "John"}
        std::string tableName = paratokens[0];
        std::vector<Condition> conditions;
        BOOL_OP pre_bool_op = AND;
        Table* table = databases->at(*currentDatabase).getTable(tableName);

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
        if (table) {
            table->deleteRow(conditions);
        } else {
            std::cerr << "Command " << linenumber<<": " << "Table " << tableName << " does not exist." << std::endl;
        }


    }else if(command_type == UPDATE_SET_WHERE){
        //input::UPDATE table_name SET column_name1 = value1, column_name2 = value2 WHERE ID = 1 AND NAME = "John"
        //paratokens = {"table_name", "column_name1", "=", "value1", ",", "column_name2", "=", "value2", "WHERE", "ID", "=", "1", "AND", "NAME", "=", "John"}
        //doing
        std::string tableName = paratokens[0];
        std::vector<Condition> conditions;
        BOOL_OP pre_bool_op = AND;
        std::vector<UpdateConfig> setConfigs;
        size_t i = 1;
        for (; i < paratokens.size()&& paratokens[i-1] != "WHERE"; i += 4) {
            std::string column = paratokens[i];
            ColumnType value;
            if (paratokens[i + 1] == "="&&paratokens[i + 3] == "+") {
                value = paratokens[i + 4];
                setConfigs.push_back(UpdateConfig{column, ADD, value});
                i+=2;
            } else if (paratokens[i + 1] == "="&&paratokens[i + 3] == "-") {
                value = paratokens[i + 4];
                setConfigs.push_back(UpdateConfig{column, SUBTRACT, value});
                i+=2;
            } else if (paratokens[i + 1] == "="&&paratokens[i + 3] == "*") {
                value = paratokens[i + 4];
                setConfigs.push_back(UpdateConfig{column, MULTIPLY, value});
                i+=2;
            } else if (paratokens[i + 1] == "="&&paratokens[i + 3] == "/") {
                value = paratokens[i + 4];
                setConfigs.push_back(UpdateConfig{column, DIVIDE, value});
                i+=2;
            } else if (paratokens[i + 1] == "=") {
                value = paratokens[i + 2];
                setConfigs.push_back(UpdateConfig{column, SET, value});
            } else {
                std::cerr << "Command " << linenumber<<": " << "Invalid update operation." << std::endl;
                return;
            }
            // std::cout << "SET" << column << value << std::endl;
        }
        
        
        //skip "WHERE"
        // std::cout << "WHERE" << paratokens[i] << std::endl;
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
        // std::cout << "BEFORE GET TABLE" << std::endl;
        Table* table = databases->at(*currentDatabase).getTable(tableName);
        if (table) {
            table->updateRow(setConfigs, conditions);
        } else {
            std::cerr << "Command " << linenumber<<": " << "Table " << tableName << " does not exist." << std::endl;
        }
    } else if (1 ){

    }
}

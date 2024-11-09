// query.cpp
#include "parser-re.hpp"
#include "query.hpp"
#include "globals.hpp"
#include <iostream>
#include <fstream>
#include <sstream>


Query::Query() {}

Query::Query(Database* db) : dbptr(db) {}

Query::Query(std::unordered_map<std::string, Database>* dbs, std::string* currentDb)
    : databases(dbs), currentDatabase(currentDb) {}

Query::~Query() {}

void Query::getQ() {
    std::cout << "MiniSQL> ";
    command_line.get_command_line();
}

void Query::excQ() {
    Parser parser;
    parser.parse(command_line, databases, currentDatabase);
    //传入的后两个参数是指针
}
/*
void Query::loaddb(Database& db){
    this->db = db;
}
*/
bool is_special(const std::string & a){
    if(a.length()>=2) return 0;
    if(a.length()==0) return 1;
    return !(a[0]=='_'||('a'<=a[0]&&a[0]<'z')||('A'<=a[0]&&a[0]<'Z')||('1'<=a[0]&&a[0]<'9'));
}
bool is_special(const char & a){
    return a==' '||a=='\n'||a==')'||a=='('||a==','||a=='\''||a==';'||a=='<'||a=='='||a=='>'||a=='*'||a=='/'||a=='+'||a=='-';
}
bool _is_empty(const char & a){
    return a==' '||a=='\n';
}
void Command_line::get_command_line() {
    std::stringstream input;
    std::string ans, para_string;
    char lin = 0;
    bool in_ = false;
    command_type = ERROR_COMMAND;
    paratokens.clear();

    BOOL_OP pre_sign = AND;

    auto get_bool_type = [](const std::string &para_string) {
        if (para_string == "OR") return OR;
        else if (para_string == "AND") return AND;
        return ERROR_BOOL_OP;
    };

    while (true) {
        lin = std::cin.get();
        if (in_ || lin == '\'') {
            input << lin;
        } else {
            if (is_special(lin)) {
                input << ' ' << lin << ' ';
            } else {
                input << lin;
            }
        }
        if (lin == ';') break;
    }

    ans = input.str();
    std::cout << ans << std::endl;

    std::istringstream iss(ans);
    std::string command;
    iss >> command;

    if (command == "CREATE") {
        std::string sub_command;
        iss >> sub_command;
        if (sub_command == "DATABASE") {
            command_type = CREATE_DATABASE;
            iss >> para_string;
            paratokens.push_back(para_string);
        } else if (sub_command == "TABLE") {
            command_type = CREATE_TABLE;
            iss >> para_string;
            paratokens.push_back(para_string);
            iss >> para_string; // (
            while (iss >> para_string && para_string != ")") {
                paratokens.push_back(para_string);
            }
        }
    } else if (command == "USE") {
        std::string sub_command;
        iss >> sub_command;
        if (sub_command == "DATABASE") {
            command_type = USE_DATABASE;
            iss >> para_string;
            paratokens.push_back(para_string);
        }
    } else if (command == "DROP") {
        std::string sub_command;
        iss >> sub_command;
        if (sub_command == "TABLE") {
            command_type = DROP_TABLE;
            iss >> para_string;
            paratokens.push_back(para_string);
        }
    } else if (command == "INSERT") {
        std::string sub_command;
        iss >> sub_command;
        if (sub_command == "INTO") {
            command_type = INSERT_INTO;
            iss >> para_string;
            paratokens.push_back(para_string);
            iss >> para_string; // VALUES
            iss >> para_string; // (
            while (iss >> para_string && para_string != ")") {
                paratokens.push_back(para_string);
            }
        }
    } else if (command == "SELECT") {
        command_type = SELECT_FROM;
        while (iss >> para_string ) {
            paratokens.push_back(para_string);
        }
    } else if (command == "UPDATE") {
        command_type = UPDATE_SET_WHERE;
        iss >> para_string;// table name
        paratokens.push_back(para_string);
        iss >> para_string; // SET
        while (iss >> para_string) {
            paratokens.push_back(para_string);
        }

    } else if (command == "DELETE") {
        std::string sub_command;
        iss >> sub_command;
        if (sub_command == "FROM") {
            command_type = DELETE_FROM_WHERE;
            while (iss >> para_string) {
                paratokens.push_back(para_string);
            }
        }
    } 

    //default : ERROR_COMMAND
}
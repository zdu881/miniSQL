// query.hpp
#ifndef QUERY_HPP
#define QUERY_HPP

#include <string>
#include <unordered_map>
#include "database.hpp"

class Query {
public:
    Query();
    Query(Database* db);
    Query(std::unordered_map<std::string, Database>* dbs, std::string* currentDb);
    ~Query();
    void getQ();
    void excQ();
    //void loaddb(Database& db);
private:
    Database* dbptr = nullptr;
    std::unordered_map<std::string, Database>* databases;
    std::string* currentDatabase;
    Command_line command_line;
};
class Command_line {
public:
    Command_line();
    ~Command_line();
    void get_command_line();
private:
    Command_type command_type;
    std::vector <std::string> paratokens;
};
#endif // QUERY_HPP
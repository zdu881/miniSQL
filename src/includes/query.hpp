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
    std::string userInput;
    Database* dbptr = nullptr;
    std::unordered_map<std::string, Database>* databases;
    std::string* currentDatabase;
};

#endif // QUERY_HPP
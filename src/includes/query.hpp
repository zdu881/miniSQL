// query.hpp
#ifndef QUERY_HPP
#define QUERY_HPP

#include <string>
#include "database.hpp"

class Query {
public:
    Query();
    ~Query();
    void getQ();
    void excQ();
    //void loaddb(Database& db);
private:
    std::string userInput;
    Database* dbptr =nullptr;
};

#endif // QUERY_HPP
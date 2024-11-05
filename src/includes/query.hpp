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
private:
    std::string userInput;
    Database db;
};

#endif // QUERY_HPP
#include <iostream>
#include <fstream>
#include <unordered_map>
#include "query.hpp"
#include "globals.hpp"
#include "database.hpp"

int main(int argv,char* argc[]){
    //input: MySQL input.sql output.csv
    if(argv!=3){
        std::cout<<"Please input the correct command"<<std::endl;
        return 0;
    }
    std::string inputFilename = argc[1];
    std::string outputFilename = argc[2];
    inputFile.open(inputFilename);
    outputFile.open(outputFilename);
    if (!inputFile|| !outputFile){
        std::cerr<<"Cannot open the input file:"<<inputFilename<<std::endl;
        return 0;
    }
    outputFile << "MiniSQL output:" << std::endl;
    std::unordered_map<std::string, Database> databases;
    std::string currentDatabase;
    std::cout<<">_< Welcome to MiniSQL"<<std::endl;
    Query query(&databases, &currentDatabase);

    // 读取 databaselist.txt 并加载数据库
    std::ifstream dbListFile("../database/databaselist.txt");
    if (dbListFile) {
        std::string dbName;
        while (std::getline(dbListFile, dbName)) {
            if (!dbName.empty()) {
                databases[dbName] = Database();
                databases[dbName].load(dbName + ".db");
                std::cout << "Database " << dbName << " loaded from databaselist.txt." << std::endl;
            }
        }
        dbListFile.close();
    }

    int cnt = 0;
    while(!inputFile.eof()){
        query.getQ();
        query.excQ();
        cnt++;
    }
    for (auto& [name, db] : databases) {
        db.save(name + ".db");
    }
    std::cout << cnt << " commands executed." << std::endl;
    inputFile.close();
    outputFile.close();

    // 程序结束前，写回 databaselist.txt
    std::ofstream dbListOut("databaselist.txt", std::ios::trunc);
    if (dbListOut) {
        for (const auto& [name, db] : databases) {
            dbListOut << name << std::endl;
        }
        dbListOut.close();
    } else {
        std::cerr << "Cannot open databaselist.txt for writing." << std::endl;
    }
    
    return 0;
}

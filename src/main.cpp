#include <iostream>
#include <fstream>
#include "query.hpp"
#include "globals.hpp"
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
    std::cout<<">_< Welcome to MiniSQL"<<std::endl;
    Query query;
    while(1){
        query.getQ();
        query.excQ();
    }

    return 0;
}

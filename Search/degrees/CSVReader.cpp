
//#include <fstream>
//#include <string>
//#include <vector>
#include <stdexcept>

#include "CSVReader.hpp"

// constructor
CSVReader::CSVReader(const std::string& filename){
    file.open(filename);

    if(!file.is_open()){
        throw std::runtime_error("Could not open file: " + filename);
    }
}

//public methods
bool CSVReader::readRow(std::vector<std::string>& row){
    row.clear();
    std::string line; 
    
    if(!std::getline(file, line)){
        return false;
    }

    parseLine(line, row);
    return true;

}

// private methods
void CSVReader::parseLine(const std::string& line, std::vector<std::string>& row){
    std::string field; 
    bool inQuotes = false;

    for (size_t i = 0; i < line.size(); i++){
        char c = line[i];

        //Check to see if inside quotes
        if (c == '"'){
            inQuotes = !inQuotes;
        }else if (c == ',' && !inQuotes){
            row.push_back(field);
            field.clear();
        }
        else {
            field.push_back(c);
        }
    }

    row.push_back(field); //last field
}
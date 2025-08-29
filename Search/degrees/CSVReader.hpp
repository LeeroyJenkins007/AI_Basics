#ifndef CSVREADER_H
#define CSVREADER_H

#include <string>
#include <vector>
#include <fstream>

class CSVReader {
public:
    CSVReader(const std::string&);
    bool readRow(std::vector<std::string>&);
private:
    std::ifstream file;
    void parseLine(const std::string&, std::vector<std::string>&);
};

#endif //CSVREADER_H
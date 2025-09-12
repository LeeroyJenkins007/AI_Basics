#ifndef CSVREADER_H
#define CSVREADER_H

#include <string>
#include <vector>
#include <fstream>

class CSVReader {
public:
    CSVReader(const std::string&, bool);
    bool readRow(std::vector<std::string>&);
    std::vector<std::string> getHeader() const;
private:
    std::ifstream file;
    std::vector<std::string> mHeaders;
    bool mHasHeader;
    void parseLine(const std::string&, std::vector<std::string>&);
};

#endif //CSVREADER_H
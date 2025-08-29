#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>

#include "CSVReader.hpp"


// person struct
struct Person {
    std::string name;
    std::string birth;
    std::unordered_set<std::string> movies;
};

// movie struct
struct Movie {
    std::string title;
    std::string year;
    std::unordered_set<std::string> stars;
};


// Maps person_ids to a dictionary of: name, birth, movies (a set of movie_ids)
std::unordered_map<std::string, Person> people;

// Maps names to a set of corresponding person_ids
std::unordered_map<std::string, std::string> names;

// Maps movie_ids to a dictionary of: title, year, stars (a set of person_ids)
std::unordered_map<std::string, Movie> movies;

void load_data(const std::string& directory){
    /*
    Load data from CSV files into memory.
    */

    //Load people
    {
        CSVReader reader(directory + "/people.csv");
        std::vector<std::string> row;

        while(reader.readRow(row)){
            for (const auto& field : row){
                std::cout << "[" << field << "] ";
            }
            std::cout << "\n";
        }
    }

    //Load movie
    {
        CSVReader reader(directory + "/movies.csv");
        std::vector<std::string> row;

        while(reader.readRow(row)){
            for (const auto& field : row){
                std::cout << "[" << field << "] ";
            }
            std::cout << "\n";
        }
    }

    //Load stars
    {
        CSVReader reader(directory + "/stars.csv");
        std::vector<std::string> row;

        while(reader.readRow(row)){
            for (const auto& field : row){
                std::cout << "[" << field << "] ";
            }
            std::cout << "\n";
        }
    }

}

int main(int argc, char* argv[]) {

    std::string directory;

    if (argc > 2){
        std::cerr << "Usage: ./degrees [directory]" << std::endl;
        return 0;
    }

    directory = argc > 1 ? argv[1] : "large";


    std::cout << "Loading Data..." << std::endl;
    load_data(directory);
    std::cout << "Data Loaded." << std::endl;


    return 0;
}
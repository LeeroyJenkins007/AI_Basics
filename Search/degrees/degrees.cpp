#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>


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

void load_data(){
    """
    Load data from CSV files into memory.
    """
}

int main(int argc, char* argv[]) {

    // Maps person_ids to a dictionary of: name, birth, movies (a set of movie_ids)
    std::unordered_map<std::string, Person> people;

    // Maps names to a set of corresponding person_ids
    std::unordered_map<std::string, std::string> names;

    // Maps movie_ids to a dictionary of: title, year, stars (a set of person_ids)
    std::unordered_map<std::string, Movie> movies;


    return 0;
}
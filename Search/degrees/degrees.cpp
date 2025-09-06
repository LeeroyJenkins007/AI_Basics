#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

#include "CSVReader.hpp"
#include "util.hpp"


// person struct
struct Person {
    std::string name;
    std::string birth;
    std::unordered_set<std::string> movies;

    Person() : movies() {}

    void print() const{
        std::cout << "[" << name << "] [" << birth << "] " << "#M: " << std::to_string(movies.size()) << "\n";
    }
};

// movie struct
struct Movie {
    std::string title;
    std::string year;
    std::unordered_set<std::string> stars;

    Movie() : stars() {}

    void print() const {
        std::cout << "[" << title << "] [" << year << "] " << "#S: " << std::to_string(stars.size()) << "\n";
    }
};


//struct pair_equal {
//    bool operator()(const std::pair<std::string, std::string>& a,
//                    const std::pair<std::string, std::string>& b) const {
//        return a.first == b.first && a.second == b.second;                
//    }
//};

struct pairHash{
    size_t operator() (const std::pair<std::string, std::string>& x) const{
        auto h1 = std::hash<std::string>{}(x.first);
        auto h2 = std::hash<std::string>{}(x.second);
        return h1 ^ (h2 << 1);
    };
};



// Maps person_ids to a dictionary of: name, birth, movies (a set of movie_ids)
std::unordered_map<std::string, Person> people;

// Maps names to a set of corresponding person_ids
std::unordered_map<std::string, std::string> names;

// Maps movie_ids to a dictionary of: title, year, stars (a set of person_ids)
std::unordered_map<std::string, Movie> movies;

/*
* Loads in the data from a CSV file into memory
*/
void load_data(const std::string& directory){

    //Load people
    {
        CSVReader reader(directory + "/people.csv", true);
        std::vector<std::string> row;

        while(reader.readRow(row)){

            Person person;
            person.name = row[1];
            person.birth = row[2];

            people.insert({row[0], person});

            for_each(person.name.begin(), person.name.end(), [](char& c){
                c = std::tolower(c);
            });

            if(names.find(person.name) == names.end()){
                names[person.name] = row[0];
            }else{
                names.insert({person.name, row[0]});
            }
        }
    }
    

    //Load movie
    {
        CSVReader reader(directory + "/movies.csv", true);
        std::vector<std::string> row;

        while(reader.readRow(row)){
            Movie movie;
            movie.title = row[1];
            movie.year = row[2];

            movies.insert({row[0], movie});
        }
    }

    //Load stars
    {
        CSVReader reader(directory + "/stars.csv", true);
        std::vector<std::string> row;

        while(reader.readRow(row)){
            try {
                people[row[0]].movies.insert(row[1]);
                movies[row[1]].stars.insert(row[0]);
            }catch(const std::runtime_error& e){
                std::cerr << "Caught exception: " << e.what() << std::endl;
                continue;
            }
        }
    }


    //for (const auto& pair : people){
    //    std::cout << "[" << pair.first << "] ";
    //    pair.second.print();
    //}

    //for (const auto& pair : movies){
    //    std::cout << "[" << pair.first << "] ";
    //    pair.second.print();
    //}

    //for (const auto& pair : names){
    //    std::cout << "[" << pair.first << "] [" << pair.second << "]\n";
    //}
}


/*
* Returns the IMDB id for a person's name,
*    resolving ambiguities as needed.
*/
std::string person_id_for_name(std::string name){

    for_each(name.begin(), name.end(), [](char& c){
                c = std::tolower(c);
            });

    if (names.find(name) == names.end())
    {
        std::cerr << "There is no one by the name of " << name << " in the database" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::cout << "ID: " << names.at(name) << std::endl;
    return names.at(name);
}

/*
*    Returns (movie_id, person_id) pairs for people
*    who starred with a given person.
*/
std::unordered_set<std::pair<std::string, std::string>, pairHash> neighbors_for_person(std::string person_id){
    std::unordered_set movie_ids = people[person_id].movies;
    std::unordered_set<std::pair<std::string, std::string>, pairHash> neighbors;

    std::cout << "Neighbors for " << person_id << ":\n";
    for(const auto& movie_id : movie_ids){
        std::cout << "Movies: " << movie_id << "\n";
        for(const auto& neighbor_id : movies[movie_id].stars){
            
            neighbors.insert({movie_id, neighbor_id});
        }
    }

    
   // std::string pmovie_id;
    //std::string pneighbor_id;
    //for(const auto& pair : neighbors){
    //    pmovie_id = pair.first;
    //    pneighbor_id = pair.second;
    //    std::cout << "(" << pmovie_id << ", " << pneighbor_id << ")\n";
    //}

    return neighbors;
}

/*
*    source is a person_id
*    target is a person_id
*
*    Returns the shortest list of (movie_id, person_id) pairs
*    that connect the source to the target.
*
*   If no possible path, returns None.
*/
std::vector<std::pair<std::string, std::string>> shortest_path(std::string source, std::string target){
    QueueFrontier frontier;
    std::unordered_set<std::string> visited;

    std::vector<std::pair<std::string, std::string>> path;
    {
        Node* newNode = new Node(source, nullptr, "");
        frontier.add(newNode);
        std::cout << "FRONT: " << newNode->state << "\n";
    }

    while(!frontier.empty()){
        Node* node = frontier.remove();
        visited.insert(node->state);
        for(const Node* node : frontier){
            std::cout << "FRONT: " << node->state << "\n";
        }

        if (node->state == target){

            std::cout << "Found Target!\n";
            // reverse path
            while(node->parent != nullptr){
                std::pair<std::string, std::string> newPair = {node->action, node->state};
                path.insert(path.begin(), newPair);
                node = node->parent;
            }
            delete node;
            return path;

        }else{
            for(const auto& pair : neighbors_for_person(node->state)){

                bool inVisited = !(visited.find(pair.second) == visited.end());
                bool inFrontier = frontier.contains_state(pair.second);

                //std::cout << pair.second << " is in Visited? " << std::to_string(inVisited) << " and in Frontier? " << std::to_string(inFrontier) << "\n";
                if(!inVisited && !inFrontier){
                    std::cout << "Adding to the frontier\n";
                    Node* newNode = new Node(pair.second, node, pair.first);
                    frontier.add(newNode);
                }
            }
        }
        delete node;
    }
    
    return path;
}


int main(int argc, char* argv[]) {

    std::string directory;
    std::vector<std::pair<std::string, std::string>> path;

    if (argc > 2){
        std::cerr << "Usage: ./degrees [directory]" << std::endl;
        return 0;
    }

    directory = argc > 1 ? argv[1] : "large";


    std::cout << "Loading Data..." << std::endl;
    load_data(directory);
    std::cout << "Data Loaded." << std::endl;

    std::string source;
    std::string target; 

    std::cout << "Name: \n";
    std::getline(std::cin, source);
    source = person_id_for_name(source);

    std::cout << "Name: \n";
    std::getline(std::cin, target);
    target = person_id_for_name(target);

    path = shortest_path(source, target);

    if(path.empty()){
        std::cout << "Not Connected.\n";
    }else{
        int degrees = path.size();
        std::cout << std::to_string(degrees) << " degrees of separation.\n";

        for (int i = 0; i < degrees; i++){
            std::string person1 = people[path[i].second].name;
            std::string person2 = people[path[i+1].second].name;
            std::string movie = movies[path[i+1].first].title;

            std::cout << std::to_string(i+1) << ": " << person1 << " and " << person2 << " starred in " << movie << std::endl;
        }
    }

    return 0;
}
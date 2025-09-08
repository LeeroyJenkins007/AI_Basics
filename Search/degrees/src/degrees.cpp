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
std::unordered_map<std::string, std::unordered_set<std::string>> names;

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

            // regardless of in the map or not itll add or create accordingly
            names[person.name].insert(row[0]); 

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


    const std::unordered_set<std::string>& ids = names[name];

    if (ids.size() > 1){
        std::cout << "Which " << name << "?\n";

        for (const auto& id : ids){
            const Person &person = people[id];
            std::cout << "ID: " << id 
                      << ", Name: " << person.name 
                      << ", Birth: " << person.birth << "\n";
        }
        std::string person_id;
        while(true){
            std::cout << "Intended Person ID: ";
            std::getline(std::cin, person_id);

            if (ids.find(person_id) == ids.end()){
                std::cerr << "That is not a valid ID. Try again. \n";
            }else{
                return person_id;
            } 
        }
    }else{
        // return the first and only element in the set of name_ids
        return *ids.begin();
    }

    
}

/*
*    Returns (movie_id, person_id) pairs for people
*    who starred with a given person.
*/
std::unordered_set<std::pair<std::string, std::string>, pairHash> neighbors_for_person(std::string person_id){
    std::unordered_set movie_ids = people[person_id].movies;
    std::unordered_set<std::pair<std::string, std::string>, pairHash> neighbors;

    for(const auto& movie_id : movie_ids){
        for(const auto& neighbor_id : movies[movie_id].stars){
            
            neighbors.insert({movie_id, neighbor_id});
        }
    }


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
    }

    while(!frontier.empty()){
        Node* node = frontier.remove();
        visited.insert(node->state);

        if (node->state == target){

            // reverse path
            while(node != nullptr){
                // check when at source node via action==""
                if(!node->action.empty()){
                    path.insert(path.begin(), {node->action, node->state});
                }
                node = node->parent;
            }
            frontier.clear();
            return path;

        }else{
            for(const auto& pair : neighbors_for_person(node->state)){

                bool inVisited = !(visited.find(pair.second) == visited.end());
                bool inFrontier = frontier.contains_state(pair.second);

                if(!inVisited && !inFrontier){
                    Node* newNode = new Node(pair.second, node, pair.first);
                    frontier.add(newNode);
                }
            }
        }
    }
    
    frontier.clear();
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
            std::string person1 = (i == 0 ? people[source].name : people[path[i-1].second].name);
            std::string person2 = people[path[i].second].name;
            std::string movie = movies[path[i].first].title;

            std::cout << i+1 << ": " << person1 << " and " << person2 << " starred in " << movie << std::endl;
        }
    }

    return 0;
}
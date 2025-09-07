
#include "util.hpp"


// adds node to the top of the stack
void StackFrontier::add(Node* node){
    frontier.push_back(node);
}

// check stack if it contains a node with state
bool StackFrontier::contains_state(const std::string s){
    for(Node* node : frontier){
        if(node->state == s){
            return true;
        }
    }
    return false;
}

// checks if the stack is empt
bool StackFrontier::empty(){
    return frontier.empty();
}

// remove node from the top of the stack
Node* StackFrontier::remove(){
    if (frontier.empty()) {
        std::cerr << "Can't remove from empty frontier!" << std::endl;
        std::exit(EXIT_FAILURE);
    }else{
        Node* node = frontier.back();
        frontier.pop_back();
        return node;
    }
}

// same methods with the exception that a queue pops off nodes from the front
Node* QueueFrontier::remove(){
    if (frontier.empty()) {
        std::cerr << "Can't remove from empty frontier!" << std::endl;
        std::exit(EXIT_FAILURE);
    }else{
        Node* node = frontier.front();
        frontier.erase(frontier.begin());
        return node;
    }
}


// clears the frontier of Nodes
void StackFrontier::clear(){
    while(!frontier.empty()){
        Node* node = StackFrontier::remove();
        delete node;
    }
}

// destructor
StackFrontier::~StackFrontier(){
    StackFrontier::clear();
}
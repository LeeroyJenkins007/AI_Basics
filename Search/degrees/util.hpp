#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <string>
#include <iostream>

struct Node{
    std::string state;
    Node* parent;
    std::string action;

    Node(const std::string& s, Node* p = nullptr, const std::string& a = ""):state(s), parent(p), action(a){}
};

class StackFrontier{
protected:
    std::vector<Node*> frontier; 

public:
    void add(Node*);
    bool contains_state(const std::string);
    bool empty();
    Node* remove();

    // Iterator access
    auto begin() {return frontier.begin();}
    auto end() {return frontier.end();}
    auto begin() const {return frontier.begin();}
    auto end() const {return frontier.end();}
};

class QueueFrontier : public StackFrontier{
public:
    Node* remove();
};

#endif //UTIL_H
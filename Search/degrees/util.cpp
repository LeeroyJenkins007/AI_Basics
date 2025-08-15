#include <vector>

struct Node{
    std::string state;
    Node* parent;
    std::string action;

    Node(const std::string& s, Node* p = nullptr, const std::string& a = ""):state(s), parent(p), action(a){}
};

class StackFrontier{
private:
    std::vector<Node*> frontier; 

public:

    // adds node to the top of the stack
    void add(Node* node){
        frontier.push_back(node);
    }

    // check stack if it contains a node with state
    bool contains_state(const std::string s){
        for(Node* node : frontier){
            node->state == s ? return true : continue;
        }
        return false;
    }

    // checks if the stack is empty
    bool empty(){
        return frontier.empty();
    }

    // remove node from the top of the stack
    Node* remove(){
        if (frontier.empty()) {
            std::cout << "Can't remove from empty frontier!" << end::endl;
        }else{
            Node* node = frontier.pop_back();
            return node;
        }
    }
};

class QueueFrontier:StackFrontier{
    // same methods with the exception that a queue pops off nodes from the front
    Node* remove(){
        
    }
};
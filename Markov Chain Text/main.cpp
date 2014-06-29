#include <iostream>
#include <fstream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <vector>
#include <map>

using namespace std;
using namespace boost;

class Node{
    Node* parent = 0;
    map<string, Node> children;
    int samples = 0;
public:
    string name = "";
    Node* sampleChild(string);
    float getPercentage(); // get parent samples!
    void printNode(string app);
    Node* pickChild();
    Node* getChild(string);
};

Node* Node::sampleChild(string str){
    this->samples ++;
    Node* child = &this->children[str];
    if(!child->parent){
        child->parent = this;
        child->name = str;
    }
    child->samples ++;
    
    return child;
}

float Node::getPercentage(){
    if(parent){
        return ( (float) this->samples / (float) this->parent->samples);
    } else {
        return 1.0;
    }
}

void Node::printNode(string app){
    for(map<string, Node>::iterator itr2 = children.begin(); itr2!= children.end(); itr2++){
        cout << app << itr2->second.name << ":" << itr2->second.getPercentage() << endl;
        itr2->second.printNode(app + "\t\t\t");
    }
}

Node* Node::pickChild(){
    float select = ((float)rand()/(float) RAND_MAX);
    float currentTotal = 0.0;
    for(map<string, Node>::iterator itr2 = children.begin(); itr2!= children.end(); itr2++){
        currentTotal += itr2->second.getPercentage();
        if(currentTotal >= select){
            return &itr2->second;
        }
    }
    return NULL;
}

Node* Node::getChild(string c){
    return &children[c];
}


int main(int argc, const char * argv[])
{
    srand( (unsigned int) time(NULL) );
    std::ifstream file("/Users/thomasturner/data.txt");
    
    
    Node* rootNode = new Node;
    
    string line;
    while(getline(file, line)){
        vector<string> splitVec;
        boost::split(splitVec,line,is_any_of(" "));
        
        for(int i=0; i<splitVec.size(); i++){
            Node * child = rootNode;
            for (int j=0; j<splitVec.size() && j<5; j++){
                child = child->sampleChild(splitVec[i+j]);
            }
        }
    }
    
    for(int k = 1; k < 30; k++){
        Node* a = rootNode;
        string word = "";
        for(int j = 1; j < 20; j++){
            for(int i = 1; i < 20; i++){
                a = a->pickChild();
                if(a != NULL){
                    word = a->name;
                    cout << word << " ";
                } else {
                    a = rootNode->getChild(word);
                }
            }
            cout << endl << endl;
        }
    }
    
    delete rootNode;
    return 0;
}
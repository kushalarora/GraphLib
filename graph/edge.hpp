#include<iostream>
using namespace std;
// forward declaration to resolve circular dependency.
#ifndef __EDGE__
#define __EDGE__
class Node;
class Edge {
    Node* node1;
    Node* node2;
    bool is_directed;
    float weight;
    Edge* next;
    const static float DEFAULT_WEIGHT = 0.0f;
    public:
        Edge(Node* n1, Node* n2);
        Edge(Node* n1, Node* n2, bool is_directed, float weight);
        Edge(Node* n1, Node* n2, bool is_directed);
        Edge(Node* n1, Node* n2, float weight);
        Edge* getNext() {return next;}
        void setNext(Edge* next) {this->next = next;}
        Node* getCurrentNode() {return node1;}
        Node* getOtherNode() { return node2;}
        bool isDirected() {return is_directed;}
        float getWeight() {return weight;}
        bool operator ==(Edge* edge2);
        void printEdge();
};
#endif

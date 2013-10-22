#include<iostream>

#include "graph.hpp"
using namespace std;

class Node {
    string label;   // labels are unique
    int value;      // need not be unique
    Edge<Node>* edge_list;     // linked list of edges.
    bool operator ==(Node* node2);
    int adj_index;
    string createRandomLabels(int nVertices);

    public:
        Node() {}
        Node(int value);
        Node(string label);
        Node(int value, string label);
        Node(Node* node);
        int getValue() {return value;}
        void setValue(int val) {value = val;}
        Edge<Node>* getEdgeList() {return edge_list;}
        Edge<Node>* setEdgeList(Edge<Node>* edge) {edge_list = edge;}
        string getLabel() {return label;}
        void setLabel(string lbl) {label = lbl;}
        void setAdjecencyIndex(int val) {adj_index = val;}
        int  getAdjecencyIndex() {return adj_index;}
        virtual Node* populateNode(bool labelled, bool valued, int seed);
        void printNode();

};

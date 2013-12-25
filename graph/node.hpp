#include    <iostream>
#include<time.h>
#include<stdlib.h>
#include "edge.hpp"     //forward declaration will not work, because in decructor we call method of edge.
#ifndef __NODE__
#define __NODE__
using namespace std;

template<class V, class E> class Graph;

template<typename T>
class Node {

    public:
        static string DEFAULT_LABEL;

        Node(T& value);
        Node(T& value, string label);
        Node(const Node& node);

        Node* getSource() const {return source;}
        int getId() const {return id;}
        T& getValue() const {return value;}
        Edge<T>* getEdgeList() const {return edge_list;}
        Edge<T>* setEdgeList(Edge<T>* edge) {edge_list = edge;}
        string getLabel() const {return label;}
        int  getAdjecencyIndex() const {return adj_index;}
        virtual void populateNode(bool labelled, int seed);
        virtual void printNode();
        virtual void reset();
        ~Node();

        // Traversal Specific
        enum COLOR {WHITE, GRAY, BLACK};
        COLOR getColor() const {return color;}
        Node* getParent(Node* node) const {return parent;}

        // BFS specific
        int getDist2Source() const {return dist2s;}

        // DFS Specific
        int getEntryTime() const { return entry_index;}
        int getExitTime() const { return exit_index;}

        static int getNewId() {return count++;}
        int assignId() { id = getNewId();}


        friend ostream& operator <<(ostream& os, const Node& node);
        template<class V, class E> friend class Graph;

    private:
        string label;   // labels are unique
        T& value;      // need not be unique
        Edge<T>* edge_list;     // linked list of edges.
        bool operator ==(Node* node2);
        int adj_index;
        int id;
        static int count;

        // Traversal Specific Variabls
        COLOR color;
        Node* parent;
        Node* source;

        // Populated with BFS
        int dist2s;

        // Populated while DFS
        int entry_index;    // Global Count of nodes processed, when encountered
        int exit_index;     // Global count of nodes when blackened.

        // Spanning Tree Related
        bool inTree;

        void setValue(T& val) {value = val;}
        void setLabel(string lbl) {label = lbl;}
        void setAdjecencyIndex(int index) {adj_index = index;}

        string createRandomLabels(int nVertices);

        void setColor(COLOR color) {this->color = color;}
        void setParent(Node* node) {this->parent = node;}
        void setSource(Node* node) {this->source = node;}
        void setDist2Source(int dist) {this->dist2s = dist;}
        void setEntryTime(int entry_idx) { entry_index = entry_idx;}
        void setExitTime(int exit_idx) { exit_index = exit_idx;}
};


template<typename T>
int Node<T>::count = 0;

template<typename T>
string Node<T>::DEFAULT_LABEL = "";

template<typename T>
Node<T>::Node(T& val, string lbl):
    value(val),
    label(lbl),
    edge_list(NULL),
    adj_index(-1),
    id(count++),

    // Traversal Specific
    color(WHITE),
    parent(NULL),
    source(NULL),

    // BFS specific
    dist2s(-1),

    // DFS specific
    entry_index(-1),
    exit_index(-1),

    // Spanning Tree Related
    inTree(false) {}

template<typename T>
void Node<T>::reset() {
    color = WHITE;
    parent = NULL;
    source = NULL;

    // BFS specific
    dist2s = -1;

    // DFS specific
    entry_index = -1;
    exit_index = -1;

    // Spanning Tree Related
    bool inTree = false;
}

template<typename T>
Node<T>::Node(T& val):
    value(val),
    label(DEFAULT_LABEL),
    edge_list(NULL),
    adj_index(-1),
    id(count++),

    // Traversal Specific
    color(WHITE),
    parent(NULL),
    source(NULL),

    // BFS specific
    dist2s(-1),

    // DFS specific
    entry_index(-1),
    exit_index(-1),

    // Spanning Tree Related
    inTree(false) {}


template<typename T>
Node<T>::Node(const Node<T>& node):
    value(node.getValue()),
    label(node.getLabel()),
    edge_list(NULL),
    adj_index(node.getAdjecencyIndex()) {}

// nodes are equal if either they are same or have same label.
template<typename T>
bool Node<T>::operator==(Node<T>* node2) {
    if (this == node2)  // if pointer matches, return true
        return true;
    if (this->label.length() > 0 && node2->label.length() > 0)  // if label is defined match that.
        return this->label == node2->label;
    return false;
};

template<typename T>
void Node<T>::printNode() {
    cout << "( " << getId() << " ";
    if (this->getLabel().length() > 0)
        cout << getLabel() << " ";
    if (getValue() > -1)
         cout << getValue() << " ";
     cout << ")";
}

template<typename T>
string Node<T>::createRandomLabels(int nVertices) {
    int len = nVertices < 10 ? 2 : nVertices < 100 ? 4 : nVertices < 1000 ? 5 : 8;
    char sbuilder[len];
    for (int i = 0; i < len; i++) {
        sbuilder[i] = 'a' + rand() % 26;
    }
    string str = string(sbuilder, len);
    return str;
}

template<typename T>
void Node<T>::populateNode(bool labelled, int seed) {
    if (labelled)
        this->setLabel(createRandomLabels(seed));
}

template<typename T>
Node<T>::~Node() {
    Edge<T>* edge = getEdgeList();
    Edge<T>* tmp;
    while(edge != NULL) {
        tmp = edge->getNext();
        delete edge;
        edge = tmp;
    }
}

template<typename T>
ostream& operator <<(ostream& os, const Node<T>& node) {
    os << node.label;
}
#endif

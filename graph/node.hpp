#include    <iostream>
#include<time.h>
#include<stdlib.h>
#include "edge.hpp"     //forward declaration will not work, because in decructor we call method of edge.
#ifndef __NODE__
#define __NODE__
using namespace std;

/*
 * Node class to be used by Graph. Can and should be subclassed while implementing new algorithms over graph
 * that would need some data tracking on node basis. Like Color, is touched etc.
 *
 * For standard algorithms, the interface exposed is set to minimum
 * User interacts with only limited members. These members are either set by user(value and label) or identify node uniquely(id for example).
 * These members only are garunteed to user while copying or assigning objects.
 * At the same time only these variable are used in testing equality between two objects.
 */


template<class V, class E> class Graph;

template<typename T>
class Node {
    protected:
        enum COLOR {WHITE, GRAY, BLACK};
        enum RESET {HARD_RESET, SOFT_RESET};
    public:
        static string DEFAULT_LABEL;

        Node(T& value);
        Node(T& value, string label);
        Node(const Node& node);

        // Populate node with random label
        virtual void populateNode(int graph_size);

        // Prints the value of node
        // User can add more values by subclassing
        // Again in default mode should print what is exposed by getters
        virtual void printNode();


        // Class must expose getters for only those
        // members that can be set by users
        //
        // So on similar lines only these values
        // should be used to define equality
        //
        // User should not be concerned with how graph
        // manipulates these variables
        // Its not in user domain
        int getId() const {return id;}
        T& getValue() const {return value;}
        string getLabel() const {return label;}

        bool operator ==(const Node& node2);
        bool operator !=(const Node& node2);
        Node& operator =(const Node& node2);

    private:
        static int count;
        string label;   // labels are unique
        T& value;      // need not be unique
        int id;

        int adj_index;

        Edge<T>* edge_list;     // linked list of edges.
        int out_deg;
        int in_deg;

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
        bool in_tree;

    protected:


        virtual void reset(RESET reset);
        void setValue(T& val) {value = val;}
        void setLabel(string lbl) {label = lbl;}

        static string createRandomLabels(int nVertices);

        Edge<T>* getEdgeList() const {return edge_list;}
        void setEdgeList(Edge<T>* edge) {edge_list = edge;}

        int getAdjecencyIndex() const {return adj_index;}
        void setAdjecencyIndex(int index) {adj_index = index;}


        void incOutDegree() {out_deg++;}
        void incInDegree() {in_deg++;}
        void decOutDegree() {out_deg--;}
        void decInDegree() {in_deg--;}

        int getInDegree() const {return in_deg;}
        int getOutDegree() const {return out_deg;}

        // Traversal Specific
        COLOR getColor() const {return color;}
        void setColor(COLOR color) {this->color = color;}

        Node* getParent() const {return parent;}
        void setParent(Node& node) {this->parent = &node;}

        Node* getSource() const {return source;}
        void setSource(Node& node) {this->source = &node;}

        // BFS specific
        int getDist2Source() const {return dist2s;}
        void setDist2Source(int dist) {this->dist2s = dist;}

        // DFS Specific
        int getEntryTime() const { return entry_index;}
        void setEntryTime(int entry_idx) { entry_index = entry_idx;}

        int getExitTime() const { return exit_index;}
        void setExitTime(int exit_idx) { exit_index = exit_idx;}

        void setInTree(bool in_tree) {this->in_tree = in_tree;}
        bool isInTree() const {return in_tree;}

        friend ostream& operator <<(ostream& os, const Node& node);
        template<class V, class E> friend class Graph;
        friend class TestNode;
};


template<typename T>
int Node<T>::count = 0;

template<typename T>
string Node<T>::DEFAULT_LABEL = "";

template<typename T>
Node<T>::Node(T& val, string lbl):
    value(val),
    label(lbl),
    id(count++),
    adj_index(-1),
    edge_list(NULL),
    out_deg(0),
    in_deg(0),

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
    in_tree(false) {}

template<typename T>
void Node<T>::reset(RESET reset) {
    if (reset == HARD_RESET) {
        edge_list = NULL;
        adj_index = -1;
        in_deg = 0;
        out_deg = 0;
    }
    color = WHITE;
    parent = NULL;
    source = NULL;

    // BFS specific
    dist2s = -1;

    // DFS specific
    entry_index = -1;
    exit_index = -1;

    // Spanning Tree Related
    bool in_tree = false;
}

template<typename T>
Node<T>::Node(T& val):
    value(val),
    label(DEFAULT_LABEL),
    edge_list(NULL),
    adj_index(-1),
    id(count++),
    in_deg(0),
    out_deg(0),

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
    in_tree(false) {}

template<typename T>
Node<T>::Node(const Node<T>& node):
    value(node.getValue()),
    label(node.getLabel()),
    id(node.getId()),
    edge_list(node.getEdgeList()),
    adj_index(node.getAdjecencyIndex()),
    in_deg(node.getInDegree()),
    out_deg(node.getOutDegree()),

    // Traversal Specific
    color(node.getColor()),
    parent(node.getParent()),
    source(node.getSource()),

    // BFS specific
    dist2s(node.getDist2Source()),

    // DFS specific
    entry_index(node.getEntryTime()),
    exit_index(node.getExitTime()),

    // Spanning Tree Related
    in_tree(node.isInTree()) {}


// nodes are equal if either they are same or have same label.
template<typename T>
bool Node<T>::operator==(const Node<T>& node2) {
            // if pointer matches, return true
    return (this == &node2 ||
            // else if id matches
            id == node2.getId() ||
            // else value should match and
            (getValue() == node2.getValue() &&
            // and label too if not default
             ((getLabel() == DEFAULT_LABEL &&
              node2.getLabel() == DEFAULT_LABEL) ||
             getLabel() == node2.getLabel())));
}

template<typename T>
bool Node<T>::operator!=(const Node<T>& node2) {
    return !(*this == node2);
}

template<typename T>
Node<T>& Node<T>::operator =(const Node<T>& node) {
    value = node.getValue();
    label = node.getLabel();
    id = node.getId();
    edge_list = node.getEdgeList();
    adj_index = node.getAdjecencyIndex();
    in_deg = node.getInDegree();
    out_deg = node.getOutDegree();

    // Traversal Specific
    color = node.getColor();
    parent = node.getParent();
    source = node.getSource();

    // BFS specific
    dist2s = node.getDist2Source();

    // DFS specific
    entry_index = node.getEntryTime();
    exit_index = node.getExitTime();

    // Spanning Tree Related
    in_tree = node.isInTree();
    return *this;
}
template<typename T>
void Node<T>::printNode() {
    cout << "(" << getId() << ", ";
    if (this->getLabel().length() > 0)
        cout << getLabel() << ", ";
     cout << getValue() << ")";
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
void Node<T>::populateNode(int graph_size) {
    this->setLabel(createRandomLabels(graph_size));
}

template<typename T>
ostream& operator <<(ostream& os, const Node<T>& node) {
    os << node.label;
}
#endif

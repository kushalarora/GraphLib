#include    <iostream>
#include<time.h>
#include<stdlib.h>
#ifndef __NODE__
#define __NODE__
using namespace std;

template<typename T>class Edge;

template<typename T>
class Node {
    string label;   // labels are unique
    T* value_ptr;      // need not be unique
    Edge<T>* edge_list;     // linked list of edges.
    bool operator ==(Node* node2);
    int adj_index;
    string createRandomLabels(int nVertices);

    public:
        Node();
        Node(T value);
        Node(string label);
        Node(T value, string label);
        Node(Node* node);
        T getValue() {return *value_ptr;}
        void setValue(T val) {value_ptr = &val;}
        Edge<T>* getEdgeList() {return edge_list;}
        Edge<T>* setEdgeList(Edge<T>* edge) {edge_list = edge;}
        string getLabel() {return label;}
        void setLabel(string lbl) {label = lbl;}
        void setAdjecencyIndex(int index) {adj_index = index;}
        int  getAdjecencyIndex() {return adj_index;}
        virtual void populateNode(bool labelled, bool valued, int seed);
        virtual void printNode();
        virtual void reset() {};
        ~Node();
        friend ostream& operator <<(ostream& os, const Node& node);
};


template<typename T>
Node<T>::Node(T val, string lbl) {
    value_ptr = &val;
    label = lbl;
    edge_list = NULL;
    adj_index = -1;
}

template<typename T>
Node<T>::Node(T val) {
    Node(val, "");
}

template<typename T>
Node<T>::Node(string lbl) {
    Node(NULL, lbl);
}

template<typename T>
Node<T>::Node() {
    Node(NULL, "");
}

template<typename T>
Node<T>::Node(Node<T>* node) {
    value_ptr = &node->getValue();
    label = node->getLabel();
    edge_list = NULL;
    adj_index = node->getAdjecencyIndex();
}

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
    cout << "( ";
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
void Node<T>::populateNode(bool labelled, bool valued, int seed) {
    if (labelled)
        this->setLabel(createRandomLabels(seed));

    if (valued)
        this->setValue(rand() % seed + 1);
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

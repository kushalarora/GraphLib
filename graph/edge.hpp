#include<iostream>
using namespace std;
// forward declaration to resolve circular dependency.
#ifndef __EDGE__
#define __EDGE__

template<typename T> class Node;

template<typename T>
class Edge {
    public:
        static int getId() { return count++;}
        enum EDGE_TYPE {NA, TREE_EDGE, BACK_EDGE, FORWARD_EDGE, CROSS_EDGE};
        Edge(Node<T>* n1, Node<T>* n2);
        Edge(Node<T>* n1, Node<T>* n2, bool is_directed, float weight);
        Edge(Node<T>* n1, Node<T>* n2, bool is_directed);
        Edge(Node<T>* n1, Node<T>* n2, float weight);
        Edge<T>* getNext() {return next;}
        void setNext(Edge<T>* next) {this->next = next;}
        Node<T>* getCurrentNode() {return node1;}
        Node<T>* getOtherNode() { return node2;}
        bool isDirected() {return is_directed;}
        float getWeight() {return weight;}
        bool operator ==(Edge<T>* edge2);
        void printEdge();
        void getEdgeType() { return type;}
        void setEdgeType(EDGE_TYPE edge_type) { this->type = edge_type;}
        void setId(int Id) { id = Id;}

    private:
        Node<T>* node1;
        Node<T>* node2;
        bool is_directed;
        float weight;
        Edge<T>* next;
        const static float DEFAULT_WEIGHT = 0.0f;
        static int count;
        EDGE_TYPE type;
        int id;
};

template<typename T>
int Edge<T>::count = 0;

template<typename T>
Edge<T>::Edge(Node<T>* n1, Node<T>* n2, bool is_directed, float weight) {
    node1 = n1;
    node2 = n2;
    this->is_directed = is_directed;
    this->weight = weight;
    this->next = NULL;
    id = -1;
    type = NA;
}

template<typename T>
void Edge<T>::printEdge() {
    cout << (isDirected() ? "--" : "<--");
    cout << " " << id;
    if (getWeight() != DEFAULT_WEIGHT)
        cout << ", " << getWeight();
    cout <<" "<< "-->";
}

template<typename T>
Edge<T>::Edge(Node<T>* n1, Node<T>* n2) {
    Edge(n1, n2, false, DEFAULT_WEIGHT);
}

template<typename T>
Edge<T>::Edge(Node<T>* n1, Node<T>* n2, bool is_directed) {
    Edge(n1, n2, is_directed, DEFAULT_WEIGHT);
}

template<typename T>
Edge<T>::Edge(Node<T>* n1, Node<T>* n2, float weight) {
    Edge(n1, n2, false, weight);
}

template<typename T>
bool Edge<T>::operator ==(Edge* edge2) {
    if (edge2 == NULL)
        return false;

    if (this->getCurrentNode() == edge2->getCurrentNode() &&
            this->getOtherNode() == edge2->getOtherNode() &&
                (this->getWeight() == edge2->getWeight()))
            return true;
    return false;
}
#endif

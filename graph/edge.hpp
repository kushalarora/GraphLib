#include<iostream>
using namespace std;
// forward declaration to resolve circular dependency.
#ifndef __EDGE__
#define __EDGE__

template<typename T> class Node;
template<class V, class E> class Graph;

template<typename T>
class Edge {
    public:
        const static float DEFAULT_WEIGHT = 0.0f;
        static int getNewId() { return count++;}
        enum EDGE_TYPE {NA, TREE_EDGE, BACK_EDGE, FORWARD_EDGE, CROSS_EDGE};
        Edge(Node<T>& n1, Node<T>& n2);
        Edge(Node<T>& n1, Node<T>& n2, bool is_directed, float weight);
        Edge(Node<T>& n1, Node<T>& n2, bool is_directed);
        Edge(Node<T>& n1, Node<T>& n2, float weight);
        Edge<T>* getNext() const {return next;}
        Node<T>& getCurrentNode() const {return node1;}
        Node<T>& getOtherNode() const { return node2;}
        bool isDirected() const {return is_directed;}
        float getWeight() const {return weight;}
        EDGE_TYPE getType() const { return type;}
        int getId() const { return id;}
        bool operator ==(const Edge<T>& edge2);
        bool operator !=(const Edge<T>& edge2);
        void printEdge();
        void reset();

    private:
        Node<T>& node1;
        Node<T>& node2;
        bool is_directed;
        float weight;
        Edge<T>* next;
        Edge<T>* previous;
        static int count;
        EDGE_TYPE type;
        int id;

        void setNext(Edge<T>* next) {
            this->next = next;
        }
        void setType(EDGE_TYPE edge_type) { this->type = edge_type;}
        void setId(int Id) { id = Id;}

        template<class V, class E> friend class Graph;
};

template<typename T>
int Edge<T>::count = 0;

template<typename T>
void Edge<T>::reset() {
    type = NA;
}

template<typename T>
Edge<T>::Edge(Node<T>& n1, Node<T>& n2, bool is_directed, float weight) :
    node1(n1),
    node2(n2),
    is_directed(is_directed),
    weight(weight),
    next(NULL),
    id(-1),
    type(NA) {}

template<typename T>
Edge<T>::Edge(Node<T>& n1, Node<T>& n2) :
    node1(n1),
    node2(n2),
    is_directed(false),
    weight(DEFAULT_WEIGHT),
    next(NULL),
    id(-1),
    type(NA) {}

template<typename T>
Edge<T>::Edge(Node<T>& n1, Node<T>& n2, bool is_directed) :
    node1(n1),
    node2(n2),
    is_directed(is_directed),
    weight(DEFAULT_WEIGHT),
    next(NULL),
    id(-1),
    type(NA) {}

template<typename T>
Edge<T>::Edge(Node<T>& n1, Node<T>& n2, float weight) :
    node1(n1),
    node2(n2),
    is_directed(false),
    weight(weight),
    next(NULL),
    id(-1),
    type(NA) {}

template<typename T>
bool Edge<T>::operator ==(const Edge& edge2) {
    return (edge2 == NULL || 
            id == edge2.getId() ||
        (this->getCurrentNode() == edge2.getCurrentNode() &&
            this->getOtherNode() == edge2.getOtherNode() &&
                (this->getWeight() == edge2.getWeight())));
}

template<typename T>
bool Edge<T>::operator !=(const Edge& edge2) {
    return !(*this == edge2);
}

template<typename T>
void Edge<T>::printEdge() {
    cout << (isDirected() ? "--" : "<--");
    cout << " " << id;
    if (getWeight() != DEFAULT_WEIGHT)
        cout << ", " << getWeight();
    cout <<" "<< "-->";
}

#endif

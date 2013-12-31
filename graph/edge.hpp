#include<iostream>
using namespace std;
// forward declaration to resolve circular dependency.
#ifndef __EDGE__
#define __EDGE__

template<typename T> class Node;
template<class V, class E> class Graph;

/*
 * User cannot create an edge. At most it can do is get an edge
 * and run some getters on it to know the nodes associated with it,
 * its weight etc.
 *
 * Class can be subclassed to add more fields in user domain or for accounting
 * A good example will be a subclassing to be reperesent edge in knowledge bases
 * where edge will represent relation between two categories.
 */

template<typename T>
class Edge {
    public:
        const static float DEFAULT_WEIGHT = 0.0f;
        enum EDGE_TYPE {NA, TREE_EDGE, BACK_EDGE, FORWARD_EDGE, CROSS_EDGE};
        Edge(const Edge<T>& edge2);

        Node<T>& getCurrentNode() const {return node1;}
        Node<T>& getOtherNode() const { return node2;}
        bool isDirected() const {return is_directed;}
        float getWeight() const {return weight;}
        int getId() const { return id;}
        virtual void printEdge();

        bool operator ==(const Edge<T>& edge2);
        bool operator !=(const Edge<T>& edge2);
        Edge& operator =(const Edge<T>& edge2);

    private:
        Node<T>& node1;
        Node<T>& node2;
        bool is_directed;
        float weight;
        Edge<T>* next;
        static int count;
        EDGE_TYPE type;
        int id;
    protected:

        // As with the node, user has no business of constructing an edge
        // This is sole perogative of graph
        // Hence except for copy constructor all edges should be hidden.
        Edge(Node<T>& n1, Node<T>& n2);
        Edge(Node<T>& n1, Node<T>& n2, bool is_directed, float weight);
        Edge(Node<T>& n1, Node<T>& n2, bool is_directed);
        Edge(Node<T>& n1, Node<T>& n2, float weight);

        void setId(int Id) { id = Id;}
        static int getNewId() { return count++;}


        void setNext(Edge<T>* next) { this->next = next; }
        Edge<T>* getNext() const {return next;}

        EDGE_TYPE getType() const { return type;}
        void setType(EDGE_TYPE edge_type) { this->type = edge_type;}

        // Edge can be reset only by graph.
        // User have no business of reseting an edge
        virtual void reset();

        template<class V, class E> friend class Graph;
        friend class TestEdge;
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
Edge<T>::Edge(const Edge<T>& edge2) :
    node1(edge2.getCurrentNode()),
    node2(edge2.getOtherNode()),
    is_directed(edge2.isDirected()),
    weight(edge2.getWeight()),
    id(-1),
    next(NULL),
    type(NA) {}

template<typename T>
bool Edge<T>::operator ==(const Edge& edge2) {
    return ( // return true if id is same or
            (id == edge2.getId()) ||
            // both nodes are same and
            (
             (this->getCurrentNode() == edge2.getCurrentNode()) &&
                (this->getOtherNode() == edge2.getOtherNode()) &&
                // both are either directed or undirected and
                (isDirected() == edge2.isDirected()) &&
                // weight too is same.
                (this->getWeight() == edge2.getWeight())
            )
           );
}

template<typename T>
bool Edge<T>::operator !=(const Edge& edge2) {
    return !(*this == edge2);
}

template<typename T>
Edge<T>& Edge<T>::operator =(const Edge& edge2) {
    node1 = edge2.getCurrentNode();
    node2 = edge2.getOtherNode();
    is_directed = edge2.isDirected();
    weight = edge2.getWeight();
    id = edge2.getId();
    return *this;
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

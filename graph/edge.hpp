#include<iostream>
using namespace std;
// forward declaration to resolve circular dependency.
#ifndef __EDGE__
#define __EDGE__

template<class V, class E> class GraphInterface;

/*
 * User cannot create an edge. At most it can do is get an edge
 * and run some getters on it to know the nodes associated with it,
 * its weight etc.
 *
 * Class can be subclassed to add more fields in user domain or for accounting
 * A good example will be a subclassing to be reperesent edge in knowledge bases
 * where edge will represent relation between two categories.
 */

class Edge {
    public:
        const static float DEFAULT_WEIGHT = 0.0f;
        enum EDGE_TYPE {NA, TREE_EDGE, BACK_EDGE, FORWARD_EDGE, CROSS_EDGE};
        Edge(const Edge& edge2);

        int getCurrentNodeId() const {return node1_id;}
        int getOtherNodeId() const { return node2_id;}
        bool isDirected() const {return is_directed;}
        float getWeight() const {return weight;}
        int getId() const { return id;}
        virtual void printEdge() const;

        bool operator ==(const Edge& edge2);
        bool operator !=(const Edge& edge2);
        Edge& operator =(const Edge& edge2);

    private:
        int node1_id;
        int node2_id;
        bool is_directed;
        float weight;
        Edge* next;
        static int count;
        EDGE_TYPE type;
        int id;
    protected:

        // As with the node, user has no business of constructing an edge
        // This is sole perogative of graph
        // Hence except for copy constructor all edges should be hidden.
        Edge(int n1_id, int n2_id);
        Edge(int n1_id, int n2_id, bool is_directed, float weight);
        Edge(int n1_id, int n2_id, bool is_directed);
        Edge(int n1_id, int n2_id, float weight);

        void setId(int Id) { id = Id;}
        static int getNewId() { return count++;}


        void setNext(Edge* next) { this->next = next; }
        Edge* getNext() const {return next;}

        EDGE_TYPE getType() const { return type;}
        void setType(EDGE_TYPE edge_type) { this->type = edge_type;}

        // Edge can be reset only by graph.
        // User have no business of reseting an edge
        virtual void reset();

        template<class V, class E> friend class GraphInterface;
        friend class TestEdge;
};

int Edge::count = 0;

void Edge::reset() {
    type = NA;
}

Edge::Edge(int n1_id, int n2_id, bool is_directed, float weight) :
    node1_id(n1_id),
    node2_id(n2_id),
    is_directed(is_directed),
    weight(weight),
    next(NULL),
    id(-1),
    type(NA) {}

Edge::Edge(int n1_id, int n2_id) :
    node1_id(n1_id),
    node2_id(n2_id),
    is_directed(false),
    weight(DEFAULT_WEIGHT),
    next(NULL),
    id(-1),
    type(NA) {}

Edge::Edge(int n1_id, int n2_id, bool is_directed) :
    node1_id(n1_id),
    node2_id(n2_id),
    is_directed(is_directed),
    weight(DEFAULT_WEIGHT),
    next(NULL),
    id(-1),
    type(NA) {}

Edge::Edge(int n1_id, int n2_id, float weight) :
    node1_id(n1_id),
    node2_id(n2_id),
    is_directed(false),
    weight(weight),
    next(NULL),
    id(-1),
    type(NA) {}


Edge::Edge(const Edge& edge2) :
    node1_id(edge2.getCurrentNodeId()),
    node2_id(edge2.getOtherNodeId()),
    is_directed(edge2.isDirected()),
    weight(edge2.getWeight()),
    id(-1),
    next(NULL),
    type(NA) {}

bool Edge::operator ==(const Edge& edge2) {
    return ( // return true if id is same or
            (id == edge2.getId()) ||
            // both nodes are same and
            (
             (this->getCurrentNodeId() == edge2.getCurrentNodeId()) &&
                (this->getOtherNodeId() == edge2.getOtherNodeId()) &&
                // both are either directed or undirected and
                (this->getWeight() == edge2.getWeight())
            )
           );
}

bool Edge::operator !=(const Edge& edge2) {
    return !(*this == edge2);
}

Edge& Edge::operator =(const Edge& edge2) {
    node1_id = edge2.getCurrentNodeId();
    node2_id = edge2.getOtherNodeId();
    is_directed = edge2.isDirected();
    weight = edge2.getWeight();
    id = edge2.getId();
    return *this;
}

void Edge::printEdge() const {
    cout << (isDirected() ? "--" : "<--");
    cout << " " << id;
    if (getWeight() != DEFAULT_WEIGHT)
        cout << ", " << getWeight();
    cout <<" "<< "-->";
}
#endif

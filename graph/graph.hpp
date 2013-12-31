/*
 * GRAPH ALOGRITHMS
 *
 * 1. Types of Graphs
 *      Undirected vs Directed
 *      Weighted vs Unweighted
 *          The difference is particularly apperant in shortest path problem.
 *          For Unweighted graph its BFS. For Weighted Graph we need Djzxtra and Bellman Ford.
 *      Simple vs Non Simple
 *          Self Loop and multiedges present - Non Simple
 *      Sparse vs Dense
 *          Typically is edges are linear to vertices, its sparse.
 *          If quadratic its dense.
 *      Cyclic vs Acyclic
 *          Acyclic doesn't contain any cycles.
 *          Example Trees(Connected Acyclic Undirected Graphs{Hoping you use doubly linked structure})
 *          Directed Acyclic Graph, example scheduling problem.
 *          Topological Sorting orders the vertices of *DAG* according to constraints.
 *          Topological Sorting is first step in any DAG problem.
 *      Embedded vs Topological
 *          Graph embedded in space where each vertices is assigned a geometric position.
 *          Topological graph are where geometry of the graph is important.
 *
 * 2. Representation of a Graph
 *      1. Adjacency Matrix
 *          M[i, j] =   1 if (i,j) has an edge else 0
 *          Faster queries like "is (i,j) in G, faster updates, deletion
 *          Excessive space for sparse graph. Used only for dense graph.
 *
 *      2. Adjacency List
 *          Used all the time as most real life graphs are sparse.
 *          Linked representation of graph.
 *          An array of nodes containing all vertices.
 *          Each array node is a linked list containing all edges directed outward/inwards of the graph.
 *          Generally outward is used.
 *          Each node contains
 *              i.   Label
 *              ii.  Weight
 *              iii. Value
 *              iv.  Pointer to next edge node.
 *
 *          Apart from this the main structure contains some journaling info like
 *              i.  No. of vertices.
 *              ii. No. of edges
 *              iii.  boolean directed.
 */

#include<iostream>
#include<string>
#include<stdlib.h>     // for rand()
#include<time.h>
#include<assert.h>
#include<queue>
#include<map>
#include<set>
#include<vector>

#include "edge.hpp"
#include "node.hpp"
using namespace std;
#ifndef __GRAPH__
#define __GRAPH__

template<class V, class E>
class Graph {
    public:
        enum RESET {HARD_RESET, SOFT_RESET};
        Graph();
        Graph(Graph& graph);
        ~Graph();
        Graph(bool directed, bool weighted, bool labelled);
        void insertNode(V& val);
        void createEdge(V& V1, V& V2);
        void createEdge(V& V1, V& V2, float weight);
        inline bool isWeighted() const {return weighted;}
        inline bool isDirected() const {return directed;}
        inline bool isLabelled() const {return labelled;}
        virtual void printGraph();
        virtual void createRandomGraph(int nVertices, V** nodes);
        virtual void createRandomGraph(int nVertices, V** nodes, bool connected);
        virtual void createRandomGraph(int nVertices, float density, V** nodes);
        virtual void createRandomGraph(int nVertices, float density, V** nodes, bool connected);
        virtual void createRandomGraph(int nVertices, float density, bool strictly_acyclic, V** nodes);
        virtual void createRandomGraph(int nVertices, float density, bool strictly_acyclic, V** nodes, bool connected);
        int getNVertices() const {return edgeNode.size();}
        int getNEdge() const {return (isDirected() ? nEdges : nEdges/2);}
        void reset();
        void reset(RESET reset);
        bool isCyclic();
        void topsort();
        void transpose();

        typedef typename map<int, V>::iterator iterator;
        iterator begin()  { return edgeNode.begin();}
        iterator end()  { return edgeNode.end();}
        bool containsNode(const V& node) { return edgeNode.find(node.getId()) != edgeNode.end();}
        vector<E>& getOutEdgesForNode(V& node);
        int getInDegreeForNode(const V& node);
        int getOutDegreeForNode(const V& node);

        // Traversal Specific functions
        void BreadthFirstSearch(V& source);
        void DepthFirstRoutine(V& node);
        void DepthFirstSearch();
        bool operator ==(Graph& graph);
        Graph& operator =(Graph& graph);

    private:
        int nEdges;
        bool directed;
        bool weighted;
        bool labelled;

        // Set would have been a better data structure as then the comparison would have been on
        // all fields not just id
        // But the problem is that set returns const reference while dereferencing iterator
        // So using map instead.
        map<int, V> edgeNode;
        void sort(iterator begin, iterator end);
    protected:
        virtual void deleteEdge(E* edge);

        void hardResetGraph();
        virtual void processEdge(E* edge) {
#ifdef DEBUG
            cout << "Processed Edge";
            (edge->getCurrentNode()).printNode();
            edge->printEdge();
            (edge->getOtherNode()).printNode();
            cout << "\n";
            cout << "Edge turned " << edge->getType() << endl;
#endif
        }
        virtual void processOnBlack(V& node) {
#ifdef DEBUG
            cout << "Node Turned Black ";
            node.printNode();
            cout << "\n";
#endif
        };
        virtual void processOnGrey(V& node) {
#ifdef DEBUG
            cout << "Node turned Grey ";
            node.printNode();
            cout << "\n";
#endif
        }
};

template<class V, class E>
Graph<V,E>::Graph(bool dirctd, bool wghtd, bool lbled) :
    nEdges(0),
    directed(dirctd),
    weighted(wghtd),
    labelled(lbled) {}

template<class V, class E>
Graph<V,E>::Graph() :
    nEdges(0),
    directed(false),
    weighted(false),
    labelled(false) {}


template<class V, class E>
void Graph<V,E>::insertNode(V& node) {
    // Allocated from heap
    // Else once function goes out of scope
    // memory will disappear
    V *node1 = new V(node);
    node1->setAdjecencyIndex(getNVertices());
    if (edgeNode.find(node.getId()) == edgeNode.end())
        edgeNode.insert(std::pair<int, V>(node.getId(), *node1));
}

template<class V, class E>
int Graph<V,E>::getInDegreeForNode(const V& node) {
    iterator it = edgeNode.find(node.getId());
    if (it == edgeNode.end()) {
        cerr << "Node not found";
        return -1;
    }
    return (it->second).getInDegree();
}

template<class V, class E>
int Graph<V,E>::getOutDegreeForNode(const V& node) {
    iterator it = edgeNode.find(node.getId());
    if (it == edgeNode.end()) {
        cerr << "Node not found";
        return -1;
    }
    return (it->second).getOutDegree();
}

template<class V, class E>
vector<E>& Graph<V,E>::getOutEdgesForNode(V& node) {
    vector<E>* edges = new vector<E>();

    iterator it;
    V* internal_node = NULL;
    if ((it = edgeNode.find(node.getId())) != edgeNode.end()) {
        internal_node = &it->second;
    }

    if (internal_node == NULL) {
        cerr << "Node not found";
    } else {
        E* edge = internal_node->getEdgeList();
        while(edge != NULL) {
            edges->push_back(*edge);
            edge = edge->getNext();
        }
    }
    return *edges;
}
template<class V, class E>
void Graph<V,E>::createEdge(V& V1, V& V2, float weight) {
    V* tempArr[2] = {&V1, &V2};
    V* nodeArr[2] = {NULL, NULL};
    iterator it;
    for (int i = 0; i < 2; i++) {
        if ((it = edgeNode.find(tempArr[i]->getId())) != edgeNode.end()) {
            nodeArr[i] = &(it->second);
        }

        // ensure that both nodes were inserted into graph.
        assert(nodeArr[i] != NULL);
        assert(nodeArr[i]->getAdjecencyIndex() != -1);
    }

    E* newEdge;
    E* temp;
    int i = 0, idx = 0;

    int id = E::getNewId();
    for (i = 0; i < (isDirected() ? 1: 2); i++, idx = 1 - idx) {
        V* currNode = nodeArr[idx];
        V* othrNode = nodeArr[1 - idx];
        newEdge = new E(*currNode, *othrNode, isDirected(), weight);
        newEdge->setId(id);
        // inserting edge to v2 in v1
        temp = currNode->getEdgeList();
        E* prevEdge = temp;
        while(temp != NULL) {
            // if V2 already present do nothing.
            if (((V&)temp->getOtherNode()) == *nodeArr[1 - idx])
                return;
            prevEdge = temp;
            temp = temp->getNext();
        }
        if (prevEdge != NULL)
            prevEdge->setNext(newEdge);
        else
            currNode->setEdgeList(newEdge);

        currNode->incOutDegree();
        if (isDirected())
            othrNode->incInDegree();
        else
            currNode->incInDegree();
        nEdges++;
    }
}

template<class V, class E>
void Graph<V,E>::createEdge(V& V1, V& V2) {
    createEdge(V1, V2, 0);
}

template<class V, class E>
void Graph<V,E>::printGraph() {
    iterator it;
    for (iterator it = begin(); it != end(); it++) {
        (it->second).printNode();

        E* tmp = (it->second).getEdgeList();
        while (tmp != NULL) {
            assert(((V&)tmp->getCurrentNode()) == it->second);
            tmp->printEdge();
            (tmp->getOtherNode()).printNode();
            tmp = tmp->getNext();
        }
        cout<<"\n"<<"\n";
    }
}


template<class V, class E>
void Graph<V,E>::createRandomGraph(int nVertices, float density, bool strictly_acyclic, V** nodes) {
    createRandomGraph(nVertices, density, strictly_acyclic, nodes, false);
}

template<class V, class E>
void Graph<V,E>::createRandomGraph(int nVertices, float density, bool strictly_acyclic, V** nodes, bool connected) {
    srand(time(NULL));
    if (nVertices < 1)
        return;
    for (int i = 0; i < nVertices; i++) {
        nodes[i]->populateNode(nVertices);
        insertNode(*nodes[i]);
    }
    int connected_edges = 0;
    if (connected) {

        // TODO: No need of it being pointer.
        // In STL containers, everything is by reference
        set<V*> s;
        typedef typename set<V>::iterator set_iterator;
        for (iterator it = begin(); it != end(); it++) {
            s.insert(&(it->second));
        }

        while (!s.empty()) {
            // get the first element in set and remove it from set.
            V* node1P = *s.begin();
            s.erase(node1P);

            V* node2P = &edgeNode.at(nodes[rand() % nVertices]->getId());

            createEdge(*node1P, *node2P, (isWeighted() ? rand() % 100 + 1: 0.0));
            connected_edges++;
        }
    }
    int nEdges = nVertices * (density > 0.0 ? density * nVertices : (rand() % nVertices)) - connected_edges;

    // Code to create random edges
    for (long i = 0; i < nEdges; i++) {
        int idx1 = rand() % nVertices;
        int idx2 = rand() % nVertices;

        // ensures that there are no self loops
        while (idx1 == idx2)
            idx2 = rand() % nVertices;

        V* node1P = &(edgeNode.find(nodes[idx1]->getId())->second);
        V* node2P = &(edgeNode.find(nodes[idx2]->getId())->second);

        if (node1P->getId() < node2P->getId() || !strictly_acyclic)
            createEdge(*node1P, *node2P, (isWeighted() ? rand() % 100  + 1: 0.0));
        else
            createEdge(*node2P, *node1P, (isWeighted() ? rand() % 100  + 1: 0.0));
    }
}

template<class V, class E>
void Graph<V,E>::createRandomGraph(int nVertices, float density, V** nodes, bool connected) {
    createRandomGraph(nVertices, density, false, nodes, connected);
}
template<class V, class E>
void Graph<V,E>::createRandomGraph(int nVertices, float density, V** nodes) {
    createRandomGraph(nVertices, density, false, nodes, false);
}

template<class V, class E>
void Graph<V,E>::createRandomGraph(int nVertices, V** nodes, bool connected) {
    createRandomGraph(nVertices, 0.0, false, nodes, connected);
}

template<class V, class E>
void Graph<V,E>::createRandomGraph(int nVertices, V** nodes) {
    createRandomGraph(nVertices, 0.0, false, nodes);
}


template<class V, class E>
void Graph<V,E>::reset() {
}

template<class V, class E>
void Graph<V,E>::reset(RESET reset) {
    if( reset == HARD_RESET) {
       hardResetGraph();
    } else {
        for (iterator it = edgeNode.begin();
                it != edgeNode.end(); it++)
            (it->second).reset(V::SOFT_RESET);
        }
    }

template<class V, class E>
Graph<V,E>::~Graph() {
    hardResetGraph();
}

template<class V, class E>
void Graph<V,E>::hardResetGraph() {
    for (iterator it = edgeNode.begin();
            it != edgeNode.end(); it++) {
        E* edge_list = (it->second).getEdgeList();
        E* edge = edge_list;
        E* tmp;
        while (edge != NULL) {
            tmp = edge->getNext();
            delete edge;
            edge = tmp;
        }
        (it->second).setEdgeList(NULL);
        (it->second).reset(V::HARD_RESET);

    }
    nEdges = 0;
    edgeNode.clear();

}

template<class V, class E>
void Graph<V,E>::BreadthFirstSearch(V& source) {

    if (edgeNode.find(source.getId()) == edgeNode.end()) {
        cerr << "Node not present" << endl;
        return;
    }

    // Get the internal corresponding node and run that.
    source = edgeNode.find(source.getId())->second;

    queue<V*> q;
    source.setColor(V::GRAY);
    E* edge;
    V* node;
    V* other;
    typename V::COLOR clr;
    q.push(&source);
    while(!q.empty()) {
        node = q.front();
        processOnGrey(*node);
        assert(node != NULL);
        edge = node->getEdgeList();
        while(edge != NULL) {
            assert(((V&)edge->getCurrentNode()) == *node);
            other = (V*)&(edge->getOtherNode());
            clr = other->getColor();
            if (clr == V::WHITE) {
                other->setColor(V::GRAY);
                other->setDist2Source(other->getDist2Source() + 1);
                other->setParent(*node);
                q.push(other);
            }
            edge = edge->getNext();
        }
        q.pop();
        node->setColor(V::BLACK);
        processOnBlack(*node);
    }
}

template<class V, class E>
void Graph<V,E>::DepthFirstRoutine(V& node) {
    static int count = 0;
    E* edge_list = node.getEdgeList();
    E* edge = edge_list;
    V* other;
    static map<int, E*> edge_mp;
    node.setEntryTime(count++);
    node.setColor(V::GRAY);
    processOnGrey(node);
    while(edge != NULL) {
        int edge_id = edge->getId();
        typename map<int, E*>::iterator it = edge_mp.find(edge_id);

        other = (V*)(&edge->getOtherNode());
        typename V::COLOR clr = other->getColor();

        if (it == edge_mp.end()) {
            edge_mp[edge_id] = edge;

            if (clr == V::WHITE)
                edge->setType(E::TREE_EDGE);
            else if (clr == V::GRAY)
                edge->setType(E::BACK_EDGE);
            else {
                int node_entry_tm = node.getEntryTime();
                int other_entry_tm = other->getEntryTime();

                if (node_entry_tm < other_entry_tm)
                    edge->setType(E::FORWARD_EDGE);
                else
                    edge->setType(E::CROSS_EDGE);
            }
        } else {
            // This means graph is undirected.
            assert (!isDirected());
            E* tmp = it->second;
            assert(tmp != NULL);

            edge->setType(tmp->getType());
        }

        processEdge(edge);

        if (clr == V::WHITE) {
            other->setParent(node);
            DepthFirstRoutine(*other);
        }
        edge = edge->getNext();
    }

    node.setExitTime(count);
    node.setColor(V::BLACK);
    processOnBlack(node);
}

template<class V, class E>
void Graph<V,E>::DepthFirstSearch() {
    static int count = 0;
    for (iterator it = begin(); it != end(); it++) {
        V& node = it->second;
        if (node.getColor() == V::WHITE)
            DepthFirstRoutine(node);
    }
}

template<class V, class E>
bool Graph<V,E>::isCyclic() {
    DepthFirstSearch();
    for (iterator it = begin(); it != end(); it++) {
        V& node = it->second;

        E* edge_list = node.getEdgeList();
        assert(edge_list != NULL);

        E* edge = edge_list;
        while(edge != NULL) {
            if (edge->getType() == E::BACK_EDGE)
                return true;
            edge = edge->getNext();
        }
        return false;
    }
}

template<class V, class E>
void Graph<V, E>::sort(iterator begin, iterator end) {
    if (begin != end) {
        V& pivotEl = begin->second;
        iterator it_begin = begin;
        iterator it_end = end;
        while(it_begin != it_end) {
            if ((it_begin->second).getExitTime() < pivotEl.getExitTime()) {
                it_begin++;
            } else {
                swap<V&>(it_begin->second, it_end->second);
                it_end--;
            }
        }
        swap<V&>(pivotEl, (it_begin - 1)->second);
        sort(begin, it_begin - 2);
        sort(it_begin, end);
    }
}

template<class V, class E>
void Graph<V,E>::topsort() {
    // Do depth first search to calculate exit Time.
    DepthFirstSearch();

#ifdef DEBUG
    for (iterator it = begin(); it != end(); it++)
        (it->second).printNode();
    cout << endl;
#endif

    sort(begin(), end() - 1);

#ifdef DEBUG
    for (iterator it = begin(); it != end(); it++)
        (it->second).printNode();
    cout << endl;
#endif
}

template<class V, class E>
bool Graph<V, E>::operator ==(Graph<V,E>& graph) {
    if (getNVertices() != graph.getNVertices()) {
        return false;
    }

    if (getNEdge() != graph.getNEdge()) {
        return false;
    }

    for(iterator it = graph.begin(); it != graph.end(); it++) {

        if (edgeNode.find(it->first) == edgeNode.end()) {
            return false;
        }

        V* node = &(edgeNode.find(it->first)->second);

        E* tmp1 = node->getEdgeList();
        E* tmp2 = (it->second).getEdgeList();

        bool edge_found = false;
        while(tmp2 != NULL) {
            while(tmp1 != NULL) {
                if (*tmp1 == *tmp2)
                    edge_found = true;
                tmp1 = tmp1->getNext();
            }
            if (!edge_found) {
                return false;
            }
            tmp2 = tmp2->getNext();
        }
    }
    return true;
}

template<class V, class E>
Graph<V,E>::Graph(Graph<V,E>& graph) {
    directed = graph.isDirected();
    weighted = graph.isWeighted();
    labelled = graph.isLabelled();
    nEdges = 0;
    for(iterator it = graph.begin(); it != graph.end(); it++) {
         V* node = new V(it->second);
         node->reset(V::HARD_RESET);
         insertNode(*node);
    }
    assert(graph.getNVertices() == getNVertices());

    for(iterator it = graph.begin(); it != graph.end(); it++) {
        E* tmp = (it->second).getEdgeList();
        while(tmp != NULL) {
            createEdge( edgeNode.at((tmp->getCurrentNode()).getId()),
                        edgeNode.at((tmp->getOtherNode()).getId()),
                        tmp->getWeight());
            tmp = tmp->getNext();
        }
    }
}

template<class V, class E>
Graph<V,E>& Graph<V,E>::operator =(Graph<V,E>& graph) {
    directed = graph.isDirected();
    weighted = graph.isWeighted();
    labelled = graph.isLabelled();
    nEdges = 0;
    for(iterator it = graph.begin(); it != graph.end(); it++) {
         V* node = new V(it->second);
         node->reset(V::HARD_RESET);
        insertNode(*node);
    }
    assert(graph.getNVertices() == getNVertices());

    for(iterator it = graph.begin(); it != graph.end(); it++) {
        E* tmp = (it->second).getEdgeList();
        while(tmp != NULL) {
            createEdge( edgeNode.at((tmp->getCurrentNode()).getId()),
                        edgeNode.at((tmp->getOtherNode()).getId()),
                        tmp->getWeight());
            tmp = tmp->getNext();
        }
    }
    return *this;
}

template<class V, class E>
void Graph<V,E>::deleteEdge(E* edge) {
    V& currNode = (V&)edge->getCurrentNode();
    V& otherNode = (V&)edge->getOtherNode();
    E* edge_list = currNode.getEdgeList();
    if (edge == edge_list) {
        currNode.setEdgeList(edge->getNext());
    } else if (edge->getNext() != NULL) {
        E* tmp = edge->getNext();
        edge->setNext(tmp->getNext());
        delete tmp;
    } else {
        E* tmp = edge_list;
        while(tmp->getNext() != edge) {
            tmp = tmp->getNext();
        }
        tmp->setNext(NULL);
    }

    currNode.decOutDegree();
    if (isDirected())
        otherNode.decInDegree();
    else
        currNode.decInDegree();
    nEdges--;
}

template<class V, class E>
void Graph<V,E>::transpose() {
    if (isDirected()) {
        int largest_edge_id = -1;
        for (iterator it = edgeNode.begin(); it != edgeNode.end(); it++) {
            E* tmp = (it->second).getEdgeList();

            while(tmp != NULL) {
                if (tmp->getId() > largest_edge_id) {
                    largest_edge_id = tmp->getId();
                }
                tmp = tmp->getNext();
            }
        }
        for (iterator it = edgeNode.begin(); it != edgeNode.end(); it++) {
            E* tmp = (it->second).getEdgeList();

            while(tmp != NULL) {
                V& node1 = (V&)tmp->getCurrentNode();
                V& node2 = (V&)tmp->getOtherNode();
                float weight = tmp->getWeight();
                if (tmp->getId() <= largest_edge_id) {
                    E* tmp2 = tmp->getNext();
                    deleteEdge(tmp);
                    createEdge(node2, node1, weight);
                    tmp = tmp2;
                } else {
                    tmp = tmp->getNext();
                }
            }
        }
    }
}
#endif

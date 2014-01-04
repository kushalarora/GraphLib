/*
 * m
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
#include<list>
#include<algorithm>

#include "edge.hpp"
#include "node.hpp"
using namespace std;
#ifndef __GRAPH__
#define __GRAPH__

template<class V, class E=Edge>
class GraphBase {
    public:
        GraphBase(const GraphBase& graph);
        ~GraphBase();
        class ComponentGraph;
        enum RESET {HARD_RESET, SOFT_RESET};
        void insertNode(V& val);
        bool createEdge(V& V1, V& V2);
        bool createEdge(V& V1, V& V2, float weight);

        inline bool isDirected() const {return directed;}

        virtual void printGraph() const;

        virtual void createRandomGraph(int nVertices, V** nodesArr);
        virtual void createRandomGraph(int nVertices, V** nodesArr, float density);
        virtual void createRandomGraph(int nVertices, V** nodesArr, float density, bool weighted);
        virtual void createRandomGraph(int nVertices, V** nodesArr, float density, bool weighted, bool connected);
        virtual void createRandomGraph(int nVertices, V** nodesArr, float density, bool weighted, bool connected, bool strictly_acyclic);

        int getNodeCount() const {return nodes.size();}
        int getEdgeCount() const {return (isDirected() ? nEdges : nEdges/2);}

        void reset();
        void reset(RESET reset);

        bool isCyclic();
        void topsort();
        void transpose();

        typedef typename vector<V>::iterator iterator;
        typedef typename vector<V>::const_iterator const_iterator;
        const_iterator cbegin() const { return nodes.begin();}
        const_iterator cend() const { return nodes.end();}

        iterator begin() { return nodes.begin();}
        iterator end() { return nodes.end();}

        bool containsNode(const V& node) { return id_idx_mp.find(node.getId()) != id_idx_mp.end();}
        vector<E>& getOutEdgesForNode(const V& node) const;
        int getInDegreeForNode(const V& node) const;
        int getOutDegreeForNode(const V& node) const;

        // Traversal Specific functions
        void breadthFirstSearch(V& source);
        void depthFirstSearch();

        bool operator ==(const GraphBase& graph);
        GraphBase& operator =(const GraphBase& graph);

        ComponentGraph& stronglyConnectedComponents();
        V& getNodeById(int id);
    private:
        int nEdges;
        bool directed;

        // Set would have been a better data structure as then the comparison would have been on
        // all fields not just id
        // But the problem is that set returns const reference while dereferencing iterator
        // So using map instead.
        map<int, int> id_idx_mp;
        vector<V> nodes;

        typedef typename map<const int, int>::iterator mp_iterator;
        typedef typename map<const int, int>::const_iterator const_mp_iterator;

    protected:
        virtual void deleteEdge(E* edge);
        void depthFirstRoutine(V& node, int component_id);
        void hardResetGraph();
        virtual void processEdge(E* edge);
        virtual void processOnBlack(V& node);
        virtual void processOnGrey(V& node);
        static bool compareExitTimeInc(const V& node1, const V& node2) { return node1.getExitTime() < node2.getExitTime();}
        GraphBase();
        GraphBase(bool directed);
};

template<class V, class E>
V& GraphBase<V,E>::getNodeById(int id) {
    mp_iterator it;
    if ((it = id_idx_mp.find(id)) == id_idx_mp.end()) {
        cout << id_idx_mp.size() << endl;
        cerr << "Node not found Id:" << id <<endl;
        exit(-1);
    }
    return nodes[it->second];
}
template<class V, class E>
class GraphBase<V,E>::ComponentGraph {
    private:
        vector< GraphBase<V,E> >* graphs;
        vector<E>cross_edges;


    public:
        void addEdge(const E edge);
        GraphBase& getGraphForComponentId(int i);
        ComponentGraph(int component_count, bool is_directed) {
            graphs = new vector< GraphBase<V,E> >(component_count, GraphBase<V,E>(is_directed));
        }
        typedef typename vector<GraphBase>::iterator graph_iterator;
        typedef typename vector<E>::iterator edge_iterator;

        int size() { return graphs->size();}
        graph_iterator graph_begin() {return graphs->begin();}
        graph_iterator graph_end() {return graphs->end();}
        edge_iterator edge_begin() {return cross_edges.begin();}
        edge_iterator edge_end() {return cross_edges.end();}
};

template<class V, class E>
GraphBase<V,E>& GraphBase<V,E>::ComponentGraph::getGraphForComponentId(int i) {
    if (i >= graphs->size()) {
        cerr << "Out of bound Size: " << graphs->size() << " Idx: " << i;
        exit(-1);
    }

    return graphs->at(i);
}

template<class V, class E>
void GraphBase<V,E>::ComponentGraph::addEdge(const E edge) {
    cross_edges.push_back(Edge(edge));
}

template<class V, class E>
void GraphBase<V,E>::processEdge(E* edge) {
#ifdef DEBUG
    cout << "Processed Edge";
    getNodeById(edge->getCurrentNodeId()).printNode();
    edge->printEdge();
    getNodeById(edge->getOtherNodeId()).printNode();
    cout << "\n";
    cout << "Edge turned " << edge->getType() << endl;
#endif
}

template<class V, class E>
void GraphBase<V,E>::processOnBlack(V& node) {
#ifdef DEBUG
    cout << "Node Turned Black ";
    node.printNode();
    cout << "\n";
#endif
}

template<class V, class E>
void GraphBase<V,E>::processOnGrey(V& node) {
#ifdef DEBUG
    cout << "Node turned Grey ";
    node.printNode();
    cout << "\n";
#endif
}

template<class V, class E>
GraphBase<V,E>::GraphBase(bool dirctd) :
    nEdges(0),
    directed(dirctd) {}

template<class V, class E>
GraphBase<V,E>::GraphBase() :
    nEdges(0),
    directed(false) {}


template<class V, class E>
void GraphBase<V,E>::insertNode(V& node) {
    if (id_idx_mp.find(node.getId()) == id_idx_mp.end()) {
        V node1(node);
        node1.reset(V::HARD_RESET);
        node1.setAdjecencyIndex(getNodeCount());
        nodes.push_back(node1);
        id_idx_mp.insert(std::pair<int, int>(node1.getId(), node1.getAdjecencyIndex()));
    }
}

// Not using getNodeById because we dont want to exit.
// Just returning error.
template<class V, class E>
int GraphBase<V,E>::getInDegreeForNode(const V& node) const {
    const_mp_iterator it = id_idx_mp.find(node.getId());
    if (it == id_idx_mp.end()) {
        cerr << "Node not found";
        return -1;
    }
    return nodes[(it->second)].getInDegree();
}

// Not using getNodeById because we dont want to exit.
// Just returning error.
template<class V, class E>
int GraphBase<V,E>::getOutDegreeForNode(const V& node) const {
    const_mp_iterator it = id_idx_mp.find(node.getId());
    if (it == id_idx_mp.end()) {
        cerr << "Node not found";
        return -1;
    }
    return nodes[(it->second)].getOutDegree();
}

// Not using getNodeById because we dont want to exit.
// Just returning empty vector.
template<class V, class E>
vector<E>& GraphBase<V,E>::getOutEdgesForNode(const V& node) const {
    vector<E>* edges = new vector<E>();

    const_mp_iterator it;
    const V* internal_node;
    if ((it = id_idx_mp.find(node.getId())) != id_idx_mp.end()) {
        internal_node = &nodes[it->second];
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
bool GraphBase<V,E>::createEdge(V& V1, V& V2, float weight) {
    V* tempArr[2] = {&V1, &V2};
    V* nodeArr[2] = {NULL, NULL};
    mp_iterator it;
    for (int i = 0; i < 2; i++) {
        nodeArr[i] = &getNodeById(tempArr[i]->getId());
        assert(nodeArr[i]->getAdjecencyIndex() != -1);
    }

    E* newEdge;
    E* temp;
    int i = 0, idx = 0;

    int id = E::getNewId();
    for (i = 0; i < (isDirected() ? 1: 2); i++, idx = 1 - idx) {
        V* currNode = nodeArr[idx];
        V* othrNode = nodeArr[1 - idx];
        newEdge = new E(currNode->getId(),
                        othrNode->getId(),
                        isDirected(),
                        weight);
        newEdge->setId(id);

        // inserting edge to v2 in v1
        temp = currNode->getEdgeList();
        E* prevEdge = temp;

        while(temp != NULL) {
            // if V2 already present do nothing.
            if (temp->getOtherNodeId() == nodeArr[1 - idx]->getId() && temp->weight == weight)
                return false;
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
    return true;
}

template<class V, class E>
bool GraphBase<V,E>::createEdge(V& V1, V& V2) {
    return createEdge(V1, V2, 0);
}

template<class V, class E>
void GraphBase<V,E>::printGraph() const {
    for (const_iterator it = cbegin(); it != cend(); it++) {
        it->printNode();

        E* tmp = it->getEdgeList();
        while (tmp != NULL) {
            assert(tmp->getCurrentNodeId() == it->getId());
            tmp->printEdge();
            nodes[id_idx_mp.at(tmp->getOtherNodeId())].printNode();
            tmp = tmp->getNext();
        }
        cout<<"\n"<<"\n";
    }
}


template<class V, class E>
void GraphBase<V,E>::createRandomGraph(int nVertices, V**nodesArr, float density, bool weighted, bool connected, bool strictly_acyclic) {
    srand(time(NULL));
    if (nVertices < 1)
        return;
    for (int i = 0; i < nVertices; i++) {
        nodesArr[i]->populateNode(nVertices);
        insertNode(*nodesArr[i]);
    }
    int connected_edges = 0;
    if (connected) {

        vector<V*> s;
        for (iterator it = begin(); it != end(); it++) {
            s.push_back(&(*it));
        }

        while (s.size() > 1) {
            // get the first element in set and remove it from set.
            V* node1 = s[0];
            s.erase(s.begin());
            int idx = rand() % (s.size());
            V* node2 = s[idx];
            createEdge(*node1, *node2, (weighted ? rand() % 100 + 1: 0.0));
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

        V& node1 = nodes.at(idx1);
        V& node2 = nodes.at(idx2);

        if (node1.getId() < node2.getId() || !strictly_acyclic)
            createEdge(node1, node2, (weighted ? rand() % 100  + 1: 0.0));
        else
            createEdge(node2, node1, (weighted ? rand() % 100  + 1: 0.0));
    }
}

template<class V, class E>
void GraphBase<V,E>::createRandomGraph(int nVertices, V**nodesArr, float density, bool weighted, bool connected) {
    createRandomGraph(nVertices, nodesArr, density, weighted, connected, false);
}

template<class V, class E>
void GraphBase<V,E>::createRandomGraph(int nVertices, V**nodesArr, float density, bool weighted) {
    createRandomGraph(nVertices, nodesArr, density, weighted, false, false); // connected=false, strictly_acyclic=false
}

template<class V, class E>
void GraphBase<V,E>::createRandomGraph(int nVertices, V**nodesArr, float density) {
    createRandomGraph(nVertices, nodesArr, density, false, false, false); // weighted=false, connected=false, strictly_acyclic=false
}

template<class V, class E>
void GraphBase<V,E>::createRandomGraph(int nVertices, V**nodesArr) {
    createRandomGraph(nVertices, nodesArr, 0, false, false, false); // density=0(take random value), weighted=false, connected=false, strictly_acyclic=false

}


template<class V, class E>
void GraphBase<V,E>::reset() {
    reset(SOFT_RESET);
}

template<class V, class E>
void GraphBase<V,E>::reset(RESET reset) {
    if( reset == HARD_RESET) {
       hardResetGraph();
    } else {
        for (iterator it = begin(); it != end(); it++)
            it->reset(V::SOFT_RESET);
    }
}

template<class V, class E>
GraphBase<V,E>::~GraphBase() {
    hardResetGraph();
}

template<class V, class E>
void GraphBase<V,E>::hardResetGraph() {
    for (iterator it = begin(); it != end(); it++) {
        E* edge = it->getEdgeList();
        E* tmp = NULL;
        while (edge != NULL) {
            tmp = edge->getNext();
            delete edge;
            edge = tmp;
        }
        it->reset(V::HARD_RESET);
    }
    nEdges = 0;
    id_idx_mp.clear();
    nodes.clear();
}

template<class V, class E>
void GraphBase<V,E>::breadthFirstSearch(V& source) {

    if (id_idx_mp.find(source.getId()) == id_idx_mp.end()) {
        cerr << "Node not present" << endl;
        return;
    }

    // Get the internal corresponding node and run that.
    source = getNodeById(source.getId());

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
            assert(edge->getCurrentNodeId() == node->getId());
            other = (V*)&(getNodeById(edge->getOtherNodeId()));
            clr = other->getColor();
            if (clr == V::WHITE) {
                other->setDist2Source(other->getDist2Source() + 1);
                other->setColor(V::GRAY);
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
void GraphBase<V,E>::depthFirstRoutine(V& node, int component_id) {
    static int count = 0;
    E* edge = node.getEdgeList();
    V* other;
    static map<int, E*> edge_mp;
    node.setEntryTime(count++);
    node.setColor(V::GRAY);
    node.setComponentId(component_id);
    processOnGrey(node);
    while(edge != NULL) {
        int edge_id = edge->getId();
        typename map<int, E*>::iterator it = edge_mp.find(edge_id);

        other = (V*)(&getNodeById(edge->getOtherNodeId()));
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
            depthFirstRoutine(*other, component_id);
        }
        edge = edge->getNext();
    }

    node.setExitTime(count++);
    node.setColor(V::BLACK);
    processOnBlack(node);
}

template<class V, class E>
void GraphBase<V,E>::depthFirstSearch() {
    int component_id = 0;
    for (iterator it = begin(); it != end(); it++) {
        if (it->getColor() == V::WHITE)
            depthFirstRoutine(*it, component_id++);
    }
}

template<class V, class E>
bool GraphBase<V,E>::isCyclic() {
    depthFirstSearch();
    for (iterator it = begin(); it != end(); it++) {

        E* edge = it->getEdgeList();
        while(edge != NULL) {
            if (edge->getType() == E::BACK_EDGE)
                return true;
            edge = edge->getNext();
        }
        return false;
    }
}


template<class V, class E>
void GraphBase<V,E>::topsort() {

    // Do depth first search to calculate exit Time.
    depthFirstSearch();

#ifdef DEBUG
    for (iterator it = begin(); it != end(); it++) {
        cout << it->getExitTime()<<" ";
    }
    cout << endl;
#endif

    sort(begin(), end(), compareExitTimeInc);

    // Indexes have changed so reset edge map
    // and reset adj values.
    int i = 0;
    for (iterator it = begin(); it != end(); it++) {
        it->setAdjecencyIndex(i++);
        id_idx_mp[it->getId()] = it->getAdjecencyIndex();
    }

#ifdef DEBUG

    for (iterator it = begin(); it != end(); it++) {
        cout << it->getExitTime() << " ";
    }
    cout << endl;
#endif
}

template<class V, class E>
bool GraphBase<V, E>::operator ==(const GraphBase<V,E>& graph) {
    if (getNodeCount() != graph.getNodeCount()) {
        return false;
    }

    if (getEdgeCount() != graph.getEdgeCount()) {
        return false;
    }

    for(const_iterator it = graph.cbegin(); it != graph.cend(); it++) {

        if (id_idx_mp.find(it->getId()) == id_idx_mp.end()) {
            return false;
        }

        V& node = getNodeById(it->getId());

        E* tmp1 = node.getEdgeList();
        E* tmp2 = it->getEdgeList();

        bool edge_found = false;
        while(tmp2 != NULL) {
            while(tmp1 != NULL) {
                if (*tmp1 == *tmp2) {
                    edge_found = true;
                    break;
                }
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
GraphBase<V,E>::GraphBase(const GraphBase<V,E>& graph) {
    directed = graph.isDirected();
    nEdges = 0;
    for(const_iterator it = graph.cbegin(); it != graph.cend(); it++) {
         V node(*it);
         // Copy constructor copies everything
         // All you need in new graph is user inputted data and id
         // So do a hard reset to wipe other data.
         //
         // Another reason for doing so is that we want deep copy not shallow
         // copy
         // So for that we need to create similar but new edges.
         node.reset(V::HARD_RESET);

         insertNode(node);
    }
    assert(graph.getNodeCount() == getNodeCount());

    // Copying edges
    for(const_iterator it = graph.cbegin(); it != graph.cend(); it++) {
        E* tmp = it->getEdgeList();
        while(tmp != NULL) {
            createEdge( getNodeById(tmp->getCurrentNodeId()),
                        getNodeById(tmp->getOtherNodeId()),
                        tmp->getWeight());
            tmp = tmp->getNext();
        }
    }
}

// Totally same as above.
template<class V, class E>
GraphBase<V,E>& GraphBase<V,E>::operator =(const GraphBase<V,E>& graph) {
    directed = graph.isDirected();
    nEdges = 0;
    for(const_iterator it = graph.cbegin(); it != graph.cend(); it++) {
         V node(*it);
         node.reset(V::HARD_RESET);
         insertNode(node);
    }
    assert(graph.getNodeCount() == getNodeCount());

    for(iterator it = graph.begin(); it != graph.end(); it++) {
        E* tmp = it->getEdgeList();
        while(tmp != NULL) {
            createEdge( getNodeById(tmp->getCurrentNodeId()),
                        getNodeById(tmp->getOtherNodeId()),
                        tmp->getWeight());
            tmp = tmp->getNext();
        }
    }
    return *this;
}

template<class V, class E>
void GraphBase<V,E>::deleteEdge(E* edge) {
    V& currNode = (V&)getNodeById(edge->getCurrentNodeId());
    V& otherNode = (V&)getNodeById(edge->getOtherNodeId());
    E* edge_list = currNode.getEdgeList();

    //assert(edge_list != NULL);

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
void GraphBase<V,E>::transpose() {
    if (isDirected()) {
        int largest_edge_id = -1;
        for (iterator it = begin(); it != end(); it++) {
            E* tmp = it->getEdgeList();

            while(tmp != NULL) {
                if (tmp->getId() > largest_edge_id) {
                    largest_edge_id = tmp->getId();
                }
                tmp = tmp->getNext();
            }
        }
        for (iterator it = begin(); it != end(); it++) {
            E* tmp = it->getEdgeList();

            while(tmp != NULL) {
                V& node1 = (V&)getNodeById(tmp->getCurrentNodeId());
                V& node2 = (V&)getNodeById(tmp->getOtherNodeId());
                float weight = tmp->getWeight();
                if (tmp->getId() <= largest_edge_id) {
                    E* tmp2 = tmp->getNext();
                    deleteEdge(tmp);
                    // Id is assigned in create edge
                    // so copy constructor will not be
                    // problem
                    createEdge(node2, node1, weight);
                    tmp = tmp2;
                } else {
                    tmp = tmp->getNext();
                }
            }
        }
    }
}

template<class V, class E>
typename GraphBase<V,E>::ComponentGraph& GraphBase<V,E>::stronglyConnectedComponents() {
    list<int> q;
    // top sort to run depth first search
    // and order entries according to exit time.
    topsort();
    // not calling reset because coz other field too might be populated
    for (iterator it = begin(); it != end(); it++) {

        it->setColor(V::WHITE);
        it->setComponentId(-1);
    }
    transpose();
    typename vector<V>::reverse_iterator rit;
    int components = 0;
    for (rit = nodes.rbegin(); rit != nodes.rend(); rit++) {
        if (rit->getColor() == V::WHITE) {
            depthFirstRoutine(*rit, components++);
            q.push_back(rit->getId());
        }
    }
    ComponentGraph* comp_graph = new ComponentGraph(components, isDirected());

    // To revert back to original configuration
    transpose();


    // not calling reset because coz other field too might be populated
    for (iterator it = begin(); it != end(); it++) {
        GraphBase& graph = comp_graph->getGraphForComponentId(it->getComponentId());
        graph.insertNode(*it);
        it->setColor(V::WHITE);
    }


    while (!q.empty()){
        int node_id = q.front();
        q.pop_front();
        V& node1 = getNodeById(node_id);
        int component_id = node1.getComponentId();
        GraphBase<V,E>& graph = comp_graph->getGraphForComponentId(component_id);
        node1.setColor(V::GRAY);

        E* edge = node1.getEdgeList();
        while(edge != NULL) {
            assert(node_id == edge->getCurrentNodeId());

            V& node2 = getNodeById(edge->getOtherNodeId());
            if (node2.getColor() == V::WHITE) {
                q.push_back(node2.getId());
            }
            if (component_id == node2.getComponentId()) {
                graph.createEdge(node1, node2, edge->getWeight());
            } else {
                comp_graph->addEdge(*edge);
            }
            edge = edge->getNext();
        }
        node1.setColor(V::BLACK);
    }
    return *comp_graph;
}

template<typename T>
class Graph :public GraphBase<Node<T>, Edge> {
    public:
        Graph(): GraphBase<Node<T>, Edge>(){} ;
        Graph(const Graph<T>& graph):GraphBase<Node<T>, Edge>(graph) {};
        Graph(bool directed):GraphBase<Node<T>, Edge>(directed){};
        friend class TestGraph;
};
#endif

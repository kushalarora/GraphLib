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
#include<vector>

#include "edge.hpp"
#include "node.hpp"
using namespace std;
#ifndef __GRAPH__
#define __GRAPH__
const int MAXV = 100000;

template<class V, class E>
class Graph {
    public:
        enum RESET {HARD_RESET, SOFT_RESET};
        Graph();
        ~Graph();
        Graph(bool directed, bool weighted, bool labelled);
        V& insertNode(V& node);
        void createEdge(V& V1, V& V2);
        void createEdge(V& V1, V& V2, float weight);
        inline bool isWeighted() const {return weighted;}
        inline bool isDirected() const {return directed;}
        inline bool isLabelled() const {return labelled;}
        virtual void printGraph();
        virtual void createRandomGraph(int nVertices, V** nodes);
        virtual void createRandomGraph(int nVertices, float density, V** nodes);
        virtual void createRandomGraph(int nVertices, float density, bool strictly_acyclic, V** nodes);
        int getNVertices() const {return edgeNode.size();}
        int getNEdge() const {return nEdges;}
        V& getNodeByIndex(int i);
        void reset();
        void reset(RESET reset);
        bool isCyclic();
        void topsort();
        void transpose();

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
        vector<V*> edgeNode;
        void sort(int start, int end);
    protected:
        virtual void createRandomEdges(int nEdges, int nVertices, bool ensure_acyclic);
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
V& Graph<V,E>::insertNode(V& node) {
    typename vector<V*>::iterator it;
    for (it = edgeNode.begin();
            it != edgeNode.end(); it++) {
        if (**it == node)
            // node already present
            return node;
    }
    node.setAdjecencyIndex(getNVertices());
    edgeNode.push_back(&node);
    return node;
}


template<class V, class E>
void Graph<V,E>::createEdge(V& V1, V& V2, float weight) {
    V* nodeArr[2] = {&V1, &V2};
    E* newEdge;
    E* temp;
    int i = 0, idx = 0;
    // ensure that both nodes were inserted into graph.
    assert(V1.getAdjecencyIndex() != -1);
    assert(V2.getAdjecencyIndex() != -1);

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
            if (temp->getOtherNode() == *nodeArr[1 - idx])
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
    }
    nEdges++;
}

template<class V, class E>
void Graph<V,E>::createEdge(V& V1, V& V2) {
    createEdge(V1, V2, 0);
}

template<class V, class E>
void Graph<V,E>::printGraph() {
    for (int i = 0; i < getNVertices(); i++) {
        V& node = getNodeByIndex(i);
        node.printNode();
        E* tmp = node.getEdgeList();
        while (tmp != NULL) {
            assert(tmp->getCurrentNode() == node);
            tmp->printEdge();
            (tmp->getOtherNode()).printNode();
            tmp = tmp->getNext();
        }
        cout<<"\n"<<"\n";
    }
}

template<class V, class E>
void Graph<V,E>::createRandomEdges(int nEdges, int nVertices, bool ensure_acyclic) {
#ifdef DEBUG
    cout<<"\nMaking "<<nEdges<<" Edges\n";
#endif
    for (long i = 0; i < nEdges; i++) {
        int idx1 = rand() % nVertices;
        int idx2 = rand() % nVertices;

        // ensures that there are no self loops
        while (idx1 == idx2)
            idx2 = rand() % nVertices;

        V& node1 = getNodeByIndex(idx1);
        V& node2 = getNodeByIndex(idx2);

        if (node1.getId() < node2.getId() || !ensure_acyclic)
            createEdge(node1, node2, (isWeighted() ? rand() % 100  + 1: 0.0));
        else
            createEdge(node2, node1, (isWeighted() ? rand() % 100  + 1: 0.0));
        //printNode(edgeNode[idx1]);printEdge(edgeNode[idx2]);printNode(edgeNode[idx2]);
    }
}

template<class V, class E>
void Graph<V,E>::createRandomGraph(int nVertices, float density, bool strictly_acyclic, V** nodes) {
    srand(time(NULL));
    if (nVertices < 1)
        return;
    for (int i = 0; i < nVertices; i++) {
        nodes[i]->populateNode(isLabelled(), nVertices);
        insertNode(*nodes[i]);
    }
    createRandomEdges(nVertices * (density > 0.0 ? density * nVertices : (rand() % nVertices)), nVertices, strictly_acyclic);
}

template<class V, class E>
void Graph<V,E>::createRandomGraph(int nVertices, float density, V** nodes) {
    createRandomGraph(nVertices, density, false, nodes);
}

template<class V, class E>
void Graph<V,E>::createRandomGraph(int nVertices, V** nodes) {
    createRandomGraph(nVertices, 0.0, false, nodes);
}

template<class V, class E>
V& Graph<V,E>::getNodeByIndex(int i) {
    assert(i >= 0 && i < getNVertices());
    return *edgeNode.at(i);
}

template<class V, class E>
void Graph<V,E>::reset() {
}

template<class V, class E>
void Graph<V,E>::reset(RESET reset) {
    if( reset == HARD_RESET) {
       hardResetGraph();
    } else {
        typename vector<V*>::iterator it;
        for (it = edgeNode.begin();
                it != edgeNode.end(); it++)
            (*it)->reset();
        }
    }

template<class V, class E>
Graph<V,E>::~Graph() {
    hardResetGraph();
}

template<class V, class E>
void Graph<V,E>::hardResetGraph() {
    typename vector<V*>::iterator it;
    for (it = edgeNode.begin();
            it != edgeNode.end(); it++) {
        E* edge_list = (*it)->getEdgeList();
        E* edge = edge_list;
        E* tmp;
        while (edge != NULL) {
            tmp = edge->getNext();
            delete edge;
            edge = tmp;
        }
        (*it)->setEdgeList(NULL);
        (*it)->reset();

    }
    nEdges = 0;
    edgeNode.clear();

}

template<class V, class E>
void Graph<V,E>::BreadthFirstSearch(V& source) {
    queue<V*> q;
    source->setColor(V::GRAY);
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
            assert(edge->getCurrentNode() == *node);
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
    for (int i = 0; i < getNVertices(); i++) {
        V& node = getNodeByIndex(i);
        if (node.getColor() == V::WHITE)
            DepthFirstRoutine(node);
    }
}

template<class V, class E>
bool Graph<V,E>::isCyclic() {
    DepthFirstSearch();
    for (int i = 0; i < getNVertices(); i++) {
        V& node = getNodeByIndex(i);

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
void Graph<V, E>::sort(int start, int end) {
    if (start < end) {
        int pivot = start + rand() % (end - start);
        swap<V&>(getNodeByIndex(start), getNodeByIndex(pivot));
        int i = start + 1, j = end;
        while(i <= j) {
            if (getNodeByIndex(i).getExitTime() < getNodeByIndex(start).getExitTime()) {
                i++;
            } else {
                swap<V&>(getNodeByIndex(i), getNodeByIndex(j));
                j--;
            }
        }
        swap<V&>(getNodeByIndex(start), getNodeByIndex(i - 1));
        sort(start, i - 2);
        sort(i, end);
    }
}

template<class V, class E>
void Graph<V,E>::topsort() {
    // Do depth first search to calculate exit Time.
    DepthFirstSearch();

#ifdef DEBUG
    for (int i = 0; i < getNVertices(); i++)
        getNodeByIndex(i).printNode();
    cout << endl;
#endif

    sort(0, getNVertices() - 1);

#ifdef DEBUG
    for (int i = 0; i < getNVertices(); i++)
        getNodeByIndex(i).printNode();
    cout << endl;
#endif
}

template<class V, class E>
bool Graph<V, E>::operator ==(Graph<V,E>& graph) {
    if (getNVertices() != graph.getNVertices())
        return false;

    for(int i = 0; i < getNVertices(); i++) {
        if (getNodeByIndex(i) != graph.getNodeByIndex(i))
            return false;

        E* tmp1 = getNodeByIndex(i).getEdgeList();
        E* tmp2 = graph.getNodeByIndex(i).getEdgeList();

        while(tmp2 != NULL && tmp2 != NULL) {
            if (tmp1 != tmp2)
                return false;
            tmp1 = tmp1->getNext();
            tmp2 = tmp2->getNext();
        }

        if (tmp1 != NULL || tmp2 != NULL)
            return false;
    }
    return true;
}

template<class V, class E>
Graph<V,E>& Graph<V,E>::operator =(Graph<V,E>& graph) {
    directed = graph.isDirected();
    weighted = graph.isWeighted();
    labelled = graph.isLabelled();
    map<int, V*> mp;
    for (int i = 0; i < graph.getNVertices(); i++) {
         V* node = new V(graph.getNodeByIndex(i));
        insertNode(*node);
        mp[node->getId()] = node;
    }
    assert(graph.getNVertices() == getNVertices());
    for (int i = 0; i < getNVertices(); i++) {
        V& node = graph.getNodeByIndex(i);
        E* tmp = node.getEdgeList();
        while(tmp != NULL) {
            V& node1 = tmp->getCurrentNode();
            V& node2 = tmp->getOtherNode();
            createEdge(*mp[node1.getId()], *mp[node2.getId()], tmp->getWeight());
            tmp = tmp->getNext();
        }
    }
    return *this;
}

template<class V, class E>
void Graph<V,E>::deleteEdge(E* edge) {
    V& currNode = edge->getCurrentNode();
    V& otherNode = edge->getOtherNode();
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
}

template<class V, class E>
void Graph<V,E>::transpose() {
    if (isDirected()) {
        typename vector < V* >::iterator it;
        int largest_edge_id = -1;
        for (it = edgeNode.begin(); it != edgeNode.end(); it++) {
            E* tmp = (*it)->getEdgeList();

            while(tmp != NULL) {
                if (tmp->getId() > largest_edge_id) {
                    largest_edge_id = tmp->getId();
                }
                tmp = tmp->getNext();
            }
        }
        for (it = edgeNode.begin(); it != edgeNode.end(); it++) {
            E* tmp = (*it)->getEdgeList();

            while(tmp != NULL) {
                V& node1 = tmp->getCurrentNode();
                V& node2 = tmp->getOtherNode();
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

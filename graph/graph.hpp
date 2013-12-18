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
 *              i.   Array of degree of each node.
 *              ii.  No. of vertices.
 *              iii. No. of edges
 *              iv.  boolean directed.
 */

#include<iostream>
#include<string>
#include<stdlib.h>     // for rand()
#include<time.h>
#include<assert.h>

#include "edge.hpp"
#include "node.hpp"
using namespace std;
#ifndef __GRAPH__
#define __GRAPH__
const int MAXV = 100000;

template<class T>
class Graph {
    public:
        Graph();
        ~Graph();
        Graph(bool directed, bool weighted, bool labelled, bool valued);
        Node<T>* insertNode(Node<T>* node);
        void createEdge(Node<T>* V1, Node<T>* V2);
        void createEdge(Node<T>* V1, Node<T>* V2, float weight);
        inline bool isWeighted() {return weighted;}
        inline bool isDirected() {return directed;}
        inline bool isLabelled() {return labelled;}
        inline bool isValued() {return valued;}
        virtual void printGraph();
        virtual void createRandomGraph(int nVertices);
        virtual void createRandomGraph(int nVertices, float density);
        int getNVertices() {return nVertices;}
        int getNEdge() {return nEdges;}
        Node<T>* getNodeByIndex(int i);
        void reset();
    private:
        int nVertices;
        int nEdges;
        bool directed;
        bool weighted;
        bool labelled;
        bool valued;
        int degree[MAXV];
        Node<T>* edgeNode[MAXV];
    protected:
        virtual void createRandomEdges(int nEdges, int nVertices);
};



template<class T>
Graph<T>::Graph(bool dirctd, bool wghtd, bool lbled, bool valed) {
    nVertices = 0;
    nEdges = 0;
    for (int i = 0; i < MAXV; i++) {
        degree[i] = 0;
        edgeNode[i] = NULL;
    }
    directed = dirctd;
    weighted = wghtd;
    labelled = lbled;
    valued = valed;
}
template<class T>
Graph<T>::Graph() {
    Graph(false, false, false, false);
}


template<class T>
Node<T>* Graph<T>::insertNode(Node<T>* node) {
    int i;
    for (i = 0; i < nVertices; i++) {
        if (edgeNode[i] == node)
            // node already present
            return node;
    }
    edgeNode[i] = node;
    nVertices += 1;
    degree[i] = 0;
    node->setAdjecencyIndex(i);
    return node;
}


template<class T>
void Graph<T>::createEdge(Node<T>* V1, Node<T>* V2, float weight) {
    Node<T>* nodeArr[2] = {V1, V2};
    Edge<T>* newEdge;
    Edge<T>* temp;
    int i = 0, idx = 0;
    // ensure that both nodes were inserted into graph.
    assert(V1->getAdjecencyIndex() != -1);
    assert(V2->getAdjecencyIndex() != -1);

    for (i = 0; i < (isDirected() ? 1: 2); i++, degree[idx]++, idx = 1 - idx) {
        Node<T>* currNode = nodeArr[idx];
        Node<T>* othrNode = nodeArr[1 - idx];
        newEdge = new Edge<T>((Node<T>*)currNode, (Node<T>*)othrNode, isDirected(), weight);

        // inserting edge to v2 in v1
        temp = currNode->getEdgeList();
        Edge<T>* prevEdge = temp;
        while(temp != NULL) {
            // if V2 already present do nothing.
            if (temp->getOtherNode() == nodeArr[1 - idx])
                return;
            prevEdge = temp;
            temp = temp->getNext();
        }
        if (prevEdge != NULL)
            prevEdge->setNext(newEdge);
        else
            currNode->setEdgeList(newEdge);
    }
    nEdges++;
}

template<class T>
void Graph<T>::createEdge(Node<T>* V1, Node<T>* V2) {
    createEdge(V1, V2, 0);
}

template<class T>
void Graph<T>::printGraph() {
    for (int i = 0; i < nVertices; i++) {
        Node<T>* node = edgeNode[i];
        node->printNode();
        Edge<T>* tmp = node->getEdgeList();
        while (tmp != NULL) {
            assert(tmp->getCurrentNode() == node);
            tmp->printEdge();
            (tmp->getOtherNode())->printNode();
            tmp = tmp->getNext();
        }
        cout<<"\n"<<"\n";
    }
}

template<class T>
void Graph<T>::createRandomEdges(int nEdges, int nVertices) {
#ifdef DEBUG
    cout<<"\nMaking "<<nEdges<<" Edges\n";
#endif
    for (long i = 0; i < nEdges; i++) {
        int idx1 = rand() % nVertices;
        int idx2 = rand() % nVertices;

        // ensures that there are no self loops
        while (idx1 == idx2)
            idx2 = rand() % nVertices;
        createEdge(edgeNode[idx1], edgeNode[idx2], (isWeighted() ? rand() % 100  + 1: 0.0));
        //printNode(edgeNode[idx1]);printEdge(edgeNode[idx2]);printNode(edgeNode[idx2]);
    }
}

template<class T>
void Graph<T>::createRandomGraph(int nVertices, float density) {
    srand(time(NULL));
    if (nVertices < 1)
        return;
    for (int i = 0; i < nVertices; i++) {
        Node<T>* node = new Node<T>();
        node->populateNode(isLabelled(), isValued(), nVertices);
        insertNode(node);
    }
    createRandomEdges(nVertices * (density > 0.0 ? density * nVertices : (rand() % nVertices)), nVertices);
}

template<class T>
void Graph<T>::createRandomGraph(int nVertices) {
    createRandomGraph(nVertices, 0.0);
}

template<class T>
Node<T>* Graph<T>::getNodeByIndex(int i) {
    if (i < 0 || i >= nVertices)
        return NULL;
    return edgeNode[i];
}

template<class T>
void Graph<T>::reset() {
    for (int i = 0; i < nVertices; i++) {
        edgeNode[i]->reset();
    }
}

template<class T>
Graph<T>::~Graph() {
    for (int i = 0; i < nVertices; i++) {
        delete edgeNode[i];
    }
}
#endif

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

using namespace std;

#ifndef GRAPH
#define GRAPH
const int MAXV = 100000;

// forward declaration to resolve circular dependency.

template<class T>
class Edge<T> {
    T* node1;
    T* node2;
    bool is_directed;
    float weight;
    Edge<T>* next;
    const static float DEFAULT_WEIGHT = 0.0f;
    public:
        Edge(T* n1, T* n2);
        Edge(T* n1, T* n2, bool is_directed, float weight);
        Edge(T* n1, T* n2, bool is_directed);
        Edge(T* n1, T* n2, float weight);
        Edge<T>* getNext() {return next;}
        void setNext(Edge<T>* next) {this->next = next;}
        T* getCurrentNode() {return node1;}
        T* getOtherNode() { return node2;}
        bool isDirected() {return is_directed;}
        float getWeight() {return weight;}
        bool operator ==(Edge<T>* edge2);
        void printEdge();
};

template<class T>
class Graph<T> {
    public:
        Graph();
        ~Graph();
        Graph(bool directed, bool weighted, bool labelled, bool valued);
        T* insertNode(T* node);
        void createEdge(T* V1, T* V2);
        void createEdge(T* V1, T* V2, float weight);
        inline bool isWeighted() {return weighted;}
        inline bool isDirected() {return directed;}
        inline bool isLabelled() {return labelled;}
        inline bool isValued() {return valued;}
        void printGraph();
        void createRandomGraph(int nVertices);
        void createRandomGraph(int nVertices, float density);
        int getNVertices() {return nVertices;}
        int getNEdge() {return nEdges;}
        T* getNodeByIndex(int i);
    private:
        int nVertices;
        int nEdges;
        bool directed;
        bool weighted;
        bool labelled;
        bool valued;
        int degree[MAXV];
        T* edgeNode[MAXV];
};

class Node {
    string label;   // labels are unique
    int value;      // need not be unique
    Edge<Node>* edge_list;     // linked list of edges.
    bool operator ==(Node* node2);
    int adj_index;
    string createRandomLabels(int nVertices);

    public:
        Node() {}
        Node(int value);
        Node(string label);
        Node(int value, string label);
        Node(Node* node);
        int getValue() {return value;}
        void setValue(int val) {value = val;}
        Edge<Node>* getEdgeList() {return edge_list;}
        Edge<Node>* setEdgeList(Edge<Node>* edge) {edge_list = edge;}
        string getLabel() {return label;}
        void setLabel(string lbl) {label = lbl;}
        void setAdjecencyIndex(int val) {adj_index = val;}
        int  getAdjecencyIndex() {return adj_index;}
        virtual Node* populateNode(bool labelled, bool valued, int seed);
        void printNode();

};
#endif

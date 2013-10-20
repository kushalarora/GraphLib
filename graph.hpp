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
class Edge;

class Node {
    string label;   // labels are unique
    int value;      // need not be unique
    Edge* edge_list;     // linked list of edges.
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
        Edge* getEdgeList() {return edge_list;}
        Edge* setEdgeList(Edge* edge) {edge_list = edge;}
        string getLabel() {return label;}
        void setLabel(string lbl) {label = lbl;}
        void setAdjecencyIndex(int val) {adj_index = val;}
        int  getAdjecencyIndex() {return adj_index;}
        virtual Node* populateNode(bool labelled, bool valued, int seed);
        void printNode();

};


class Edge {
    Node* node1;
    Node* node2;
    bool is_directed;
    float weight;
    Edge* next;
    const static float DEFAULT_WEIGHT = 0.0f;
    public:
        Edge(Node* n1, Node* n2);
        Edge(Node* n1, Node* n2, bool is_directed, float weight);
        Edge(Node* n1, Node* n2, bool is_directed);
        Edge(Node* n1, Node* n2, float weight);
        Edge* getNext() {return next;}
        void setNext(Edge* next) {this->next = next;}
        Node* getCurrentNode() {return node1;}
        Node* getOtherNode() { return node2;}
        bool isDirected() {return is_directed;}
        float getWeight() {return weight;}
        bool operator ==(Edge* edge2);
        void printEdge();
};

class Graph {
    public:
        Graph();
        ~Graph();
        Graph(bool directed, bool weighted, bool labelled, bool valued);
        Node* insertNode(Node* node);
        Node* insertNode(int value);
        Node* insertNode(string label);
        Node* insertNode(int value, string label);
        void createEdge(Node* V1, Node* V2);
        void createEdge(Node* V1, Node* V2, float weight);
        void createEdge(string label1, string label2);
        void createEdge(string label1, string label2, float weight);
        inline bool isWeighted() {return weighted;}
        inline bool isDirected() {return directed;}
        inline bool isLabelled() {return labelled;}
        inline bool isValued() {return valued;}
        Node* searchNodeByLabel(string label);
        void printGraph();
        void createRandomGraph(int nVertices);
        void createRandomGraph(int nVertices, float density);
        int getNVertices() {return nVertices;}
        int getNEdge() {return nEdges;}
        Node* getNodeByIndex(int i);
    private:
        int nVertices;
        int nEdges;
        bool directed;
        bool weighted;
        bool labelled;
        bool valued;
        int degree[MAXV];
        Node *edgeNode[MAXV];
};

#endif

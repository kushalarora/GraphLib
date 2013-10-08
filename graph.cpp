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
 *      2. Adhacency List
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

using namespace std;
const int MAXV = 100000;
static bool DEBUG;

class Graph {
    public:
        class Node {
            string label;   // labels are unique
            int value;      // need not be unique
            int weight;
            Node* next;     // linked list of edges.
            bool operator ==(Node* node2);

            public:
            Node(int value, int weight, string label);

        };
        Graph();
        Graph(bool directed, bool weighted);
        void insertNode(Node* node);
        Node* insertNode(int value);
        Node* insertNode(int value, int weight);
        Node* insertNode(int value, string label);
        Node* insertNode(string label, int weight);
        Node* insertNode(string label);
        Node* insertNode(int value, int weight, string label);
        void createEdge(Node& V1, Node& V2);
        void createEdge(Node& V1, Node& V2, int weight);
        void createEdge(string label1, string label2);
        void createEdge(string label1, string label2, int weight);
    private:
        int nVertices;
        int nEdges;
        bool directed;
        bool weighted;
        int degree[MAXV];
        Node *edgeNode[MAXV];
};
Graph::Graph(bool dirctd, bool wghtd) {
    nVertices = 0;
    nEdges = 0;
    for (int i = 0; i < MAXV; i++) {
        degree[i] = 0;
        edgeNode[i] = NULL;
    }
    directed = dirctd;
    weighted = wghtd;
}
Graph::Graph() {
    Graph(false, false);
}

typedef Graph::Node Node;
Node::Node(int val, int wght, string lbl) {
    value = val;
    weight = wght;
    label = lbl;
    next = NULL;
}

bool Node::operator==(Node* node2) {
    if (this == node2)
        return true;
    if (this->label.length() > 0 && node2->label.length() > 0)
        return this->label == node2->label;
    return false;
};

void Graph::insertNode(Node* node) {
    int i;
    for (i = 0; i < nVertices; i++) {
        if (edgeNode[i] == node)
            return;
    }
    edgeNode[i] = node;
    nVertices += 1;
    degree[i] = 0;
}
Node* Graph::insertNode(int value) {
    insertNode(new Node(value, 0, NULL));
}

Node* Graph::insertNode(int value, int weight) {
    insertNode(new Node(value, weight, NULL));
}

Node* Graph::insertNode(int value, int weight, string label) {
    insertNode(new Node(value, weight, label));
}

Node* Graph::insertNode(int value, string label) {
    insertNode(new Node(value, 0, label));
}

Node* Graph::insertNode(string label, int weight) {
    insertNode(new Node(0, weight, label));
}

Node* Graph::insertNode(string label) {
    insertNode(new Node(0, 0, label));
}

int main(int argc, char *argv[]) {
    if (argc > 1)
        return -1;
    DEBUG = argc == 1 && argv[0][0] - '1' > 0;
    return 0;
}

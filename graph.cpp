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
            int getWeight() {return weight;}
            int getValue() {return value;}
            Node* getNext() {return next;}
            string getLabel() {return label;}
            void setWeight(int wght) {weight = wght;}
            void setNext(Node* nxt) {next = nxt;}

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
        void createEdge(Node* V1, Node* V2);
        void createEdge(Node* V1, Node* V2, int weight);
        void createEdge(string label1, string label2);
        void createEdge(string label1, string label2, int weight);
        inline bool isWeighted() {return weighted;}
        inline bool isDirected() {return directed;}
        Node* searchNodeByLabel(string label);
        void printGraph();
    private:
        int nVertices;
        int nEdges;
        bool directed;
        bool weighted;
        int degree[MAXV];
        Node *edgeNode[MAXV];
        void printNode(Node* node);
        void printEdge(Node* edgeNode);
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
    if (this == node2)  // if pointer matches, return true
        return true;
    if (this->label.length() > 0 && node2->label.length() > 0)  // if label is defined match that.
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
Node* Graph::searchNodeByLabel(string label) {
    if (label.length() == 0)
        return NULL;
    Node* node = NULL;
    for (int i = 0 ; i < nVertices; i++) {
        if (label == edgeNode[i]->getLabel())
            break;
    return node;
    }
}
void Graph::createEdge(Node *V1, Node *V2, int weight) {
    int i;
    for (i = -1; i < nVertices - 1; i++) {
        if (edgeNode[i + 1] == V1)
            break;
    }
    if (i > -1) {
        Node* temp = edgeNode[i + 1];
        while(temp->getNext() != NULL) {
            // if V2 already present do nothing.
            if (temp->getNext() == V2)
                return;
            temp = temp->getNext();
        }
        // insert node at the end.
        temp->setNext(V2);
        if (isWeighted())
            V2->setWeight(weight);

        degree[i + 1]++;
        nEdges++;
    }
}

void Graph::createEdge(Node *V1, Node* V2) {
    createEdge(V1, V2, 0);
}

void Graph::createEdge(string label1, string label2, int weight) {
    Node* node1 = searchNodeByLabel(label1);
    Node* node2 = searchNodeByLabel(label2);
    if (node1 == NULL || node2 == NULL)
        return;
    createEdge(node1, node2, weight);
}

void Graph::createEdge(string label1, string label2) {
    createEdge(label1, label2, 0);
}

void Graph::printGraph() {
    for (int i = 0; i < nVertices; i++) {
        printNode(edgeNode[i]);
        Node* tmp = edgeNode[i];
        while (tmp->getNext() != NULL) {
            printEdge(tmp->getNext());
            printNode(tmp->getNext());
            tmp = tmp->getNext();
        }
        cout<<"\n"<<"\n";
    }
}

void Graph::printNode(Node* node) {
    if (node == NULL)
        return;
    cout << "( ";
    if (node->getLabel().length() > 0)
        cout << node->getLabel() << " ";
     if (node->getValue() > 0)
         cout << node->getValue() << " ";
     cout << ")";
}

void Graph::printEdge(Node* node) {
    if (node == NULL)
        return;
    cout << isDirected() ? "<-" : "-";
    if (isWeighted())
        cout << node->getWeight();
    cout << "->";
}
int main(int argc, char *argv[]) {
    if (argc > 1)
        return -1;
    DEBUG = argc == 1 && argv[0][0] - '1' > 0;
    return 0;
}

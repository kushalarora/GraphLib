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
#include "../cpp/utils.hpp"
#include<assert.h>

using namespace std;
const int MAXV = 100000;
static bool DEBUG;

class Node {
    string label;   // labels are unique
    int value;      // need not be unique
    int weight;
    Node* next;     // linked list of edges.
    bool operator ==(Node* node2);
    int adj_index;

    public:
        Node() {}
        Node(int value, int weight, string label);
        Node(Node* node);
        int getValue() {return value;}
        void setValue(int val) {value = val;}
        Node* getNext() {return next;}
        void setNext(Node* nxt) {next = nxt;}
        int getWeight() {return weight;}
        void setWeight(int wght) {weight = wght;}
        string getLabel() {return label;}
        void setLabel(string lbl) {label = lbl;}
        void setAdjecencyIndex(int val) {adj_index = val;}
        int  getAdjecencyIndex() {return adj_index;}

};
class Graph {
    public:
        Graph();
        ~Graph();
        Graph(bool directed, bool weighted, bool labelled, bool valued);
        Node* insertNode(Node* node);
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
        inline bool isLabelled() {return labelled;}
        inline bool isValued() {return valued;}
        Node* searchNodeByLabel(string label);
        void printGraph();
        void createRandomGraph(int nVertices);
        void createRandomGraph(int nVertices, float density);
    private:
        int nVertices;
        int nEdges;
        bool directed;
        bool weighted;
        bool labelled;
        bool valued;
        int degree[MAXV];
        Node *edgeNode[MAXV];
        void printNode(Node* node);
        void printEdge(Node* edgeNode);
        string createRandomLabels(int nVertices);
};
Graph::Graph(bool dirctd, bool wghtd, bool lbled, bool valed) {
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
Graph::Graph() {
    Graph(false, false, false, false);
}

Node::Node(int val, int wght, string lbl) {
    value = val;
    weight = wght;
    label = lbl;
    next = NULL;
    adj_index = -1;
}

Node::Node(Node* node) {
    value = node->getValue();
    weight = node->getWeight();
    label = node->getLabel();
    next = NULL;
    adj_index = node->getAdjecencyIndex();
}

// nodes are equal if either they are same or have same label.
bool Node::operator==(Node* node2) {
    if (this == node2)  // if pointer matches, return true
        return true;
    if (this->label.length() > 0 && node2->label.length() > 0)  // if label is defined match that.
        return this->label == node2->label;
    return false;
};

Node* Graph::insertNode(Node* node) {
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
Node* Graph::insertNode(int value) {
    assert(isValued());
    return insertNode(new Node(value, 0, NULL));
}

Node* Graph::insertNode(int value, int weight) {
    assert(isValued() && isWeighted());
    return insertNode(new Node(value, weight, NULL));
}

Node* Graph::insertNode(int value, int weight, string label) {
    assert(isValued() && isWeighted() && isLabelled());
    return insertNode(new Node(value, weight, label));
}

Node* Graph::insertNode(int value, string label) {
    assert(isValued() && isLabelled());
    return insertNode(new Node(value, 0, label));
}

Node* Graph::insertNode(string label, int weight) {
    assert(isLabelled() && isValued());
    return insertNode(new Node(0, weight, label));
}

Node* Graph::insertNode(string label) {
    assert(isLabelled());
    return insertNode(new Node(0, 0, label));
}

Node* Graph::searchNodeByLabel(string label) {
    assert(isLabelled());
    if (label.length() > 0) {
        for (int i = 0 ; i < nVertices; i++) {
            if (label == edgeNode[i]->getLabel()) {
                return edgeNode[i];
            }
        }
    }
    return NULL;
}

void Graph::createEdge(Node *V1, Node *V2, int weight) {
    int i;
    // ensure that both nodes were inserted into graph.
    int idx1 = V1->getAdjecencyIndex();
    int idx2 = V2->getAdjecencyIndex();
    assert(idx1 != -1);
    assert(idx2 != -1);
    int idx;

    Node* temp;
    // inserting edge to v2 in v1
    temp = V1;
    idx = idx1;
    Node* newNode;
    while(temp->getNext() != NULL) {
        // if V2 already present do nothing.
        if (temp->getNext() == V2)
            return;
        temp = temp->getNext();
    }
    // insert node at the end.
    newNode = new Node(V2);
    temp->setNext(newNode);
    if (isWeighted())
        newNode->setWeight(weight);
    degree[idx]++;
    if (!isDirected()) {
        // if undirected, insert corresponding edge in v2.
        temp = V2;
        idx = idx2;
        while(temp->getNext() != NULL) {
            // if V1 already present do nothing.
            if (temp->getNext() == V1)
                return;
            temp = temp->getNext();
        }
        // insert node at the end.
        newNode = new Node(V1);
        temp->setNext(newNode);
        if (isWeighted())
            newNode->setWeight(weight);
        degree[idx]++;
    }
    nEdges++;
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
    if (isLabelled() && node->getLabel().length() > 0)
        cout << node->getLabel() << " ";
     if (isValued() && node->getValue() > 0)
         cout << node->getValue() << " ";
     cout << ")";
}

void Graph::printEdge(Node* node) {
    if (node == NULL)
        return;
    cout << (isDirected() ? "--" : "<--");
    if (isWeighted())
        cout << node->getWeight();
    cout << "-->";
}


string Graph::createRandomLabels(int nVertices) {
    int len = nVertices < 10 ? 2 : nVertices < 100 ? 4 : nVertices < 1000 ? 5 : 8;
    char sbuilder[len];
    for (int i = 0; i < len; i++) {
        sbuilder[i] = 'a' + rand() % 26;
    }
    string str = string(sbuilder, len);
    return str;
}

void Graph::createRandomGraph(int nVertices, float density) {
    srand(time(NULL));
    if (nVertices < 1)
        return;
    for (int i = 0; i < nVertices; i++) {
        Node* node = new Node();
        if (isLabelled())
            node->setLabel(createRandomLabels(nVertices));

        if (isValued())
            node->setValue(rand() % nVertices + 1);
        insertNode(node);
    }
    int nEdges = nVertices * (density > 0.0 ? density * nVertices : (rand() % nVertices));
    cout<<"\nMaking "<<nEdges<<" Edges\n";
    for (long i = 0; i < nEdges; i++) {
        int idx1 = rand() % nVertices;
        int idx2 = rand() % nVertices;

        // ensures that there are no self loops
        while (idx1 == idx2)
            idx2 = rand() % nVertices;
        createEdge(edgeNode[idx1], edgeNode[idx2], (isWeighted() ? rand() % 100 : 0));
        //printNode(edgeNode[idx1]);printEdge(edgeNode[idx2]);printNode(edgeNode[idx2]);
    }

}

void Graph::createRandomGraph(int nVertices) {
    createRandomGraph(nVertices, 0.0);
}

Graph::~Graph() {
    for (int i = 0; i < nVertices; i++) {
        Node* tmp = edgeNode[i];
        Node* tmp2;
        while(tmp != NULL) {
            tmp2 = tmp->getNext();
            delete tmp;
            tmp = tmp2;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc > 1)
        return -1;
    DEBUG = argc == 1 && argv[0][0] - '1' > 0;
    Graph g = Graph(false, true, true, true);
    g.createRandomGraph(10);
    g.printGraph();
    return 0;
}

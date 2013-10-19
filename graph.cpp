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
const int MAXV = 100000;
static bool DEBUG;

// forward declaration to resolve circular dependency.
class Edge;

class Node {
    string label;   // labels are unique
    int value;      // need not be unique
    int weight;
    Edge* edge_list;     // linked list of edges.
    bool operator ==(Node* node2);
    int adj_index;

    public:
        Node() {}
        Node(int value, int weight, string label);
        Node(Node* node);
        int getValue() {return value;}
        void setValue(int val) {value = val;}
        Edge* getEdgeList() {return edge_list;}
        Edge* setEdgeList(Edge* edge) {edge_list = edge;}
        int getWeight() {return weight;}
        void setWeight(int wght) {weight = wght;}
        string getLabel() {return label;}
        void setLabel(string lbl) {label = lbl;}
        void setAdjecencyIndex(int val) {adj_index = val;}
        int  getAdjecencyIndex() {return adj_index;}

};

Node::Node(int val, int wght, string lbl) {
    value = val;
    weight = wght;
    label = lbl;
    edge_list = NULL;
    adj_index = -1;
}

Node::Node(Node* node) {
    value = node->getValue();
    weight = node->getWeight();
    label = node->getLabel();
    edge_list = NULL;
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
};

Edge::Edge(Node* n1, Node* n2, bool is_directed, float weight) {
    node1 = n1;
    node2 = n2;
    this->is_directed = is_directed;
    this->weight = weight;
    this->next = NULL;
}

Edge::Edge(Node* n1, Node* n2) {
    Edge(n1, n2, false, DEFAULT_WEIGHT);
}

Edge::Edge(Node* n1, Node* n2, bool is_directed) {
    Edge(n1, n2, is_directed, DEFAULT_WEIGHT);
}

Edge::Edge(Node* n1, Node* n2, float weight) {
    Edge(n1, n2, false, weight);
}

bool Edge::operator ==(Edge* edge2) {
    if (edge2 == NULL)
        return false;

    if (this->getCurrentNode() == edge2->getCurrentNode() &&
            this->getOtherNode() == edge2->getOtherNode() &&
                (this->getWeight() == edge2->getWeight()))
            return true;
    return false;
}

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
        void printEdge(Edge* edge);
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

void Graph::createEdge(Node *V1, Node *V2, float weight) {
    Node* nodeArr[2] = {V1, V2};
    Edge* newEdge;
    Edge* temp;
    int i = 0, idx = 0;
    // ensure that both nodes were inserted into graph.
    assert(V1->getAdjecencyIndex() != -1);
    assert(V2->getAdjecencyIndex() != -1);

    for (i = 0; i < (isDirected() ? 1: 2); i++, degree[idx]++, idx = 1 - idx) {
        Node* currNode = nodeArr[idx];
        Node* othrNode = nodeArr[1 - idx];
        newEdge = new Edge(currNode, othrNode, isDirected(), weight);

        // inserting edge to v2 in v1
        temp = currNode->getEdgeList();
        Edge* prevNode = temp;
        while(temp != NULL) {
            // if V2 already present do nothing.
            if (temp == newEdge)
                return;
            prevNode = temp;
            temp = temp->getNext();
        }
        if (prevNode != NULL)
            prevNode->setNext(newEdge);
        else
            currNode->setEdgeList(newEdge);
    }
    nEdges++;
}

void Graph::createEdge(Node *V1, Node* V2) {
    createEdge(V1, V2, 0);
}

void Graph::createEdge(string label1, string label2, float weight) {
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
        Node* node = edgeNode[i];
        printNode(node);
        Edge* tmp = node->getEdgeList();
        while (tmp != NULL) {
            assert(tmp->getCurrentNode() == node);
            printEdge(tmp);
            printNode(tmp->getOtherNode());
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

void Graph::printEdge(Edge* edge) {
    if (edge == NULL)
        return;
    cout << (isDirected() ? "--" : "<--");
    if (isWeighted())
        cout << edge->getWeight();
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
        createEdge(edgeNode[idx1], edgeNode[idx2], (isWeighted() ? rand() % 100 : 0.0));
        //printNode(edgeNode[idx1]);printEdge(edgeNode[idx2]);printNode(edgeNode[idx2]);
    }

}

void Graph::createRandomGraph(int nVertices) {
    createRandomGraph(nVertices, 0.0);
}

Graph::~Graph() {
    for (int i = 0; i < nVertices; i++) {
        Node* tmp = edgeNode[i];
        Edge* tmp2 = tmp->getEdgeList();

        delete tmp;
        Edge *tmp3;
        while(tmp2 != NULL) {
            tmp3 = tmp2->getNext();
            delete tmp2;
            tmp2 = tmp3;
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

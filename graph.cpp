#include "graph.hpp"

Node::Node(int val, string lbl) {
    value = val;
    label = lbl;
    edge_list = NULL;
    adj_index = -1;
}

Node::Node(int val) {
    Node(val, NULL);
}
Node::Node(string lbl) {
    Node(-1, lbl);
}

Node::Node(Node* node) {
    value = node->getValue();
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
    return insertNode(new Node(value, NULL));
}

Node* Graph::insertNode(int value, string label) {
    assert(isValued() && isLabelled());
    return insertNode(new Node(value, label));
}

Node* Graph::insertNode(string label) {
    assert(isLabelled());
    return insertNode(new Node(0, label));
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
        node->printNode();
        Edge* tmp = node->getEdgeList();
        while (tmp != NULL) {
            assert(tmp->getCurrentNode() == node);
            tmp->printEdge();
            (tmp->getOtherNode())->printNode();
            tmp = tmp->getNext();
        }
        cout<<"\n"<<"\n";
    }
}

void Node::printNode() {
    cout << "( ";
    if (this->getLabel().length() > 0)
        cout << getLabel() << " ";
     if (getValue() > 0)
         cout << getValue() << " ";
     cout << ")";
}

void Edge::printEdge() {
    cout << (isDirected() ? "--" : "<--");
    if (getWeight() != DEFAULT_WEIGHT)
        cout << getWeight();
    cout << "-->";
}


string Node::createRandomLabels(int nVertices) {
    int len = nVertices < 10 ? 2 : nVertices < 100 ? 4 : nVertices < 1000 ? 5 : 8;
    char sbuilder[len];
    for (int i = 0; i < len; i++) {
        sbuilder[i] = 'a' + rand() % 26;
    }
    string str = string(sbuilder, len);
    return str;
}

Node* Node::populateNode(bool labelled, bool valued, int seed) {
    if (labelled)
        this->setLabel(createRandomLabels(seed));

    if (valued)
        this->setValue(rand() % seed + 1);
}

void Graph::createRandomGraph(int nVertices, float density) {
    srand(time(NULL));
    if (nVertices < 1)
        return;
    for (int i = 0; i < nVertices; i++) {
        insertNode((new Node())->populateNode(isLabelled(),
                                    isValued(),
                                    nVertices));
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

static int count = 0;
Graph::~Graph() {
    for (int i = 0; i < nVertices; i++) {
        Node* tmp = edgeNode[i];
        Edge* tmp2 = tmp->getEdgeList();
        Edge *tmp3;
        while(tmp2 != NULL) {
            tmp3 = tmp2->getNext();
            delete tmp2;
            tmp2 = tmp3;
        }
        delete tmp;
    }
}

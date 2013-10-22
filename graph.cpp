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

template<class T>
Edge<T>::Edge(T* n1, T* n2, bool is_directed, float weight) {
    node1 = n1;
    node2 = n2;
    this->is_directed = is_directed;
    this->weight = weight;
    this->next = NULL;
}

void Node::printNode() {
    cout << "( ";
    if (this->getLabel().length() > 0)
        cout << getLabel() << " ";
     if (getValue() > 0)
         cout << getValue() << " ";
     cout << ")";
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

template<class T>
void Edge<T>::printEdge() {
    cout << (isDirected() ? "--" : "<--");
    if (getWeight() != DEFAULT_WEIGHT)
        cout << getWeight();
    cout << "-->";
}

template<class T>
Edge<T>::Edge(T* n1, T* n2) {
    Edge<T>(n1, n2, false, DEFAULT_WEIGHT);
}

template<class T>
Edge<T>::Edge(T* n1, T* n2, bool is_directed) {
    Edge<T>(n1, n2, is_directed, DEFAULT_WEIGHT);
}

template<class T>
Edge<T>::Edge(T* n1, T* n2, float weight) {
    Edge<T>(n1, n2, false, weight);
}

template<class T>
bool Edge<T>::operator ==(Edge<T>* edge2) {
    if (edge2 == NULL)
        return false;

    if (this->getCurrentNode() == edge2->getCurrentNode() &&
            this->getOtherNode() == edge2->getOtherNode() &&
                (this->getWeight() == edge2->getWeight()))
            return true;
    return false;
}


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
T* Graph<T>::insertNode(T* node) {
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
void Graph<T>::createEdge(T *V1, T *V2, float weight) {
    T* nodeArr[2] = {V1, V2};
    Edge<T>* newEdge;
    Edge<T>* temp;
    int i = 0, idx = 0;
    // ensure that both nodes were inserted into graph.
    assert(V1->getAdjecencyIndex() != -1);
    assert(V2->getAdjecencyIndex() != -1);

    for (i = 0; i < (isDirected() ? 1: 2); i++, degree[idx]++, idx = 1 - idx) {
        T* currNode = nodeArr[idx];
        T* othrNode = nodeArr[1 - idx];
        newEdge = new Edge<T>(currNode, othrNode, isDirected(), weight);

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
void Graph<T>::createEdge(T *V1, T* V2) {
    createEdge(V1, V2, 0);
}

template<class T>
void Graph<T>::printGraph() {
    for (int i = 0; i < nVertices; i++) {
        T* node = edgeNode[i];
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
void Graph<T>::createRandomGraph(int nVertices, float density) {
    srand(time(NULL));
    if (nVertices < 1)
        return;
    for (int i = 0; i < nVertices; i++) {
        insertNode((new T())->populateNode(isLabelled(),
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

template<class T>
void Graph<T>::createRandomGraph(int nVertices) {
    createRandomGraph(nVertices, 0.0);
}

template<class T>
T* Graph<T>::getNodeByIndex(int i) {
    if (i < 0 || i >= nVertices)
        return NULL;
    return edgeNode[i];
}


template<class T>
Graph<T>::~Graph() {
    for (int i = 0; i < nVertices; i++) {
        T* tmp = edgeNode[i];
        Edge<T>* tmp2 = tmp->getEdgeList();
        Edge<T> *tmp3;
        while(tmp2 != NULL) {
            tmp3 = tmp2->getNext();
            delete tmp2;
            tmp2 = tmp3;
        }
        delete tmp;
    }
}

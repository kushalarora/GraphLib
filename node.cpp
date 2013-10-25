#include "node.hpp"
#include "edge.hpp"
#include<time.h>
#include<stdlib.h>

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

void Node::populateNode(bool labelled, bool valued, int seed) {
    if (labelled)
        this->setLabel(createRandomLabels(seed));

    if (valued)
        this->setValue(rand() % seed + 1);
}
Node::~Node() {
    Edge* edge = getEdgeList();
    Edge* tmp;
    while(edge != NULL) {
        tmp = edge->getNext();
        delete edge;
        edge = tmp;
    }
}

ostream& operator <<(ostream& os, const Node& node) {
    os << node.label;
}

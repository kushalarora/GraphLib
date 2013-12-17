#include "edge.hpp"
#include "node.hpp"
Edge::Edge(Node* n1, Node* n2, bool is_directed, float weight) {
    node1 = n1;
    node2 = n2;
    this->is_directed = is_directed;
    this->weight = weight;
    this->next = NULL;
}

void Edge::printEdge() {
    cout << (isDirected() ? "--" : "<--");
    if (getWeight() != DEFAULT_WEIGHT)
        cout << getWeight();
    cout << "-->";
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

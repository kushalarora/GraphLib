#include "MST.hpp"
#include<climits>

int MinimumSpanningTree::PRIMS(Graph<NodeMST> *G, NodeMST* source) {
    mQ->init(G);
    NodeMST* currVertex = source;
    currVertex->setDistance(0);
    int total_dist = 0;
    while(!currVertex->isInTree()) {
        currVertex->addToTree();
        total_dist += currVertex->getDistance();
        Edge* tmp = currVertex->getEdgeList();
        while(tmp != NULL) {
            assert(tmp->getCurrentNode() == currVertex);
            NodeMST* other = (NodeMST*)tmp->getOtherNode();
            int weight = tmp->getWeight();

            if ((other->getDistance() > weight) &&
                (!other->isInTree())) {
                    mQ->decreaseKey(other, weight);
                    other->setParent(currVertex);
                }
            tmp = tmp->getNext();
        }
        currVertex = mQ->getMinWeightNode();
    }
    return total_dist;
}


NodeMST* MinimumSpanningTree::simpleMinQueue(Graph<NodeMST>* G) {
    int distance = INT_MAX;
    NodeMST* smallest_node;
    for (int i = 0; i < G->getNVertices(); i++) {
        NodeMST* node = G->getNodeByIndex(i);
        if ((!node->isInTree()) &&
                distance > node->getDistance()) {
            distance = node->getDistance();
            smallest_node = node;
        }
    }
    return smallest_node;
}

void MinimumSpanningTree::printMSTEdges(Graph<NodeMST>* G) {
    NodeMST* parent;
    for (int i = 0; i < G->getNVertices(); i++) {
        NodeMST* node = G->getNodeByIndex(i);
        assert(node->isInTree());
        parent = node->getParent();
        if (parent == NULL)
            continue;
        Edge* tmp = parent->getEdgeList();
        while(tmp != NULL) {
            assert(tmp->getCurrentNode() == parent);
            if (tmp->getOtherNode() == node) {
                parent->printNode();
                tmp->printEdge();
                node->printNode();
                cout << endl;
                break;
            }
            tmp = tmp->getNext();
        }
    }
}
void SimpleMinQueueWrapper::decreaseKey(NodeMST* node, int weight) {
    if (weight < node->getDistance())
        node->setDistance(weight);
}
NodeMST* SimpleMinQueueWrapper::getMinWeightNode() {
    int distance = INT_MAX;
    NodeMST* smallest_node;
    for (int i = 0; i < g->getNVertices(); i++) {
        NodeMST* node = g->getNodeByIndex(i);
        if ((!node->isInTree()) &&
                distance > node->getDistance()) {
            distance = node->getDistance();
            smallest_node = node;
        }
    }
    return smallest_node;
}

void FHeapQueueWrapper::init(Graph<NodeMST>* G) {
    for (int i = 0; i < G->getNVertices(); i++) {
        heap.insertNode(G->getNodeByIndex(i), INT_MAX);
    }
}

void FHeapQueueWrapper::insertNode(NodeMST* node) {
    heap.insertNode(node, INT_MAX);
}

void FHeapQueueWrapper::decreaseKey(NodeMST* node, int weight) {
    if (node->getDistance() > weight){
        node->setDistance(weight);
        heap.decreaseKey(node, weight);
    }
}

NodeMST* FHeapQueueWrapper::getMinWeightNode() {
    return heap.extractMin();
}


#include "traversal.hpp"


void Traversal::BreadthFirstSearch(Graph<NodeT>* G, NodeT* source) {
    queue<NodeT*> q;
    source->setColor(NodeT::GRAY);
    Edge* edge;
    NodeT* node;
    NodeT* other;
    NodeT::COLOR clr;
    q.push(source);
    while(!q.empty()) {
        node = q.front();
        processOnGrey(node);
        assert(node != NULL);
        edge = node->getEdgeList();
        while(edge != NULL) {
            assert(edge->getCurrentNode() == node);
            other = (NodeT*)edge->getOtherNode();
            clr = other->getColor();
            if (clr == NodeT::WHITE) {
                processEdge(edge);
                other->setColor(NodeT::GRAY);
                other->setDist2Source(other->getDist2Source() + 1);
                other->setParent(node);
                q.push(other);
            }
            edge = edge->getNext();
        }
        q.pop();
        node->setColor(NodeT::BLACK);
        processOnBlack(node);
    }
}

void Traversal::DepthFirstRoutine(Graph<NodeT>* G, NodeT* source) {
}
void Traversal::DepthFirstSearch(Graph<NodeT>* G, NodeT* source) {

}



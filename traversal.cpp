#include "traversal.hpp"


void Traversal::BreadthFirstSearch(Graph* G, Node4BFS* source) {
    queue<Node4BFS*> q;
    processOnGrey(source);
    source->setColor(GRAY);
    Edge* edge;
    Node4BFS* node;
    Node4BFS* other;
    COLOR clr;
    q.push(source);
    while(!q.empty()) {
        node = q.front();
        assert(node != NULL);
        edge = node->getEdgeList();
        while(edge != NULL) {
            assert(edge->getCurrentNode() == node);
            other = (Node4BFS*)edge->getOtherNode();
            clr = other->getColor();
            if (clr == WHITE) {
                processOnGrey(other);
                other->setColor(GRAY);
                other->setDist2Source(other->getDist2Source() + 1);
                other->setParent(node);
                q.push(other);
            }
            edge = edge->getNext();
        }
        q.pop();
        node->setColor(BLACK);
        processOnBlack(node);
    }
}

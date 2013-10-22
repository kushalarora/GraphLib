#include "traversal.hpp"


void Traversal::BreadthFirstSearch(Graph<Node>* G, Node4BFS* source) {
    queue<Node4BFS*> q;
    source->setColor(Node4BFS::GRAY);
    Edge<Node>* edge;
    Node4BFS* node;
    Node4BFS* other;
    Node4BFS::COLOR clr;
    q.push(source);
    while(!q.empty()) {
        node = q.front();
        processOnGrey(node);
        assert(node != NULL);
        edge = node->getEdgeList();
        while(edge != NULL) {
            assert(edge->getCurrentNode() == node);
            other = (Node4BFS*)edge->getOtherNode();
            clr = other->getColor();
            if (clr == Node4BFS::WHITE) {
                processEdge(edge);
                other->setColor(Node4BFS::GRAY);
                other->setDist2Source(other->getDist2Source() + 1);
                other->setParent(node);
                q.push(other);
            }
            edge = edge->getNext();
        }
        q.pop();
        node->setColor(Node4BFS::BLACK);
        processOnBlack(node);
    }
}

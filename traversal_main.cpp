#include "traversal.hpp"

int main(int argc, char** argv) {
    Graph g = Graph(false, false, true, true);
    Node4BFS* n[10];
    for (int i = 0; i < 10; i++) {
        n[i] = new Node4BFS();
        n[i]->populateNode(true, true, 10);
        g.insertNode(n[i]);
    }
    for (int i = 1; i < 10; i++) {
        g.createEdge(n[0], n[i]);
    }
    g.printGraph();
    BreadthFirstSearch(&g, n[1]);
}

#include "connected_graph.hpp"
#include "node.hpp"

int main() {
    Graph<Node>* G = new ConnectedGraph<Node>(false, true, true, true);
    G->createRandomGraph(5);
    G->printGraph();
}

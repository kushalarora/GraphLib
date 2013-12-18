#include "../graph/connected_graph.hpp"

int main() {
    Graph<Node<int>, Edge<int> >* G = new ConnectedGraph<Node<int>, Edge<int> >(false, true, true, true);
    G->createRandomGraph(5);
    G->printGraph();
}

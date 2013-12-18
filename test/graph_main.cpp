#include "../graph/graph.hpp"
#include "../graph/node.hpp"

int main(int argc, char *argv[]) {
    if (argc > 1)
        return -1;
    Graph<Node<int>, Edge<int> > g = Graph<Node<int>, Edge<int> >(false, true, true, true);
    g.createRandomGraph(10);
    g.printGraph();
    return 0;
}

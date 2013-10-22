#include "graph.hpp"
#include "node.hpp"

int main(int argc, char *argv[]) {
    if (argc > 1)
        return -1;
    Graph<Node> g = Graph<Node>(false, true, true, true);
    g.createRandomGraph(10);
    g.printGraph();
    return 0;
}

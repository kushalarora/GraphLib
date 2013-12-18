#include "../graph/graph.hpp"
#include "../graph/node.hpp"

int main(int argc, char *argv[]) {
    if (argc > 1)
        return -1;
    Graph<int> g = Graph<int>(false, true, true, true);
    g.createRandomGraph(10);
    g.printGraph();
    return 0;
}

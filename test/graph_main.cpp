#include "../graph/graph.hpp"
#include "../graph/node.hpp"

int main(int argc, char *argv[]) {
    if (argc > 1)
        return -1;
    Graph<Node<int>, Edge<int> > g = Graph<Node<int>, Edge<int> >(true, true, true, true);
    g.createRandomGraph(5, 0.6, true);
    g.printGraph();

    cout << "The graph is " << (g.isCyclic() ? "Cyclic": "Acyclic") << endl;
    return 0;
}

#include <iostream>
#include "../graph/graph.hpp"

int main(int argc, char *argv[]) {
    if (argc > 1)
        return -1;
    Graph<int> g(true);
    Node<int>* nodeArr[5];

    for (int i = 0; i < 5; i++)
        nodeArr[i] = new Node<int>(i);
    g.createRandomGraph(5, nodeArr, 0.6, true);
    g.printGraph();

    cout << "The graph is " << (g.isCyclic() ? "Cyclic": "Acyclic") << endl;
    return 0;
}

/*
    Maximal set of vertices such that there is a path between every pair of vertices.

    Connected components are seperated pieces of graph such that there are no connections between vertices.

    Many problem can be reduced to connected component problem.
    Like rubic cube can be solved from any position or not. In this case current position is source and final position is solved cube.
*/

#include "traversal.hpp"
#include "graph/connected_graph.hpp"

int connected_component(Graph<NodeT>* g) {
    int c = 0;
    for (int i = 0; i < g->getNVertices(); i++) {
        NodeT* node = (NodeT*)g->getNodeByIndex(i);
        if (node->getColor() == NodeT::WHITE) {
            c++;
            cout << "Component No. " << c << "\n";
            Traversal().search(g, node, Traversal::BFS);
            cout << "\n" << "\n";
        }
    }

}

int main() {
   // Graph g = Graph(false, false, true, true);
   //  g.createRandomGraph(10);
   //
    ConnectedGraph<NodeT> g = ConnectedGraph<NodeT>(false, true, true, true);
    g.createRandomGraph(5);
    g.printGraph();
    cout << "\n" << "\n";
    connected_component(&g);
}

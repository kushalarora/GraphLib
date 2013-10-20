/*
    Maximal set of vertices such that there is a path between every pair of vertices.

    Connected components are seperated pieces of graph such that there are no connections between vertices.

    Many problem can be reduced to connected component problem.
    Like rubic cube can be solved from any position or not. In this case current position is source and final position is solved cube.
*/

#include "traversal.hpp"

int connected_component(Graph* g) {
    int c = 0;
    for (int i = 0; i < g->getNVertices(); i++) {
        Node4BFS* node = (Node4BFS*)g->getNodeByIndex(i);
        if (node->getColor() == WHITE) {
            c++;
            cout << "Component No. " << c << "\n";
            BreadthFirstSearch(g, node);
            cout << "\n" << "\n";
        }
    }

}

int main() {
    Graph g = Graph(false, false, true, true);
    Node4BFS* n[10];
    for (int i = 0; i < 10; i++) {
        n[i] = new Node4BFS();
        n[i]->populateNode(true, true, 10);
        g.insertNode(n[i]);
        }
    srand(time(NULL));
    int nEdges = 7; //* (rand() % 10);
    cout<<"nEdges "<< nEdges<<"\n";
    for (int i = 0; i < nEdges; i++) {
        g.createEdge(n[rand() % 10], n[rand() % 10]);
    }
    g.printGraph();
    cout << "\n" << "\n";
    connected_component(&g);
}

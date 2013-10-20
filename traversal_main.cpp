#include "traversal.hpp"

int main(int argc, char** argv) {
    Graph g = Graph(false, false, true, true);
    Node4BFS* n[10];
    for (int i = 0; i < 10; i++) {
        n[i] = new Node4BFS();
        n[i]->populateNode(true, true, 10);
        g.insertNode(n[i]);
        }
    srand(time(NULL));
    int nEdges = 10 * (rand() % 10);
    cout<<"nEdges "<< nEdges<<"\n";
    for (int i = 0; i < nEdges; i++) {
        g.createEdge(n[rand() % 10], n[rand() % 10]);
    }
    //g.printGraph();
    BreadthFirstSearch(&g, n[1]);
}

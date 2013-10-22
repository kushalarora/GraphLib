#include "traversal.hpp"

int main(int argc, char** argv) {
    Graph<NodeT> g = Graph<NodeT>(false, false, true, true);
/*    Node4BFS* n[10];
    for (int i = 0; i < 10; i++) {
        n[i] = new Node4BFS();
        n[i]->populateNode(true, true, 10);
        g.insertNode(n[i]);
        }
    srand(time(NULL));
    int nEdges = 10 * (rand() % 10);
    cout<<"nEdges "<< nEdges<<"\n";
    for (int i = 0; i < nEdges; i++) {
        Node* node1 = n[rand() % 10];
        Node* node2 = n[rand() % 10];
        while (node2 == node1)
            node2 = n[rand() % 10];

        g.createEdge(n[rand() % 10], n[rand() % 10]);
    }
    */
    g.createRandomGraph(10);
    g.printGraph();
    cout<<endl<<endl;
    Traversal().search(&g, g.getNodeByIndex(1), Traversal::BFS);
}

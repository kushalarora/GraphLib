#include "../graph/graph.hpp"

int main(int argc, char** argv) {
    Graph< Node<int>, Edge<int> > g = Graph< Node<int>, Edge<int> >(true, false, true, true);
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
    g.createRandomGraph(20, 0.20, true);
    g.printGraph();
    cout<<endl<<endl<<"BFS"<<endl;
    g.BreadthFirstSearch(g.getNodeByIndex(0));

    g.reset();
    cout<<endl<<endl<<"DFS"<<endl;
    //g.DepthFirstSearch();


    g.reset();
    g.topsort();
}

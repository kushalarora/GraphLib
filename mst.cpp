#include "MST.hpp"
#include "connected_graph.hpp"

int main() {
    Graph<NodeMST>* G = new ConnectedGraph<NodeMST>(false, true, true, true);
    G->createRandomGraph(1000);
    //G->printGraph();
    cout<<endl<<endl;
    MinimumSpanningTree* mst = MinimumSpanningTree::getInstance();
    cout << "Using Simple Scheme"<<endl;
    cout << "Total Spanned Weight "<< mst->spanMinimumTree(G, G->getNodeByIndex(0)) << endl;
    //mst->printMSTEdges(G);
    cout<<endl;
    mst->reset(G);
    cout << "Using FHeap Scheme"<<endl;
    mst->setScheme(FHEAP);
    cout << "Total Spanned Weight "<< mst->spanMinimumTree(G, G->getNodeByIndex(0)) << endl;
    mst->printMSTEdges(G);
}

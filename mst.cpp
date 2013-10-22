#include "MST.hpp"
#include "connected_graph.hpp"

int main() {
    Graph<NodeMST>* G = new ConnectedGraph<NodeMST>(false, true, true, true);
    G->createRandomGraph(5);
    G->printGraph();
    cout<<endl<<endl;
    
    cout << "Total Spanned Weight "<< MinimumSpanningTree::getInstance().getSpanningTree(G, G->getNodeByIndex(0), PRIMS) << endl;
    
    MinimumSpanningTree::getInstance().printMSTEdges(G);

}

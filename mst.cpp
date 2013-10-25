#include "MST.hpp"
#include "connected_graph.hpp"
#include<time.h>
#include<sstream>
#include<string>
#include<string.h>
#include<iostream>
#include<stdlib.h>


void randomComparisons(Graph<NodeMST>* G) {
    clock_t start, time;
    int totalWeight;

    // MST class is a singleton, getting instance
    MinimumSpanningTree* mst = MinimumSpanningTree::getInstance();

    cout << "Using Simple Scheme"<<endl;
    // scheme is set on runtime.
    mst->setScheme(SIMPLE);

    start = clock();
    totalWeight = mst->spanMinimumTree(G, G->getNodeByIndex(0));
    time = clock() - start;

    // print tree weight for cross checking and time taken.
    cout << "Total Spanned Weight "<< totalWeight << endl;
    cout << "Time taken: " << time;
    cout<<endl;

    // reset Graph to be used again.
    G->reset();


    cout << "Using FHeap Scheme"<<endl;
    start = clock();
    // set scheme to be FHEAP
    mst->setScheme(FHEAP);
    totalWeight = mst->spanMinimumTree(G, G->getNodeByIndex(0));
    time = clock() - start;
    cout << "Total Spanned Weight "<< totalWeight << endl;
    cout << "Time taken: " << time;
    cout<<endl;

    //mst->printMSTEdges(G);
}

int main(int argc, char* argv[]) {
    if (argc > 2) {
        if (strcmp(argv[1], "-r") == 0) {
            // if random mode, read nVertices and density.
            // read number of nodes
            int nNodes = atoi(argv[2]);

            cout << "Number of Nodes: " << nNodes<<endl;
            // read density of edges
            float density = atoi(argv[3])/100.0;
            cout << "Density of Edges: " << density<<endl;

            // create random graph
            Graph<NodeMST>* G = new ConnectedGraph<NodeMST>(false, true, true, true);
            G->createRandomGraph(nNodes, density);

        #ifdef DEBUG
            //G->printGraph();
            //cout<<endl<<endl;
        #endif
            // comparison function
            randomComparisons(G);
        }
    } else {
        // create random graph
        Graph<NodeMST>* G = new ConnectedGraph<NodeMST>(false, true, true, true);
        G->createRandomGraph(1000, 0.4);

        // comparison function
        randomComparisons(G);
    }
}

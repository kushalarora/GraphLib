#include "../MST.hpp"
#include "../graph/connected_graph.hpp"
#include<time.h>
#include<fstream>
#include<string>
#include<string.h>
#include<iostream>
#include<stdlib.h>

//#define DEBUG
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
#ifdef DEBUG
    mst->printMSTEdges(G);
#endif
}
void readUserInput(Graph<NodeMST>* G, char* inputfile) {
    ifstream ifs(inputfile, ifstream::in);

    if (!ifs) {
        cerr << "Error opening input file: " << inputfile << endl;
    }
    int nNodes, nEdges, val1, val2, weight;
    ifs >> nNodes;
    ifs >> nEdges;

    for (int i = 0; i < nNodes; i++) {
        G->insertNode(new NodeMST(i));
    }
    for (int i = 0; i < nEdges; i++) {
        assert(!ifs.eof());
        ifs >> val1;
        ifs >> val2;
        ifs >> weight;
        G->createEdge(G->getNodeByIndex(val1), G->getNodeByIndex(val2), weight);
    }
}

int main(int argc, char* argv[]) {
    Graph<NodeMST>* G = new ConnectedGraph<NodeMST>(false, true, true, true);
    if (argc > 1) {
        if (strcmp(argv[1], "-r") == 0) {
            if (argc == 4) {
                // if random mode, read nVertices and density.
                // read number of nodes
                int nNodes = atoi(argv[2]);

                cout << "Number of Nodes: " << nNodes<<endl;
                // read density of edges
                float density = atoi(argv[3])/100.0;
                cout << "Density of Edges: " << density<<endl;

                // create random graph
                G->createRandomGraph(nNodes, density);

            } else if (argc == 2) {
                // create random graph
                G->createRandomGraph(1000, 0.4);
            }
            #ifdef DEBUG
                G->printGraph();
                cout<<endl<<endl;
            #endif
            // comparison function
            randomComparisons(G);
        } else if(strcmp(argv[1], "-f") == 0 ||
                    strcmp(argv[1], "-s") == 0) {
            // run user input mode with Fibonacci Scheme
            readUserInput(G, argv[2]);

            // MST class is a singleton, getting instance
            MinimumSpanningTree* mst = MinimumSpanningTree::getInstance();

            if (strcmp(argv[1], "-f") == 0) {
                mst->setScheme(FHEAP);
            } else {
                mst->setScheme(SIMPLE);
            }
            cout<<mst->spanMinimumTree(G, G->getNodeByIndex(0))<<endl;
            mst->printMSTEdges(G);
        }
    }
}

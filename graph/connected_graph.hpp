/*
 * Using random walk strategy as defined here
 * http://stackoverflow.com/questions/2041517/random-simple-connected-graph-generation-with-given-sparseness/14618505#14618505
 */

#ifndef __CONNECTED_GRAPH__
#define __CONNECTED_GRAPH__
#include "graph.hpp"
#include<set>
template<class Node, class Edge>
class ConnectedGraph : public Graph<Node, Edge> {
    private:
        using Graph<Node, Edge>::createRandomEdges;
    public:
        // http://stackoverflow.com/questions/2812470/why-does-gcc-need-extra-declarations-in-templates-when-vs-does-not
        using Graph<Node, Edge>::isLabelled;
        using Graph<Node, Edge>::isValued;
        using Graph<Node, Edge>::isWeighted;
        using Graph<Node, Edge>::insertNode;
        using Graph<Node, Edge>::createEdge;
        using Graph<Node, Edge>::getNodeByIndex;
        ConnectedGraph():Graph<Node, Edge>() {};
        ConnectedGraph(bool directed, bool weighted, bool labelled, bool valued) : Graph<Node, Edge>(directed, weighted, labelled, valued) {};
        void createRandomGraph(int nVertices);
        void createRandomGraph(int nVertices, float density);
};


template<class Node, class Edge>
void ConnectedGraph<Node, Edge>::createRandomGraph(int nVertices, float density) {
    srand(time(NULL));
    if (nVertices < 1)
        return;
    set<Node*> s;
    for (int i = 0; i < nVertices; i++) {
        Node* node = new Node();
        node->populateNode(isLabelled(), isValued(), nVertices);
        insertNode(node);
        s.insert(node);
    }
    int connected_edges = 0;
    while (!s.empty()) {
        // get the first element in set and remove it from set.
        Node* node = *(s.begin());
        s.erase(node);

        Node* node2 = getNodeByIndex(rand() % nVertices);
        // keep looking for node until you find one from unvisited set.
        while((!s.empty()) && s.find(node2) == s.end()) {
            node2 = getNodeByIndex(rand() % nVertices);
        }
        createEdge(node, node2, (isWeighted() ? rand() % 100 + 1: 0.0));
        connected_edges++;
    }

    int nEdges = nVertices * (density > 0.0 ? density * nVertices : (rand() % nVertices));

    createRandomEdges((nEdges - connected_edges), nVertices);
};

template<class Node, class Edge>
void ConnectedGraph<Node, Edge>::createRandomGraph(int nVertices) {
    createRandomGraph(nVertices, 0.0);
}
#endif

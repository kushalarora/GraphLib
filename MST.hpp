#include<iostream>
using namespace std;

#include "graph.hpp"
#include<limits.h>

class NodeMST : public Node {
    private:
        bool in_tree;
        int distance;
        NodeMST* parent;
    public:
        NodeMST():Node() {
            in_tree = false;
            distance = INT_MAX;
            parent = NULL;
        }
        void addToTree() {in_tree = true;}
        bool isInTree() {return in_tree;}
        void setDistance(int distance) {this->distance = distance;}
        int getDistance() {return distance;}
        void setParent(NodeMST* parent) {this->parent = parent;}
        NodeMST* getParent() {return parent;}
};

        enum Algo {PRIMS, KRUSKAL};

class MinimumSpanningTree {
    private:
        MinimumSpanningTree(){};
        MinimumSpanningTree(MinimumSpanningTree const& copy);
        MinimumSpanningTree& operator =(MinimumSpanningTree const& copy);
        NodeMST* getVertexWithSmallestWeight(Graph<NodeMST>* G);
    public:
        static MinimumSpanningTree& getInstance() {
            static MinimumSpanningTree instance;
            return instance;
        }

        int getSpanningTree(Graph<NodeMST> *G, NodeMST* source, Algo algo) {
            PRIMS(G, source);
        }
        int PRIMS(Graph<NodeMST> *G, NodeMST* source);
        int KRUSKAL(Graph<NodeMST> *G, NodeMST* source);
        void printMSTEdges(Graph<NodeMST>* G);
};

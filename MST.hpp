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
class MinQueueWrapper {
    public:
        virtual void init(Graph<NodeMST>* G) = 0;
        virtual void insertNode(NodeMST* node) = 0;
        virtual NodeMST* getMinWeightNode() = 0;
        virtual void decreaseKey(NodeMST* node, int weight) = 0;
};

class SimpleMinQueueWrapper : public MinQueueWrapper{
    private:
        Graph<NodeMST> *g;
        int size;
    public:
        void init(Graph<NodeMST> *G) {
            g = G;
        }

        void insertNode(NodeMST* node) {
            // do nothing
        }

        void decreaseKey(NodeMST* node, int weight) {
            if (weight < node->getDistance())
                node->setDistance(weight);
        }

        NodeMST* getMinWeightNode() {
            int distance = INT_MAX;
            NodeMST* smallest_node;
            for (int i = 0; i < g->getNVertices(); i++) {
                NodeMST* node = g->getNodeByIndex(i);
                if ((!node->isInTree()) &&
                        distance > node->getDistance()) {
                    distance = node->getDistance();
                    smallest_node = node;
                }
            }
            return smallest_node;
        }
};



enum Algo {PRIM, KRUSKAL};
enum Scheme {SIMPLE, FHEAP};
class MinimumSpanningTree {
    private:
        MinQueueWrapper* mQ;
        Algo algo;
        Scheme scheme;
        MinimumSpanningTree(){
            algo = PRIM;
            scheme = SIMPLE;
            mQ = new SimpleMinQueueWrapper();
        };
        MinimumSpanningTree(MinimumSpanningTree const& copy);
        MinimumSpanningTree& operator =(MinimumSpanningTree const& copy);
        NodeMST* getVertexWithSmallestWeight(Graph<NodeMST>* G);
        NodeMST* simpleMinQueue(Graph<NodeMST>* G);
        int PRIMS(Graph<NodeMST> *G, NodeMST* source);
        int KRUSKAL(Graph<NodeMST> *G, NodeMST* source);
    public:
        static MinimumSpanningTree& getInstance() {
            static MinimumSpanningTree instance;
            return instance;
        }

        int spanMinimumTree(Graph<NodeMST> *G, NodeMST* source) {
            if (algo == PRIM)
                PRIMS(G, source);
        }
        void setAlgo(Algo algo) {
            this->algo = algo;
        }

        void setScheme(Scheme scheme) {
            if (scheme == SIMPLE)
                mQ = new SimpleMinQueueWrapper();
        }

        void printMSTEdges(Graph<NodeMST>* G);
};



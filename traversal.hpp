/*
 * The key idea is to mark each vertex when first visited and keep track of what we have not yet completely explored.
 *
 * Three States for each vertex:
 *      1. undiscovered -> not yet reached.
 *      2. discovered -> found but all incident edges not yet checked.
 *      3. processed -> visited all its incident edges
 *
 *  We should maintain a structure to keep in store all the vertices we have discovered but not yet processed.
 */

#include<iostream>
#include<queue>
#include "graph.hpp"
#include "node.hpp"
#include "edge.hpp"

/*
 * Breadth First Search
 *
 *
 */


class NodeT : public Node {
    public:
        enum COLOR {WHITE, GRAY, BLACK};
        COLOR getColor() {return color;}
        void setColor(COLOR color) {this->color = color;}
        void setParent(Node* node) {this->parent = node;}
        Node* getParent(Node* node) {return parent;}
        Node* getSource() {return source;}
        void setSource(Node* node) {this->source = node;}
        NodeT() {}
        NodeT(int value, string label):Node(value, label) {
            color = WHITE;
            parent = NULL;
            source = NULL;
            dist2s = -1;
        }
        NodeT(int value) {
            NodeT(value, NULL);
        }
        NodeT(string label) {
            NodeT(-1, label);
        }
        int getDist2Source(){return dist2s;}
        void setDist2Source(int dist) {this->dist2s = dist;}
    private:
        COLOR color;
        Node* parent;
        Node* source;
        int dist2s;
};

class Traversal {
    private:
        virtual void processEdge(Edge* edge) {
            cout << "Processed Edge";
            (edge->getCurrentNode())->printNode();
            edge->printEdge();
            (edge->getOtherNode())->printNode();
            cout << "\n";

        }
        virtual void processOnBlack(NodeT* node) {
            cout << "Node Turned Black ";
            node->printNode();
            cout << "\n";
        };
        virtual void processOnGrey(NodeT* node) {
            cout << "Node turned Grey ";
            node->printNode();
            cout << "\n";
        }
    public:
        enum SearchAlgo {BFS, DFS};
        void BreadthFirstSearch(Graph<NodeT>* G, NodeT* source);
        void search(Graph<NodeT>* G, NodeT* source, SearchAlgo algo) {
            if (algo == BFS) {
                BreadthFirstSearch(G, source);
            } else {
                // write DFS code
            }
        }
};


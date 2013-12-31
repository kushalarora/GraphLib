#include<iostream>
#include "../../test/ASSERT.hpp"
#include "../graph/graph.hpp"
#include<stdlib.h>
#include<time.h>

using namespace std;

class TBFSNode : public Node<int> {
    private:
        bool in_tree;
        void setInTree(bool in_tree) {this->in_tree = in_tree;}
    public:
        TBFSNode(int& val) : Node(val), in_tree(false){}
        TBFSNode(int& val, string label) : Node(val, label), in_tree(false){}
        TBFSNode(const TBFSNode& node) : Node(node), in_tree(false){}
        friend class TBFSGraph;
        bool inTree() {return in_tree;}
};



class TBFSGraph : public Graph<TBFSNode, Edge<int> > {
    virtual void processOnGrey(TBFSNode& node) {
        node.setInTree(true);
    }
};

class TestGraph {
    public:
        typedef Graph< Node<int>, Edge<int> > TGraph;
        typedef TGraph::RESET RESET;
    public:
        int MAX_TRIES;
        TestGraph() {
            MAX_TRIES = 6;
            srand(time(NULL));
        }

        void testEmpty() {
            TGraph *g = new Graph< Node<int>, Edge<int> >(true, true, true);
            TGraph g1(true, true, true);
            assert(g->isWeighted() == true);
            assert(g->isDirected() == true);
            assert(g->isLabelled() == true);

            ASSERT(*g == g1, "Empty graphs should be equal");

            TGraph* g2 = new Graph < Node<int>, Edge<int> >();
            TGraph g3;
            assert(g2->isWeighted() == false);
            assert(g2->isDirected() == false);
            assert(g2->isLabelled() == false);

            ASSERT(*g2 == g3, "Empty graphs should be equal");
            cout << "testEmpty Done!"<<endl;
        }

        void testInsertNode() {
            vector< Node<int> > nodeArr;
            for(int i = 0; i < 10; i++) {
                Node<int> node = Node<int>(i);
                node.populateNode(10);
                nodeArr.push_back(node);
            }

            TGraph *g = new Graph < Node<int>, Edge<int> >();
            for (int i = 0; i < nodeArr.size(); i++) {
                g->insertNode(nodeArr.at(i));
            }

            int size = nodeArr.size();
            int i = 0;

            ASSERT(g->getNVertices() == size, "Count of nodes wrong Size:" << size << ", NVertices:" << g->getNVertices());
            for (int i = 0; i < g->getNVertices(); i++) {
                ASSERT(g->getNodeByIndex(i)  == nodeArr[i], "Nodes should match Node");
            }

            cout << "testInsert Done!"<<endl;
        }
        void testCreateUndirectedEdge() {
            int val = 10, val1 = 20;
            Node<int> node1 = Node<int>(val);
            Node<int> node2 = Node<int>(val1);

            TGraph* g = new Graph < Node<int>, Edge<int> >();

            g->insertNode(node1);
            g->insertNode(node2);
            g->createEdge(node1, node2);

            ASSERT(g->getNEdge() == 1 , "Should have two edges");
            ASSERT(node1.getInDegree() == 1, "In Degree should have been 1, its " + node1.getInDegree());
            ASSERT(node1.getOutDegree() == 1, "In Degree should have been 1, its " + node1.getOutDegree());
            ASSERT(node2.getInDegree() == 1, "In Degree should have been 1, its " + node1.getInDegree());
            ASSERT(node2.getOutDegree() == 1, "In Degree should have been 1, its " + node1.getOutDegree());

            ASSERT(node1.getEdgeList() != NULL, "Node1 should have one edge");
            ASSERT(node2.getEdgeList() != NULL, "Node2 should have one edge");
            ASSERT(node1.getEdgeList()->getId() == node2.getEdgeList()->getId(), "Id of edge should match.");
            cout << "testCreateUndirectedEdge Done!"<<endl;
        }

/*
        void testCreateDirectedEdge() {
            int val = 10, val1 = 20;
            Node<int> node3 = Node<int>(val);
            Node<int> node4 = Node<int>(val1);
            TGraph* g = new Graph< Node<int>, Edge<int> >(true, false, false);
            g->insertNode(node3);
            g->insertNode(node4);
            g->createEdge(node3, node4);
            ASSERT(g->getNEdge() == 1, "Should have one edges");
            ASSERT(node3.getOutDegree() == 1, "In Degree should have been 1, its " << node3.getOutDegree());
            ASSERT(node4.getInDegree() == 1, "In Degree should have been 1, its " << node4.getInDegree());

            ASSERT(node3.getEdgeList() != NULL, "Node1 should have one edge");
            cout << "testCreateDirectedEdge Done!"<<endl;
        }

        void testRandomGraph() {
        }

        void testReset(RESET reset) {
            TGraph* g = new TGraph;
            TGraph g1;
            Node<int>* nodeArr[100];

            for (int i = 0; i < 100; i++) {
                nodeArr[i] = new Node<int>(i);
                nodeArr[i]->populateNode(true, 10);
            }

            g->createRandomGraph(100, 0.8, nodeArr);
            Graph< Node<int>, Edge<int> >  g2 = *g;
            g->reset(reset);
            if (reset == TGraph::HARD_RESET) {
                ASSERT(g->getNVertices() == 0, "There should be no vertices");
                ASSERT(g->getNEdge() == 0, "There should be no edges");
                ASSERT(g1 == *g, "Empty graph should match");

                cout << "testReset HARD_RESET Done!" << endl;
            } else {

            }
        }

        void testCreateTopsort() {
        }

        void testBFS() {
            TBFSGraph g;

            TBFSNode* nodeArr[100];

            for (int i = 0; i < 100; i++) {
                nodeArr[i] = new TBFSNode(i);
                nodeArr[i]->populateNode(true, 10);
            }

            g.createRandomGraph(100, 0.5, true, nodeArr, true);

            g.BreadthFirstSearch(*nodeArr[0]);

            for (int i = 0; i < g.getNVertices(); i++) {
                ASSERT(g.getNodeByIndex(i).inTree(), "All nodes should be in tree for BFS. i:" << i);
            }

            cout << "testBFS Done!"<< endl;

        }

        void testDFS() {
        }

        void testTransposeUndirected() {
            TGraph* g = new Graph< Node<int>, Edge<int> >(false, false, false);

            Node<int>* nodeArr[100];

            for (int i = 0; i < 100; i++) {
                nodeArr[i] = new Node<int>(i);
                nodeArr[i]->populateNode(true, 10);
            }

            g->createRandomGraph(100, 0.5, true, nodeArr);
            Graph< Node<int>, Edge<int> >  g2 = *g;
            g->transpose();
            ASSERT(*g == g2, "Transpose should be same for undirected graph");
            std::cout << "testTransposeUndirected Done!"<<endl;
        }

        void testTransposeDirected() {
            TGraph* g = new Graph< Node<int>, Edge<int> >(true, true, true);
            Node<int>* nodeArr[100];

            for (int i = 0; i < 100; i++) {
                nodeArr[i] = new Node<int>(i);
                nodeArr[i]->populateNode(true, 10);
            }

            g->createRandomGraph(100, 0.5, true, nodeArr);
            Graph< Node<int>, Edge<int> >  g2 = *g;

            g->transpose();
            g->transpose();

            ASSERT(*g == g2, "Transpose of transpose should be same");

            // Test tranpose
            std::cout << "testTransposeDirected Done!"<<endl;
        }

        */
};

int main() {
    TestGraph test;
    test.testEmpty();
    test.testInsertNode();
    test.testCreateUndirectedEdge();
    /*
    test.testCreateDirectedEdge();
    test.testTransposeUndirected();
    test.testTransposeDirected();
    test.testReset(TestGraph::TGraph::HARD_RESET);
    test.testBFS();
    */
    return 0;
}

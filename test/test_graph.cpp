#include<iostream>
#include "../../test/ASSERT.hpp"
#include "../graph/graph.hpp"
#include<stdlib.h>
#include<time.h>

using namespace std;

class TestGraph {
    public:
        typedef Graph< Node<int>, Edge<int> > TGraph;
        typedef TGraph::RESET RESET;
    private:
        TGraph *g;
    public:
        int MAX_TRIES;
        TestGraph() {
            MAX_TRIES = 6;
            srand(time(NULL));
        }

        void testEmpty() {
            g = new Graph< Node<int>, Edge<int> >(true, true, true);
            assert(g->isWeighted() == true);
            assert(g->isDirected() == true);
            assert(g->isLabelled() == true);

            g = new Graph < Node<int>, Edge<int> >();
            assert(g->isWeighted() == false);
            assert(g->isDirected() == false);
            assert(g->isLabelled() == false);

            cout << "testEmpty Done!"<<endl;
        }

        void testInsertNode() {
            vector< Node<int> > nodeArr;
            for(int i = 0; i < 10; i++) {
                Node<int> node = Node<int>(i);
                node.populateNode(true, 10);
                nodeArr.push_back(node);
            }

            g = new Graph < Node<int>, Edge<int> >();
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

            g = new Graph < Node<int>, Edge<int> >();

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

        void testCreateDirectedEdge() {
            int val = 10, val1 = 20;
            Node<int> node3 = Node<int>(val);
            Node<int> node4 = Node<int>(val1);
            g = new Graph< Node<int>, Edge<int> >(true, false, false);
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
            g = new Graph< Node<int>, Edge<int> >();
            Node<int>* nodeArr[5];

            for (int i = 0; i < 5; i++)
                nodeArr[i] = new Node<int>(i);
            g->createRandomGraph(5, 0.8, nodeArr);
            Graph< Node<int>, Edge<int> >  g2 = *g;
            g->reset(reset);
            if (reset == TGraph::HARD_RESET) {
                ASSERT(g->getNVertices() == 0, "There should be no vertices");
                ASSERT(g->getNEdge() == 0, "There should be no edges");
            } else {

            }
        }

        void testCreateTopsort() {
        }

        void testBFS() {
        }

        void testDFS() {
        }

        void testTranspose() {
            g = new Graph< Node<int>, Edge<int> >();
            Graph< Node<int>, Edge<int> > *g1 = new Graph< Node<int>, Edge<int> >(true, true, true);
            ASSERT(*g == *g1, "Empty graphs should be equal");

            Node<int>* nodeArr[5];

            for (int i = 0; i < 5; i++) {
                nodeArr[i] = new Node<int>(i);
                nodeArr[i]->populateNode(true, 10);
            }

            g->createRandomGraph(5, 0.5, true, nodeArr);

            Graph< Node<int>, Edge<int> >  g2 = *g;
            g->transpose();
            cout << "Hello World"<<endl;
            ASSERT(*g == g2, "Transpose should be same for undirected graph");

            g2.reset(TGraph::HARD_RESET);
            g1->createRandomGraph(5, 0.8, true, nodeArr);
            g2 = *g1;

/*
            g1->transpose();

            // Test tranpose
*/
            std::cout << "testTranspose Done!"<<endl;
        }
};

int main() {
    TestGraph test;
    test.testEmpty();
    test.testInsertNode();
    test.testCreateUndirectedEdge();
    test.testCreateDirectedEdge();
    test.testTranspose();
    return 0;
}

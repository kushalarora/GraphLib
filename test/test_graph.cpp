#include<iostream>
#include "../../test/ASSERT.hpp"
#include "../graph/graph.hpp"
#include<stdlib.h>
#include<time.h>

using namespace std;

class TestGraph {
    public:
        typedef Graph< Node<int>, Edge<int> > TGraph;
    private:
        TGraph g;
    public:
        int MAX_TRIES;
        TestGraph() {
            MAX_TRIES = 6;
            srand(time(NULL));
        }

        void testEmpty() {
            g = Graph< Node<int>, Edge<int> >(true, true, true);
            assert(g.isWeighted() == true);
            assert(g.isDirected() == true);
            assert(g.isLabelled() == true);

            g = Graph < Node<int>, Edge<int> >();
            assert(g.isWeighted() == false);
            assert(g.isDirected() == false);
            assert(g.isLabelled() == false);

            cout << "testEmpty Done!"<<endl;
        }

        int testInsertNode() {
            vector< Node<int> > nodeArr;
            for(int i = 0; i < 10; i++) {
                Node<int> node = Node<int>(i);
                node.populateNode(true, 10);
                nodeArr.push_back(node);
            }

            g = Graph < Node<int>, Edge<int> >();
            for (int i = 0; i < nodeArr.size(); i++) {
                g.insertNode(nodeArr.at(i));
            }

            int size = nodeArr.size();
            int i = 0;

            ASSERT(g.getNVertices() == size, "Count of nodes wrong Size:" << size << ", NVertices:" << g.getNVertices());
            for (int i = 0; i < g.getNVertices(); i++) {
                Node<int> node1 = g.getNodeByIndex(i);
                ASSERT(node1.getAdjecencyIndex()  == i, "Adjecency index wrong, Adj Indec" << node1.getAdjecencyIndex() << " i:" << i);
                i++;    // increase index;
            }

            cout << "testInsert Done!"<<endl;
        }

        void testCreateUndirectedEdge() {
            int val = 10, val1 = 20;
            Node<int> node1 = Node<int>(val);
            Node<int> node2 = Node<int>(val1);

            g = Graph < Node<int>, Edge<int> >();

            g.insertNode(node1);
            g.insertNode(node2);
            g.createEdge(node1, node2);

            ASSERT(g.getNEdge() == 1 , "Should have two edges");
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
            g = Graph< Node<int>, Edge<int> >(true, false, false);
            g.insertNode(node3);
            g.insertNode(node4);
            g.createEdge(node3, node4);
            ASSERT(g.getNEdge() == 1, "Should have one edges");
            ASSERT(node3.getOutDegree() == 1, "In Degree should have been 1, its " << node3.getOutDegree());
            ASSERT(node4.getInDegree() == 1, "In Degree should have been 1, its " << node4.getInDegree());

            ASSERT(node3.getEdgeList() != NULL, "Node1 should have one edge");
            cout << "testCreateDirectedEdge Done!"<<endl;
        }

        void testRandomGraph() {
        }
        void testReset() {
        }
        void testCreateTopsort() {
        }
        void testBFS() {
        }
        void testDFS() {
        }
        void testTranspose() {
            g = Graph< Node<int>, Edge<int> >();
            Graph< Node<int>, Edge<int> > g1 = Graph< Node<int>, Edge<int> >();
            ASSERT(g == g1, "Empty graphs should be equal");
        }
};

int main() {
    TestGraph test;

    test.testEmpty();
    test.testInsertNode();
    test.testCreateUndirectedEdge();
    test.testCreateDirectedEdge();
    test.testTranspose();
}

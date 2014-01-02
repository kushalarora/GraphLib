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
        TBFSNode(const TBFSNode& node) : Node(node), in_tree(node.in_tree){}
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

            ASSERT(g->getNodeCount() == size, "Count of nodes wrong Size:" << size << ", NVertices:" << g->getNodeCount());
            ASSERT(g->containsNode(nodeArr[i]), "Nodes should match Node");

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

            ASSERT(g->getEdgeCount() == 1 , "Should have two edges");
            ASSERT(g->getInDegreeForNode(node1) == 1, "In Degree should have been 1, its " + g->getInDegreeForNode(node1));
            ASSERT(g->getOutDegreeForNode(node2) == 1, "In Degree should have been 1, its " + g->getOutDegreeForNode(node2));
            ASSERT(g->getInDegreeForNode(node2) == 1, "In Degree should have been 1, its " + g->getInDegreeForNode(node2));
            ASSERT(g->getOutDegreeForNode(node2) == 1, "In Degree should have been 1, its " + g->getOutDegreeForNode(node2));

            vector< Edge<int> > edges1 = g->getOutEdgesForNode(node1);
            vector< Edge<int> > edges2 = g->getOutEdgesForNode(node2);
            ASSERT(edges1.size() == 1, "Node1 should have one edge");
            ASSERT(edges2.size() == 1, "Node1 should have one edge");
            ASSERT(*(edges1.begin()) == *(edges2.begin()), "Id of edge should match.");
            cout << "testCreateUndirectedEdge Done!"<<endl;
        }

        void testCreateDirectedEdge() {
            int val = 10, val1 = 20;
            Node<int> node3 = Node<int>(val);
            Node<int> node4 = Node<int>(val1);
            TGraph* g = new Graph< Node<int>, Edge<int> >(true, false, false);
            g->insertNode(node3);
            g->insertNode(node4);
            g->createEdge(node3, node4);
            ASSERT(g->getEdgeCount() == 1, "Should have one edges");
            ASSERT(g->getOutDegreeForNode(node3) == 1, "In Degree should have been 1, its " + g->getOutDegreeForNode(node3));

            ASSERT(g->getInDegreeForNode(node4) == 1, "In Degree should have been 1, its " + g->getInDegreeForNode(node4));

            vector< Edge<int> > edges = g->getOutEdgesForNode(node3);
            ASSERT(edges.size() == 1, "Node1 should have one edge");

            cout << "testCreateDirectedEdge Done!"<<endl;
        }

        void testRandomGraph() {
        }

        // TODO: Memory issues in reset.
        void testReset(RESET reset) {
            TGraph* g = new TGraph;
            TGraph *g1 = new TGraph();
            Node<int>* nodeArr[100];

            for (int i = 0; i < 100; i++) {
                nodeArr[i] = new Node<int>(i);
                nodeArr[i]->populateNode(true);

            }
            g->createRandomGraph(100, 0.8, nodeArr);
            Graph< Node<int>, Edge<int> >  g2 = *g;
            g->reset(reset);
            if (reset == TGraph::HARD_RESET) {
                ASSERT(g->getNodeCount() == 0, "There should be no vertices");
                ASSERT(g->getEdgeCount() == 0, "There should be no edges");
                ASSERT(*g1 == *g, "Empty graph should match");

                cout << "testReset HARD_RESET Done!" << endl;
            } else {

            }
        }

        void testTopsort() {

            Node<int>* nodeArr[100];

            for (int i = 0; i < 100; i++) {
                nodeArr[i] = new Node<int>(i);
                nodeArr[i]->populateNode(true);
            }


            TGraph *g = new Graph < Node<int>, Edge<int> >(true, true, true);

            g->createRandomGraph(100, 0.5, true, nodeArr);

            g->topsort();

            TGraph::iterator it;
            for (it = g->begin(); it != g->end() - 1; it++) {
                ASSERT(TGraph::compareExitTimeInc(*it, *(it + 1)), "Top sort should sort in inc order of exit time");
            }
            cout << "testTopsort Done!" << endl;
        }

        void testBFS() {
            TBFSGraph g;

            TBFSNode* nodeArr[100];

            for (int i = 0; i < 100; i++) {
                nodeArr[i] = new TBFSNode(i);
                nodeArr[i]->populateNode(true);
            }

            g.createRandomGraph(100, 0.5, true, nodeArr, true);

            g.BreadthFirstSearch(*nodeArr[0]);
            typename TBFSGraph::iterator it;
            for (it = g.begin(); it != g.end(); it++) {
                  ASSERT(it->inTree(), "All nodes should be in tree for BFS. i:" << it->getId());

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
                nodeArr[i]->populateNode(true);
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
                nodeArr[i]->populateNode(true);
            }

            g->createRandomGraph(100, 0.5, true, nodeArr);
            Graph< Node<int>, Edge<int> >  g2 = *g;

            g->transpose();
            g->transpose();

            ASSERT(*g == g2, "Transpose of transpose should be same");

            // Test tranpose
            std::cout << "testTransposeDirected Done!"<<endl;
        }

/*
        */
};

int main() {
    TestGraph test;
    test.testEmpty();
    test.testInsertNode();
    test.testCreateUndirectedEdge();
    test.testCreateDirectedEdge();
    test.testBFS();
    test.testTransposeUndirected();
    test.testReset(TestGraph::TGraph::HARD_RESET);
    test.testTransposeDirected();
    test.testTopsort();
    /*
    */
    return 0;
}

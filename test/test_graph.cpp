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



class TBFSGraph : public GraphBase<TBFSNode, Edge > {
    virtual void processOnGrey(TBFSNode& node) {
        node.setInTree(true);
    }
    public:
        TBFSGraph(bool is_directed) :
            GraphBase(is_directed) {};
        TBFSGraph():GraphBase() {};
};

class TestGraph {
    public:
        typedef Graph<int> TGraph;
        typedef TGraph::RESET RESET;
    public:
        int MAX_TRIES;
        TestGraph() {
            MAX_TRIES = 6;
            srand(time(NULL));
        }

        void testEmpty() {
            TGraph *g = new TGraph(true);
            TGraph g1(true);
            assert(g->isDirected() == true);

            ASSERT(*g == g1, "Empty graphs should be equal");

            TGraph* g2 = new TGraph;
            TGraph g3;
            assert(g2->isDirected() == false);

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

            TGraph *g = new TGraph;
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

            TGraph* g = new TGraph;

            g->insertNode(node1);
            g->insertNode(node2);
            g->createEdge(node1, node2);

            ASSERT(g->getEdgeCount() == 1 , "Should have two edges");
            ASSERT(g->getInDegreeForNode(node1) == 1, "In Degree should have been 1, its " + g->getInDegreeForNode(node1));
            ASSERT(g->getOutDegreeForNode(node2) == 1, "In Degree should have been 1, its " + g->getOutDegreeForNode(node2));
            ASSERT(g->getInDegreeForNode(node2) == 1, "In Degree should have been 1, its " + g->getInDegreeForNode(node2));
            ASSERT(g->getOutDegreeForNode(node2) == 1, "In Degree should have been 1, its " + g->getOutDegreeForNode(node2));

            vector< Edge > edges1 = g->getOutEdgesForNode(node1);
            vector< Edge > edges2 = g->getOutEdgesForNode(node2);
            ASSERT(edges1.size() == 1, "Node1 should have one edge");
            ASSERT(edges2.size() == 1, "Node1 should have one edge");
            ASSERT(*(edges1.begin()) == *(edges2.begin()), "Id of edge should match.");
            cout << "testCreateUndirectedEdge Done!"<<endl;
        }

        void testCreateDirectedEdge() {
            int val = 10, val1 = 20;
            Node<int> node3 = Node<int>(val);
            Node<int> node4 = Node<int>(val1);
            TGraph* g = new TGraph(true);
            g->insertNode(node3);
            g->insertNode(node4);
            g->createEdge(node3, node4);
            ASSERT(g->getEdgeCount() == 1, "Should have one edges");
            ASSERT(g->getOutDegreeForNode(node3) == 1, "In Degree should have been 1, its " + g->getOutDegreeForNode(node3));

            ASSERT(g->getInDegreeForNode(node4) == 1, "In Degree should have been 1, its " + g->getInDegreeForNode(node4));

            vector< Edge > edges = g->getOutEdgesForNode(node3);
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
            g->createRandomGraph(100, nodeArr, 0.8);
            TGraph  g2 = *g;
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


            TGraph *g = new TGraph(true);

            g->createRandomGraph(100, nodeArr);
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

            g.createRandomGraph(100, nodeArr, 0.5, true, true);

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
            TGraph* g = new TGraph(false);

            Node<int>* nodeArr[100];

            for (int i = 0; i < 100; i++) {
                nodeArr[i] = new Node<int>(i);
                nodeArr[i]->populateNode(true);
            }

            g->createRandomGraph(100, nodeArr);
            TGraph g2 = *g;
            g->transpose();
            ASSERT(*g == g2, "Transpose should be same for undirected graph");
            std::cout << "testTransposeUndirected Done!"<<endl;
        }

        void testTransposeDirected() {
            TGraph* g = new TGraph(true);
            Node<int>* nodeArr[6];

            for (int i = 0; i < 6; i++) {
                nodeArr[i] = new Node<int>(i);
                nodeArr[i]->populateNode(true);
            }

            g->createRandomGraph(6, nodeArr, 0.6, true, true);
            TGraph  g2 = *g;
            g->transpose();
            g->transpose();

            ASSERT(*g == g2, "Transpose of transpose should be same");

            // Test tranpose
            std::cout << "testTransposeDirected Done!"<<endl;
        }

        void testStronglyConnectedComponent() {

            TBFSGraph* g = new TBFSGraph(true);
            TBFSNode* nodeArr[100];

            for (int i = 0; i < 100; i++) {
                nodeArr[i] = new TBFSNode(i);
                nodeArr[i]->populateNode(true);
            }

            g->createRandomGraph(100, nodeArr);
            TBFSGraph  g2 = *g;
            TBFSGraph::ComponentGraph comp_graph =  g->stronglyConnectedComponents();


            // Iterate over components
            for (TBFSGraph::ComponentGraph::graph_iterator it = comp_graph.graph_begin(); it != comp_graph.graph_end(); it++) {
                // Check all nodes reachable from each other both
                // in normal node and transpose node.
                for(int i = 0; i < 2; i++) {
                    it->transpose();
                    // iterate over nodes of graph
                    for(TBFSGraph::iterator it1 = it->begin(); it1 != it->end(); it1++) {
                        // call bfs for each one of the nodes
                        it->BreadthFirstSearch(*it1);
                        TBFSGraph::iterator it2;
                        // see if all other nodes are in tree.
                        for (it2 = it1; it2 != it->end(); it2++) {
                            ASSERT(it2->inTree(), "All nodes should be in its tree");
                        }

                        // reset graph for next iteration
                        it->reset();
                    }
                }
            }
            cout << "testStronglyConnectedComponent Done!"<<endl;
        }
};

int main() {
    TestGraph test;
    test.testEmpty();
    test.testInsertNode();
    test.testCreateUndirectedEdge();
    test.testCreateDirectedEdge();
    test.testTransposeUndirected();
    test.testTransposeDirected();
    test.testReset(TestGraph::TGraph::HARD_RESET);
    test.testTopsort();
    test.testBFS();
    test.testStronglyConnectedComponent();
    return 0;
}

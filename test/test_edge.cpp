#include "../graph/edge.hpp"
#include "../graph/node.hpp"
#include "../../test/ASSERT.hpp"

    class TestEdge {
        public:
            void testSimpleEdge() {
                int val1 = 10;
                int val2 = 20;
                Node<int> node1 = Node<int>(val1);
                Node<int> node2 = Node<int>(val2);

                Edge<int> edge = Edge<int>(node1, node2);
                ASSERT(edge.getCurrentNode() == node1, "Current Node check fails");
                ASSERT(edge.getOtherNode() == node2, "Other node not same");
                ASSERT(edge.isDirected() == false, "Default directed value set as true");

                cout << "testSimpleEdge Done!" << endl;
            }

            void testDirectedEdge() {
                int val1 = 10;
                int val2 = 20;
                Node<int> node1 = Node<int>(val1);
                Node<int> node2 = Node<int>(val2);
                Edge<int> edge1 = Edge<int>(node1, node2, true);
                ASSERT(edge1.isDirected() == true, "Edge should have been directed");

                cout << "testDirectedEdge Done!" << endl;
            }

            void testWeightedDirectedEdge() {
                int val1 = 10;
                int val2 = 20;
                Node<int> node1 = Node<int>(val1);
                Node<int> node2 = Node<int>(val2);
                Edge<int> edge2 = Edge<int>(node1, node2, true, 10);
                ASSERT(edge2.getWeight() == 10, "Edge weight should have been 10");

                cout << "testWeightedDirectedEdge Done!" << endl;
            }
};

int main() {
    TestEdge testEdge;

    testEdge.testSimpleEdge();
    testEdge.testDirectedEdge();
    testEdge.testWeightedDirectedEdge();
}

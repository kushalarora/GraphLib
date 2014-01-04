#include "../../test/ASSERT.hpp"
#include "../graph/edge.hpp"

    class TestEdge {
        public:
            void testSimpleEdge() {
                int val1 = 10;
                int val2 = 20;

                Edge edge = Edge(val1, val2);
                ASSERT(edge.getCurrentNodeId() == val1, "Current Node check fails");
                ASSERT(edge.getOtherNodeId() == val2, "Other node not same");
                ASSERT(edge.isDirected() == false, "Default directed value set as true");

                cout << "testSimpleEdge Done!" << endl;
            }

            void testDirectedEdge() {
                int val1 = 10;
                int val2 = 20;
                Edge edge1 = Edge(val1, val2, true);
                ASSERT(edge1.isDirected() == true, "Edge should have been directed");

                cout << "testDirectedEdge Done!" << endl;
            }

            void testWeightedDirectedEdge() {
                int val1 = 10;
                int val2 = 20;
                Edge edge2 = Edge(val1, val2, true, 10);
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

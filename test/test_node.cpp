#include<iostream>
#include "../graph/node.hpp"
#include "../../test/ASSERT.hpp"

class NodeTest {
    public:
        void testNode() {
            int val = 10;
            Node<int> node = Node<int>(val);
            ASSERT(node.getLabel() == Node<int>::DEFAULT_LABEL, "Label Wrongly initialized");
            ASSERT(node.getValue() == val, "Value set wrong");
        }
};


class EdgeTest {
    public:
        void testEdge() {
            int val1 = 10;
            int val2 = 20;
            Node<int> node1 = Node<int>(val1);
            Node<int> node2 = Node<int>(val2);

            Edge<int> edge = Edge<int>(node1, node2);
            ASSERT(edge.getCurrentNode() == node1, "Current Node check fails");
            ASSERT(edge.getOtherNode() == node2, "Other node not same");
            ASSERT(edge.isDirected() == false, "Default directed value set as true");
            ASSERT(edge.getWeight() == Edge<int>::DEFAULT_WEIGHT, "Default weight wrongly set");
            ASSERT(edge.getType() == Edge<int>::NA, "Default edge type wrong");


            Edge<int> edge1 = Edge<int>(node1, node2, true);
            ASSERT(edge1.isDirected() == true, "Edge should have been directed");

            Edge<int> edge2 = Edge<int>(node1, node2, true, 10);
            ASSERT(edge2.getWeight() == 10, "Edge weight should have been 10");
        }
};

class GraphTest {
    void testGraph() {

    }
};

int main() {
    NodeTest().testNode();
    EdgeTest().testEdge();
}

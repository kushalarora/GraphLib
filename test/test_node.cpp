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




int main() {
    NodeTest test;
    test.testNode();
}

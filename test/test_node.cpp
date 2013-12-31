#include<iostream>
#include "../graph/node.hpp"
#include "../../test/ASSERT.hpp"

class TestNode {
    public:
        void testUnlabelledNode() {
            for (int i = 0; i < 10; i++) {
                int* val = new int(i);
                Node<int> node = Node<int>(*val);
                ASSERT(node.getId() == i, "Id should be same i:" << i << "Id: " << node.getId());
                ASSERT(node.getLabel() == Node<int>::DEFAULT_LABEL, "Label Wrongly initialized Label:" << node.getLabel());
                ASSERT(node.getValue() == *val, "Value set wrong val: " << *val << " Node Val: " << node.getValue());
            }
            cout << "testUnlabelledNode Done!" << endl;
        }

        void testLabelledNode() {
            for (int i = 0; i < 10; i++) {
                int* val = new int(i);
                string label = Node<int>::createRandomLabels(i);
                Node<int> node = Node<int>(*val, label);

                ASSERT(node.getId() == i + 10, "Id should be same i:" << i << "Id: " << node.getId());
                ASSERT(node.getLabel() == label, "Label Wrongly initialized Label: " << label << " Node Label: " << node.getLabel());
                ASSERT(node.getValue() == *val, "Value set wrong val: " << *val << " Node Val: " << node.getValue());
            }

            cout << "testLabelledNode Done!" << endl;
        }

        void testNodeCopy() {
            for (int i = 0; i < 10; i++) {
                int* val = new int(i);
                string label = Node<int>::createRandomLabels(i);
                Node<int> node = Node<int>(*val, label);

                Node<int> node2 = Node<int>(node);
                ASSERT(node == node2, "Copy Constructor should give same object");

                int val2 = 100;
                Node<int> node3 = Node<int>(val2);
                ASSERT(node != node3, "Different objects shouldn't match");

                node3 = node2;
                ASSERT(node == node3, "Assignment Operator should give same object");
            }
            cout << "testNodeCopy Done!" << endl;
        }
    };

int main() {
    TestNode testNode;
    testNode.testUnlabelledNode();
    testNode.testLabelledNode();
    testNode.testNodeCopy();

 //   EdgeTest().testEdge();
}

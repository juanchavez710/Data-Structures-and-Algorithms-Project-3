#include "utree.h"
#include <random>
#include <stdexcept>

class Tester {
public:
    bool testBasicUTreeInsert(UTree& dtree);
    bool testBasicUTreeRemove(UTree& dtree);
    bool removeNode(UTree& utree, UNode *& unode);

};


bool Tester::testBasicUTreeInsert(UTree& utree) {
    bool allInserted = true;
    
    utree.loadData("accounts.csv", 1);

    if (utree._root->_height != 3)
    { allInserted = false; }
    return allInserted;

}

bool Tester::testBasicUTreeRemove(UTree& utree) {
    bool allRemoved = true;
    int numDiscriminators = 3;
    int discriminators[] = {8232, 1286, 9884};
    DNode* removed = nullptr;

    for(int i = 0; i < numDiscriminators; i++) {
        if(!utree.removeUser("Kippage", discriminators[i], removed)) {
            cout << "Deletion on node " << discriminators[i] << " did not return true" << endl;
        }
    }

    cout << "Dumping the tree after 3 deletions time" << endl;
    utree.dump();
    cout << endl;

    cout << "Inserting 1 element to cause an imbalance and to clear vacant nodes" << endl;
    Account bigAcct = Account("A", 9999, 0, "", "");
    utree.insert(bigAcct);
    return allRemoved;
}


int main() {
    Tester tester;

    /* Basic dtree tests */
    UTree utree;

    utree.loadData("accounts.csv", 1);

    cout << "Testing UTree insertion...";
    if(tester.testBasicUTreeInsert(utree)) {
        cout << "test passed" << endl;
    } else {
        cout << "test failed" << endl;
    }

    cout << "Resulting UTree:" << endl;
    utree.dump();
    cout << endl;

    cout << "\n\nTesting UTree deletion..." << endl;
    if(tester.testBasicUTreeRemove(utree)) {
        cout << "test passed" << endl;
    } else {
        cout << "test failed" << endl;
    }

    return 0;
}

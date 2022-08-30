/**
 * Project 2 - Binary Trees
 * UserTree.h
 * Implementation for the UTree class.
 */

#include "utree.h"

/**
 * Destructor, deletes all dynamic memory.
 */
UTree::~UTree() {
    clear();
    _root = nullptr;
}

/**
 * Sources a .csv file to populate Account objects and insert them into the UTree.
 * @param infile path to .csv file containing database of accounts
 * @param append true to append to an existing tree structure or false to clear before importing
 */ 
void UTree::loadData(string infile, bool append) {
    std::ifstream instream(infile);
    string line;
    char delim = ',';
    const int numFields = 5;
    string fields[numFields];

    /* Check to make sure the file was opened */
    if(!instream.is_open()) {
        std::cerr << __FUNCTION__ << ": File " << infile << " could not be opened or located" << endl;
        exit(-1);
    }

    /* Should we append or clear? */
    if(!append) this->clear();

    /* Read in the data from the .csv file and insert into the UTree */
    while(std::getline(instream, line)) {
        std::stringstream buffer(line);

        /* Quick check to make sure each line is formatted correctly */
        int delimCount = 0;
        for(unsigned int c = 0; c < buffer.str().length(); c++) if(buffer.str()[c] == delim) delimCount++;
        if(delimCount != numFields - 1) {
            throw std::invalid_argument("Malformed input file detected - ensure each line contains 5 fields deliminated by a ','");
        }

        /* Populate the account attributes - 
         * Each line always has 5 sections of data */
        for(int i = 0; i < numFields; i++) {
            std::getline(buffer, line, delim);
            fields[i] = line;
        }
        Account newAcct = Account(fields[0], std::stoi(fields[1]), std::stoi(fields[2]), fields[3], fields[4]);
        this->insert(newAcct);
    }
}

/**
 * Dynamically allocates a new UNode in the tree and passes insertion into DTree. 
 * Should also update heights and detect imbalances in the traversal path after
 * an insertion.
 * @param newAcct Account object to be inserted into the corresponding DTree
 * @return true if the account was inserted, false otherwise
 */
bool UTree::insert(Account newAcct) {
    return insert(newAcct, _root);
}

/**
 * Removes a user with a matching username and discriminator.
 * @param username username to match
 * @param disc discriminator to match
 * @param removed DNode object to hold removed account
 * @return true if an account was removed, false otherwise
 */
bool UTree::removeUser(string username, int disc, DNode*& removed) {
    
    removed = retrieveUser(username, disc);

    if (removed == nullptr)
    { return false; }

    else
    {
        UNode* thisUNode = retrieve(username);

        if (thisUNode->getDTree()->remove(disc, removed))
        { 
            if (thisUNode->getDTree()->getNumUsers() == 0)
            { removeUNode(thisUNode); }
            return true; 
        }

        else
        { return false; }
    }
}

/**
 * Retrieves a set of users within a UNode.
 * @param username username to match
 * @return UNode with a matching username, nullptr otherwise
 */
UNode* UTree::retrieve(string username) {
    return retrieve(username, _root);
}

/**
 * Retrieves the specified Account within a DNode.
 * @param username username to match
 * @param disc discriminator to match
 * @return DNode with a matching username and discriminator, nullptr otherwise
 */
DNode* UTree::retrieveUser(string username, int disc) {
    UNode * thisUNode = retrieve(username);
    
    return thisUNode->getDTree()->retrieve(disc);
}

/**
 * Returns the number of users with a specific username.
 * @param username username to match
 * @return number of users with the specified username
 */
int UTree::numUsers(string username) {
    UNode * users = retrieve(username);
    
    return users->getDTree()->getNumUsers();
}

/**
 * Helper for the destructor to clear dynamic memory.
 */
void UTree::clear() {
    makeEmpty(_root);
}

/**
 * Prints all accounts' details within every DTree.
 */
void UTree::printUsers() const {
    printUsers(_root);
}

/**
 * Dumps the UTree in the '()' notation.
 */
void UTree::dump(UNode* node) const {
    if(node == nullptr) return;
    cout << "(";
    dump(node->_left);
    cout << node->getUsername() << ":" << node->getHeight() << ":" << node->getDTree()->getNumUsers();
    dump(node->_right);
    cout << ")";
}

/**
 * Updates the height of the specified node.
 * @param node UNode object in which the height will be updated
 */
void UTree::updateHeight(UNode* node) {
    if (node == nullptr)
    { return; }

    if (node->_left != nullptr)
    { updateHeight(node->_left); }
    if (node->_right != nullptr)
    { updateHeight(node->_right); }

    if (node->_left == nullptr && node->_right == nullptr)
    { node->_height = DEFAULT_HEIGHT; return; }

    else
    { 
        if (node->_left == nullptr)
        { node->_height = node->_right->getHeight() + 1; }
        else if(node->_right == nullptr)
        { node->_height = node->_left->getHeight() + 1; }
        else if (node->_right->getHeight() > node->_left->getHeight())
        { node->_height = node->_right->getHeight() + 1; }

        else 
        { node->_height = node->_left->getHeight() + 1; }
    }
}

/**
 * Checks for an imbalance, defined by AVL rules, at the specified node.
 * @param node UNode object to inspect for an imbalance
 * @return (can change) returns true if an imbalance occured, false otherwise
 */
int UTree::checkImbalance(UNode* node) {
    if ( node == nullptr)
    { return false; }

    if (node->_height == 0 || node->_height == 1)
    { return false; }
    
    if (node->_right == nullptr && node->_left->_height > 0)
    { return true; }
    else if (node->_left == nullptr && node->_right->_height > 0)
    { return true; }
    else if ( abs(node->_left->_height - node->_right->_height) > 1 )
    { return true; }

    else { return false; }
}

//----------------
/**
 * Begins and manages the rebalance procedure for an AVL tree (pass by reference).
 * @param node UNode object where an imbalance occurred
 */
void UTree::rebalance(UNode*& node) {

    if (node == nullptr)
    { return; }

    int balance;
    if ( node->_left == nullptr && node->_right->getHeight() >= 1 )
    { balance = -1 * node->_right->getHeight() - 1; }
    else if ( node->_right == nullptr && node->_left->getHeight() >= 1 )
    { balance = node->_left->getHeight() + 1; }
    else{ balance = node->_left->_height - node->_right->_height; }
 
    // If this node becomes unbalanced, then
    // there are 4 cases
 
    // right rotation
    if (balance > 1 && node->_left->_left != nullptr)
        rightRotate(node);
 
    // left rotation
    else if (balance < -1 && node->_right->_right != nullptr)
        leftRotate(node);
 
    // double left right rotation
    else if (balance > 1 && node->_left->_right != nullptr)
    {
        leftRotate(node->_left);
        rightRotate(node);
    }
 
    // double left right rotation
    else if (balance < -1 && node->_right->_left != nullptr)
    {
        rightRotate(node->_right);
        leftRotate(node);
    }

    updateHeight(node);

// -- OR --
}

/**
 * Begins and manages the rebalance procedure for an AVL tree (returns a pointer).
 * @param node UNode object where an imbalance occurred
 * @return UNode object replacing the unbalanced node's position in the tree
 */
//UNode* UTree::rebalance(UNode* node) {

//}
//----------------


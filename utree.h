/**
 * Project 2 - Binary Trees
 * UserTree.h
 * An interface for the UTree class.
 */

#pragma once

#include "dtree.h"
#include <fstream>
#include <sstream>

#define DEFAULT_HEIGHT 0

class Grader;   /* For grading purposes */
class Tester;   /* Forward declaration for testing class */

class UNode {
    friend class Grader;
    friend class Tester;
    friend class UTree;
public:
    UNode() {
        _dtree = new DTree();
        _height = DEFAULT_HEIGHT;
        _left = nullptr;
        _right = nullptr;
    }

    ~UNode() {
        delete _dtree;
        _dtree = nullptr;
    }

    /* Getters */
    DTree*& getDTree() {return _dtree;}
    int getHeight() const {return _height;}
    string getUsername() const {return _dtree->getUsername();}

private:
    DTree* _dtree;
    int _height;
    UNode* _left;
    UNode* _right;

    /* IMPLEMENT (optional): Additional helper functions */
    // void copyDTree(DTree *& dest, DTree * src)
    // {
    //     dest = src;
    // }

};

// UNode& UNode::operator=(const UNode& rhs) {
//     if(this != &rhs) {
//         delete _dtree;
//         _dtree = nullptr;
//         copyDTree(_dtree, rhs._dtree);
//         this->_right = rhs._right;
//         this->_left = rhs._left;
//         this->_height = rhs._height;
//     }
//     return *this;
// }

class UTree {
    friend class Grader;
    friend class Tester;

public:
    UTree():_root(nullptr){}

    /* IMPLEMENT: destructor */
    ~UTree();

    /* IMPLEMENT: Basic operations */

    void loadData(string infile, bool append = true);
    bool insert(Account newAcct);
    bool removeUser(string username, int disc, DNode*& removed);
    UNode* retrieve(string username);
    DNode* retrieveUser(string username, int disc);
    int numUsers(string username);
    void clear();
    void printUsers() const;
    void dump() const {dump(_root);}
    void dump(UNode* node) const;


    /* IMPLEMENT: "Helper" functions */
    
    void updateHeight(UNode* node);
    int checkImbalance(UNode* node);
    //----------------
    void rebalance(UNode*& node);
    // -- OR --
    //UNode* rebalance(UNode* node);
    //----------------

private:
    UNode* _root;

    /* IMPLEMENT (optional): any additional helper functions here! */

    /* helper function for insert*/
    int max(int a, int b)
    { return (a > b) ? a : b; }

    bool insert(Account newAcct, UNode*& node) {

        bool inserted = false;

        // if node is a leaf, insert into Dtree and rebalance if necessary
        if (node == nullptr) 
        {
            node = new UNode();
            node->getDTree()->insert(newAcct);
            
            return true;
        }

        // if it is not a leaf, traverse through tree using BST rules and find a matching username
        if (newAcct.getUsername() < node->getUsername()) 
        { inserted = insert(newAcct, node->_left); } 
        else if (newAcct.getUsername() > node->getUsername()) 
        { inserted = insert(newAcct, node->_right); } 
        else // matching username, insert and if the discriminator is the same it will return false
        { inserted = node->getDTree()->insert(newAcct); }

        updateHeight(node);

        //check imbalance
        if (checkImbalance(node)) 
        { rebalance(node); }

        return inserted;
    }

    /*retrieve helper function*/
    UNode* retrieve( const string username, UNode* t ) const
    {
		if ( t == nullptr ) { return nullptr; }
		else if( username < t->getDTree()->getUsername() ) 
        {
            return retrieve( username, t->_left ); 
        }
		else if( username > t->getDTree()->getUsername() ) 
        { 
            return retrieve( username, t->_right );
        } 
		else // Match
        {
            return t;
        }
    }
    
    /*destructor helper function*/
    void makeEmpty( UNode * & t )
    {
        if( t == nullptr ) return;
        makeEmpty( t->_left );
        makeEmpty( t->_right );
        //delete t->getDTree();
        delete t;
        t = nullptr;
    }

    /*helper function for printUsers*/
    void printUsers(UNode* node) const {
        if(node == nullptr) return;
        printUsers(node->_left);
        node->getDTree()->printAccounts();
        printUsers(node->_right);
    }

    void leftRotate(UNode *& x)
    {   
        UNode *z = x;
        UNode *y = x->_right;
        UNode *rightleft = y->_left;
    
        // Perform rotation
        x = y;
        x->_left = z;
        x->_left->_right = rightleft;
    
        // Update heights
        updateHeight(x);
        updateHeight(y);
    }

    void rightRotate(UNode *& y)
    {
        UNode *z = y;
        UNode *x = y->_left;
        UNode *leftright = x->_right;
    
        // Perform rotation
        y = x;
        y->_right = z;
        y->_right->_left = leftright;
    
        // Update heights
        updateHeight(x);
        updateHeight(y);
    }

    /* function to find max node for removing UNode */
    UNode * findMaxNode(UNode*& node)
    {
        if (node->_right == nullptr)
        { 
            UNode* newNode = node;
            if (node->_left)
            { node = node->_left; 
              delete node->_left; }
            else
            { delete node; }

            updateHeight(node);
            
            return newNode;
        }
        
        else 
        {
            findMaxNode(node->_right);

            updateHeight(node);

            if(checkImbalance(node->_right))
            { rebalance(node->_right); }
        }
        return nullptr;
    }

    /* Function for removing a UNode (doesn't work) */
    void removeUNode(UNode *& node)
    {
        if (node->_left != nullptr)
        {
            UNode* maxNode = findMaxNode(node->_left);

            node->getDTree() = maxNode->getDTree();
        }

        else 
        {
            UNode * rightnode = node->_right;
            node = rightnode;

            node = nullptr;

            updateHeight(node);
        }
    }
};

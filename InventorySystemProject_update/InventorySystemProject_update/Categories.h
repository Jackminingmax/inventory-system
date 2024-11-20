// Jack Maxwell
// 11/20/2024
// Categories held by an AVL tree. Items for categories are held in a dynamic array.


#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <ctime>
using namespace std;

const static int capacityMax = 11;
const static int capacity[capacityMax] = { 50, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000 };

class AVL
{
private:

    struct Node
    {
        int tableSize;          //capacity of array
        string identifier;
        string idData;

        string* itemData;       // Pointer to dynamically allocated array for items
        int numOfItems;         // Number of items currently in the array

        int height;
        Node* left;
        Node* right;

        Node(string& theIdentifier, string& theData) : identifier(theIdentifier), idData(theData), tableSize(capacity[0]), numOfItems(0), height(0), left(nullptr), right(nullptr) { itemData = new string[tableSize]; }
    };

    Node* root;

public:
    // Constructor
    AVL() : root(nullptr) { }

    // Copy constructor
    AVL(const AVL& orig) { copy(orig); }

    // Destructor
    ~AVL() { clear(); }

    // Assignment operator
    AVL& operator=(const AVL& rhs);

    // Check to see if the AVl tree isEmpty
    bool isEmpty() const { return root == nullptr; }

    // Add a category to the AVL tree
    void addCategory(string ident, string anEntry);

    // Determines whether the string is in the dictionary. True if yes, false otherwise.
    bool findIfCategory(const string key) const;

    // Print categories in order.
    void printCategoriesInOrder(ostream& outputStream) const;

    // Insert an item into the category, given the category, position in array to be inserted, and description.
    void insertItem(string categ, int num, string desc);

    // Goes through and prints every item in the array for that category.
    void printItemsInCategory(string categ, ostream& outputStream);

    // Outputs a single item
    void printSingleItem(string categ, int num, ostream& outputStream);

    // Prints every item in every category, in order by category abbreviation.
    void printEveryItem(ostream& outputStream);

    // Adds a new item to the category, generating and returning a SKU number. Also adds the date.
    int addItem(string categ, string desc);

    // Removes an item from the category, returning the item removed.
    string removeItem(string categ, int num);

    // Removes a category from the AVL tree
    void removeCategory(string categ);

    // Prints out the structure of the AVL tree with indented format.
    void printStructure(ostream& outputStream);

private:
    // clear helper method for copy constructor and assignment operator
    void clear() { clear(root); }

    // recursive clear helper
    void clear(Node* node);

    // copy helper method for destructor and assignment operator
    void copy(const AVL& orig) { this->root = copy(orig.root); }

    // recursive copy helper
    Node* copy(Node* origNode);

    // recursive helper method for insertion
    void insert(string ident, string value, Node*& curPtr);

    // recursive helper for printDictionaryInOrder
    void printInOrder(ostream& outputStream, Node* curNode) const;

    // tree printer helper -- recursive function to print the tree structure
    void printTree(ostream& outputStream, Node* curNode, int depth) const;

    // performs a single right rotation on the given node
    Node* singleRightRotation(Node* curNode);

    // performs a single left rotation on the given node
    Node* singleLeftRotation(Node* curNode);

    // assigns the height to each node.
    int height(Node* curNode);

    // helper function for printing an item
    void printItemHelper(Node* curNode, ostream& outputStream);

    // called from removeCategory to delete the category.
    void deleteNode(Node*& curNode, string categ);

    // helper function that retrieves the left-most node of a given node, replacing it with its right child and returning the pointer to it.
    Node* retrieveLeast(Node* givenNode, Node* parentNode);

    // balanceTree is a helper function that determines if the node is unbalanced and uses rotations to try and balance the tree.
    void balanceTree(Node*& curPtr);

    // finds and returns a category given the category abbreviation (identifier)
    Node* findCategory(const string& categ) const;
};

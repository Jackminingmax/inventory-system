// Jack Maxwell
// 11/20/2024
// Implementation for the Categories Class.
// Stores categories in an AVL tree


#include "Categories.h"
using namespace std;

// Assignment operator
AVL& AVL::operator=(const AVL& rhs)
{
    // Don't self-assign, make sure the two root nodes are different.
    if (this->root != rhs.root)
    {
        clear();    // delete the left hand sides tree
        copy(rhs);  // deep copy right hand sides tree into this
    }

    return *this;
}


// Add a category to the AVL tree
void AVL::addCategory(string ident, string anEntry)
{
    // call private recursive insert method
    insert(ident, anEntry, root);
}

// Determines whether the string is in the dictionary. True if yes, false otherwise.
bool AVL::findIfCategory(string categ) const
{
    // start from the root
    Node* curNode = findCategory(categ);
    return curNode != nullptr;
}

// Print categories in order.
void AVL::printCategoriesInOrder(ostream& outputStream) const
{
    // call helper function starting from the root to go through and recursively output the categories in order.
    if (root) {
        printInOrder(outputStream, root);
    }
}


// Assumption: num is 3 digits max.
// Insert an item into the given category.
void AVL::insertItem(string categ, int num, string desc)
{
    // search for the category in the tree, starting at the root, and comparing the categories identifiers (abbreviations).
    Node* curNode = findCategory(categ);

    // check if the category was found, complain otherwise.
    if (curNode) {
        // check if the category has enough space for the new item, if not then resize based on the current item number.
        if (num >= curNode->tableSize) {
            int newTableSize = 0;
            int i = 1;
            // increase size until it either can accomodate the item number or the size reaches 1000.
            while (newTableSize < num && i < capacityMax) {
                newTableSize = capacity[i];
                ++i;
            }

            // create a new array with the new size and copy all the old items in.
            string* newArray = new string[newTableSize];
            for (int j = 0; j < curNode->tableSize; ++j) {
                newArray[j] = curNode->itemData[j];
            }

            // delete old array and assign it to the new one.
            delete[] curNode->itemData;
            curNode->itemData = newArray;
            curNode->tableSize = newTableSize;
        }
        // make same check again. If the item still exceeds capacity, complain.
        if (num >= curNode->tableSize) {
            cout << "Error- max capacity reached. Number bigger than 999 inserted. Skipping." << endl;
        }
        else {

            // If the spot is empty, insert it in and update the numOfItems variable. Otherwise complain.
            if (curNode->itemData[num].empty()) {
                curNode->itemData[num] = desc;
                curNode->numOfItems = curNode->numOfItems + 1;
            }
            else {
                cout << "Error inserting item: This spot is already taken." << endl;
            }
        }
    }
    else {
        cout << "Error inserting item: Could not find category" << endl;
    }
}

// print all items in a specific category
void AVL::printItemsInCategory(string categ, ostream& outputStream)
{
    // find the category
    Node* curNode = findCategory(categ);

    // check if the category was found, complain otherwise.
    if (curNode) {
        // print all items in the categories list
        for (int i = 0; i < curNode->tableSize; ++i) {
            // make sure we dont print empty items
            if (!curNode->itemData[i].empty()) {
                // put leading 0's in front of the numbers
                string extra = "";
                if (i < 100) {
                    extra += "0";
                }
                if (i < 10) {
                    extra += "0";
                }
                // output item
                outputStream << categ << extra << i << ": " << curNode->itemData[i] << endl;
            }
        }
    }
    else {
        cout << "Error printing item: Could not find category" << endl;
    }


}

// Outputs a single item
void AVL::printSingleItem(string categ, int num, ostream& outputStream)
{
    // search for the category.
    Node* curNode = findCategory(categ);

    // if category was found, print out the item. Otherwise, complain.
    if (curNode) {

        // check if the number is valid and the spot isn't empty. If it is one of those, complain.
        if (num < curNode->tableSize && !curNode->itemData[num].empty()) {

            // add leading zeroes
            string extra = "";
            if (num < 100) {
                extra += "0";
            }
            if (num < 10) {
                extra += "0";
            }
            // output the item
            outputStream << categ << extra << num << ": " << curNode->itemData[num] << endl;
        }
        else {
            cout << "Could not find item number" << endl;
        }
    }
    else {
        cout << "Error inserting item: Could not find category" << endl;
    }
}

// output every item in the AVL tree from all categories.
void AVL::printEveryItem(ostream& outputStream)
{
    // call helper function to start at the root and print all items in order
    printItemHelper(root, outputStream);
}

// Adds a new item to a category, adding the current date to it's description. Returns SKU number.
int AVL::addItem(string categ, string desc)
{
    // search for the category
    Node* curNode = findCategory(categ);

    int emptySpot = 0; // variable to return later.

    // check if we found the category, complain otherwise
    if (curNode) {

        // find the first empty spot in the array
        bool spotNotFound = true;
        int foundSpot = curNode->tableSize;
        while (emptySpot < curNode->tableSize && spotNotFound) {
            ++emptySpot;
            if (curNode->itemData[emptySpot].empty()) {
                int foundSpot = emptySpot;
                spotNotFound = false;
            }
        }

        // get the current date.
        time_t now = time(0);
        string currentTime = asctime(localtime(&now));
        string monthDay = currentTime.substr(4, 6);
        string year = currentTime.substr(19, 5);

        // insert the item into the category, adding the date to its description.
        insertItem(categ, emptySpot, (monthDay + year + ". " + desc));

    }
    else {
        cout << "Could not find the category. Make sure the category exists and is spelled correctly." << endl;
    }

    // return the spot the item was added.
    return emptySpot;
}

// Removes an item from the category, returning the item removed.
string AVL::removeItem(string categ, int num)
{
    // search for the category
    Node* curNode = findCategory(categ);

    string infoToReturn = ""; // info to return later

    // check if we found the category, complain otherwise.
    if (curNode) {
        // check if the item exists
        if (num < curNode->tableSize && !curNode->itemData[num].empty()) {
            // set the infoToReturn variable to the item description before removing it.
            infoToReturn = curNode->itemData[num];
            curNode->itemData[num] = "";
        }
        else {
            cout << "Error- Could not find item." << endl;
        }
    }
    else {
        cout << "Error- Could not find category." << endl;
    }

    // return item description.
    return infoToReturn;
}

// Remove a category and its items. Call helper function.
void AVL::removeCategory(string categ)
{
    deleteNode(root, categ);
}

// prints the structure of the avl tree, for visualizing. Call helper function.
void AVL::printStructure(ostream& outputStream) {
    printTree(outputStream, root, 0);
}



// PRIVATE FUNCTIONS-



// recursive clear helper.
// Deletes nodes recursively in post-order, including the dynamic array and the node itself.
void AVL::clear(Node* node)
{
    if (node)
    {
        clear(node->left);
        clear(node->right);
        delete[] node->itemData;
        delete node;
    }
}

// Does a postOrder traversal of the original tree, doing a deep copy.
// Returns a pointer to the new root
AVL::Node* AVL::copy(AVL::Node* origNode)
{
    Node* nodeToReturn = nullptr;
    if (origNode)
    {
        // recursively copy the left and right subtrees
        Node* newLeft = copy(origNode->left);
        Node* newRight = copy(origNode->right);

        // copies the category's data from the original node
        string newData = origNode->idData;
        string newIdentifier = origNode->identifier;

        // create a new node with the copied data, and set its pointers to the left and right subtrees
        Node* node = new Node(newIdentifier, newData);
        node->left = newLeft;
        node->right = newRight;

        nodeToReturn = node;
    }
    return nodeToReturn;
}

// recursive insert, inserts new category into the AVL tree
void AVL::insert(string categ, string value, Node*& curPtr)
{
    // if no current node, this is where the category goes
    if (!curPtr) {
        curPtr = new Node(categ, value);
    }
    // heading left
    else if (categ < curPtr->identifier) {
        insert(categ, value, curPtr->left);
    }
    // heading right
    else if (categ > curPtr->identifier) {
        insert(categ, value, curPtr->right);
    }
    //value is found in the tree, do nothing.
    else {
        cout << "category already exists." << endl;
    }

    // update the height
    height(curPtr);

    //If imbalanced (height of left subtree - right subtree height isn't -1, 0, or 1), balance it.
    balanceTree(curPtr);
}

// helper function doing an inOrder traversal of the tree, printing out the descriptions for each category.
void AVL::printInOrder(ostream& outputStream, Node* curNode) const
{
    if (curNode->left)
        printInOrder(outputStream, curNode->left);
    outputStream << (curNode->idData) << endl;
    if (curNode->right)
        printInOrder(outputStream, curNode->right);
}

// this is doing a preOrder traversal of the tree, printing it out in a way that shows the structure
// indents to show the structure visually.
void AVL::printTree(ostream& outputStream, Node* curNode, int depth) const
{
    string indent = "  "; // dealing with weird compiler error
    string endOfLine = "\n";
    // add more indents the further along the tree we go
    for (int i = 0; i < depth; i++) {
        outputStream << indent;
    }
    // output current category
    outputStream << curNode->idData << endOfLine;
    // print left subtree
    if (curNode->left) {
        printTree(outputStream, curNode->left, depth + 1);
    }
    // print right subtree
    if (curNode->right) {
        printTree(outputStream, curNode->right, depth + 1);
    }
}

// performs a single right rotation on the current node
AVL::Node* AVL::singleRightRotation(Node* curNode)
{
    // create a new node to hold the left child
    Node* tempLeft = curNode->left;

    // perform the rotation, updating the pointers
    curNode->left = tempLeft->right;
    tempLeft->right = curNode;

    // update height of the new root.
    tempLeft->height = height(tempLeft);

    // return the new root
    return tempLeft;
}

// performs a single left rotation on the current node
AVL::Node* AVL::singleLeftRotation(Node* curNode)
{
    // create a new node to hold the right child
    Node* tempRight = curNode->right;

    // set the right of the root to whatever its right child's left node was
    curNode->right = tempRight->left;

    // set tempRight's left to be the original root.
    tempRight->left = curNode;

    // update heights
    tempRight->height = height(tempRight);

    // return new root
    return tempRight;
}

// updates the height of the current node and those below it.
int AVL::height(Node* curNode) {
    int heightFound;
    if (!curNode) {
        heightFound = -1;
    }
    else {
        heightFound = max(height(curNode->left), height(curNode->right)) + 1;
        curNode->height = heightFound;
    }
    return heightFound;
}

// helper function to output all items in the categories
void AVL::printItemHelper(Node* curNode, ostream& outputStream)
{
    // recursively traverse through all nodes, printing all the items in the categories as we go.
    if (curNode->left) {
        printItemHelper(curNode->left, outputStream);
    }
    printItemsInCategory(curNode->identifier, outputStream);
    if (curNode->right) {
        printItemHelper(curNode->right, outputStream);
    }
}

// delete a node, used for removeCategory
void AVL::deleteNode(Node*& givenNode, string categ)
{
    // no more nodes and no category. Complain.
    if (!givenNode) {
        cout << "Could not find category." << endl;
    }
    // category is smaller than givenNode, go left.
    else if (categ < givenNode->identifier) {
        deleteNode(givenNode->left, categ);
    }
    // category is bigger than givenNode, go right.
    else if (categ > givenNode->identifier) {
        deleteNode(givenNode->right, categ);
    }
    // this is the category
    else {
        Node* temp = givenNode;
        // if no children, just remove it.
        if (!givenNode->right && !givenNode->left) {
            givenNode = nullptr;
        }
        // if only right child, swap it with current node.
        else if (givenNode->right && !givenNode->left) {
            givenNode = givenNode->right;
        }
        // if only left child, swap it with current node.
        else if (!givenNode->right && givenNode->left) {
            givenNode = givenNode->left;
        }
        // if two children, remove the right side's left-most child up to its spot.
        else {
            Node* rightsLeast = retrieveLeast(givenNode->right, givenNode);
            rightsLeast->left = givenNode->left;
            givenNode->left = nullptr;

            if (givenNode->right != givenNode->right) {
                rightsLeast->right = givenNode->right;
            }
            givenNode->right = nullptr;


            givenNode = rightsLeast;


        }
        delete[] temp->itemData;
        delete temp;
    }
    if (givenNode) {
        height(givenNode);
        balanceTree(givenNode);
    }

}

//gets left-most child of the node. Assumes the node we're given exists. Replaces the node with its right child.
AVL::Node* AVL::retrieveLeast(Node* givenNode, Node* parentNode)
{
    Node* foundLeast = nullptr;

    // Base case: If the left child is null, we've found the leftmost node
    if (givenNode->left == nullptr) {
        foundLeast = givenNode;

        // if the leftmost node has a right child, it should replace it
        // reset the parentNodes pointer to point to the right node.
        if (parentNode->right != givenNode) {
            parentNode->left = givenNode->right;
        }

    }
    else {
        // Recursive case: Go left until we find the leftmost node
        foundLeast = retrieveLeast(givenNode->left, givenNode);
    }

    // balanceTree will help rebalance the tree as it goes back up
    balanceTree(givenNode);

    return foundLeast;
}

// helper function that determines if the node is unbalanced and uses rotations to try and balance the tree.
void AVL::balanceTree(Node*& curPtr)
{
    // initialize balance to default of -1 for things like nullptr.
    int balance = -1;

    // calculate the balance for the current node
    if (curPtr) {
        balance = height(curPtr->left) - height(curPtr->right);
    }

    // if the balance is greater than 1, the tree is left heavy
    if (balance > 1) {
        if (height(curPtr->left->left) >= height(curPtr->left->right)) {
            // Right rotation (Single rotation)
            curPtr = singleRightRotation(curPtr);
        }
        else {
            // Left-Right rotation (Double rotation)
            curPtr->left = singleLeftRotation(curPtr->left);
            curPtr = singleRightRotation(curPtr);
        }
    }
    // else if it's less than -1 then it's right heavy
    else if (balance < -1) {
        if (height(curPtr->right->right) >= height(curPtr->right->left)) {
            // Left rotation (Single rotation)
            curPtr = singleLeftRotation(curPtr);
        }
        else {
            // Right-Left rotation (Double rotation)
            curPtr->right = singleRightRotation(curPtr->right);
            curPtr = singleLeftRotation(curPtr);
        }
    }
}

// finds and returns a category given the category abbreviation (identifier)
AVL::Node* AVL::findCategory(const string& categ) const {
    Node* curNode = root;

    // traverse the tree starting from the root to find the category we're looking for.
    while (curNode && curNode->identifier != categ)
    {
        // go left if our category's identifier (abbreviation) is smaller than the current identifier
        if (curNode->identifier > categ)
        {
            curNode = curNode->left;
        }
        else
        {
            curNode = curNode->right;
        }
    }

    // return the category node if found, or nullptr if not found
    return curNode;
}

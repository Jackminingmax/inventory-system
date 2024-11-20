// Jack Maxwell
// 11/20/2024
// Small Inventory Management Program
// This program allows the user to create categories and add items, automatically generating SKU numbers and dates for them.
// Imports a txt file "inventory.txt" to store information in.


#include <iostream>
#include <fstream>
#include <string>
#include "Categories.h"
#include <algorithm>

using namespace std;

// Helper Functions to reduce code duplication-
void waitForUserInput();                                        // wait for user input before continuing
void capitalizeString(string& str);                             // convert a string to uppercase
bool parseSKU(const string& input, string& categ, int& num);    // extract the category and SKU number
string extractCategory(const string& descrip);                  // extract the category part of a description (before the '-')
string addLeadingZeroes(int num);                               // add leading zeroes for when outputting the SKU number.

int main()
{
    ostream& coutStream = cout;

    // Create an AVL Tree for managing categories:
    AVL Categories;

    // Open the inventory file to load data.
    ifstream inventory("inventory.txt");

    // Ensure the file is open, complain and exit if otherwise.
    if (!inventory.is_open()) {
        cout << "Error opening inventory file!" << endl;
        return 0;
    }


    string info = "";

    // Read from file, loading categories into our AVL tree.
    while (getline(inventory, info) && info != "-") {
        string categoryAbreviation = "";

        int i = 0;
        // Read the category abbreviation (no case-sensitivity).
        while (i <= info.length() && info[i] != '-') {
            categoryAbreviation += toupper(info[i]);
            ++i;
        }
        // Add category to AVL tree
        Categories.addCategory(categoryAbreviation, info);

    }




    // Loop through to read all the items from file, loading items into the corresponding categories in the AVL tree.
    while (getline(inventory, info) && info != "-") {

        //when adding entry, determine category by every value up until a numeric number
        int letter = 0;
        string categoryFound = "";

        // Get the category's identifier
        while (isalpha(info[letter])) {
            categoryFound += toupper(info[letter]);
            letter += 1;
        }


        string arrayNum = info.substr(letter, 3); //gets next 3 numbers after the category
        bool checkIfInt = true;

        //check if the SKU number is valid (if it is a number)
        for (int i = 0; i < arrayNum.length(); ++i) {
            if (!isdigit(arrayNum[i])) {
                checkIfInt = false;
            }
        }

        // If it is a valid SKU number, add the item to the category.
        if (checkIfInt) {
            string description = "ERROR- No description.";
            if (info.length() > letter + 5) {
                description = info.substr(letter + 5);
            }
            Categories.insertItem(categoryFound, stoi(arrayNum), description);
        }
        else {
            cout << "ERROR- Number provided was not an integer" << endl;
        }

        cout << endl << endl;
    }



    // File information has now been input.



    // Print all categories for the user.
    cout << "Printing Categories-\n";
    Categories.printCategoriesInOrder(cout);

    // Now give the user control. Continue to loop back while they aren't done.
    bool notDone = true;
    while (notDone) {
        // Display options to the user.
        cout << endl << "_____________\nHUB-\n";
        cout << "Type out one of the following - \n";
        cout << "help - Displays some potentially useful information.\n";
        cout << "0 - End the program.\n";
        cout << "1 - Bring up the categories list.\n";
        cout << "2 - Bring up all SKU's and their descriptions.\n";
        cout << "3 - Show all items in a specific category.\n";
        cout << "4 - Search for a specific item by its SKU.\n";
        cout << "5 - Add a new category to the categories list.\n";
        cout << "6 - Remove a category from the categories list.\n";
        cout << "7 - Add an item to the list.\n";
        cout << "8 - Delete an item from the list.\n" << endl;

        string input;
        cin >> input;

        // make sure the input is in uppercase if it's HELP. Don't want case sensitivity here.
        for (int i = 0; i < input.length(); ++i) {
            input[i] = toupper(input[i]);
        }

        //Handle user inputs:
        // Provide help information-
        if (input == "HELP") {
            cout << "Help Menu-\nKeep in mind that each category currently has 999 item slots.\n";
            cout << "If you delete something on accident, you can add it back in again. It should even print the item it just deleted.\n";
            cout << "If you mess up a description or category, you'll just have to delete the thing and start it again.\n";
            cout << "If you delete a category by accident or some other major change you want to undo, the inventory file doesn't update any changes until you exit the program with 0, so just exiting out will reverse the changes you've made.\n";
            cout << "If you need to do something that can't be done in the program, you can open the inventory text file and edit it there. Just make sure to make a copy before you start changing things.\n" << endl;
            cout << "Make sure you end the program by entering 0, otherwise changes won't be saved.\n";
        }


        // Exit the program-
        else if (input == "0") {
            notDone = false;
            cout << "Enter any key to exit.\nIf this was a mistake, enter * to go back." << endl;

            // Wait for user input, returning if it is a *.
            string nothing; cin.ignore(); getline(cin, nothing);
            if (nothing == "*") {
                notDone = true;
            }
        }


        //Display categories list-
        else if (input == "1") {
            cout << "Printing Categories-" << endl;
            Categories.printCategoriesInOrder(cout);

            // Wait for user input.
            waitForUserInput();
        }


        // Display all items-
        else if (input == "2") {
            cout << "Printing Items-" << endl;
            Categories.printEveryItem(coutStream);

            // Wait for user input.
            waitForUserInput();
        }


        // Display items from a certain category-
        else if (input == "3") {
            cout << "Printing Category- \nType in the category you wish to search for. Example: A" << endl;
            string input = "";
            cin >> input;
            capitalizeString(input);

            Categories.printItemsInCategory(input, coutStream);

            // Wait for user input.
            waitForUserInput();
        }


        // Search for an item by its SKU-
        else if (input == "4") {
            cout << "Searching For Item-\nType in the item you wish to search for. Example: A001" << endl;
            cin >> input;

            // retrieve the category abbreviation

            string categoryFound = "";
            int sku = 0;

            if (parseSKU(input, categoryFound, sku)) {

                // Search and print the tree
                Categories.printSingleItem(categoryFound, sku, coutStream);
            }
            else {
                cout << "Error parsing SKU, make sure it's input correctly." << endl;
            }

            // Wait for user input.
            waitForUserInput();
        }


        // Add a new category to the AVL tree-
        else if (input == "5") {
            cout << "Adding Category-\nEnter the label and description for the category, separated by a '-'. Example: A- Ladies Jeans" << endl;
            string descrip;

            // Get category and description from user input
            cin.ignore();
            getline(cin, descrip);

            // Get the abbreviation.
            string abbrev = extractCategory(descrip);

            // Capitalize the abbreviation.
            capitalizeString(abbrev);

            // Add the new category to the AVl tree
            Categories.addCategory(abbrev, descrip);
            cout << "New categories list:" << endl;
            Categories.printCategoriesInOrder(cout);

            // Wait for user input.
            waitForUserInput();
        }


        // Remove a category from the AVL tree-
        else if (input == "6") {

            // Get the category from the user
            cout << "Enter the abbreviation for the category you wish to remove. Example: A" << endl;
            string abbrev = "";
            cin >> abbrev;

            // Convert abbreviation to uppercase
            capitalizeString(abbrev);

            // Show items still in the category to the user (if any)
            cout << "Warning- This will also remove all of the following items in the category (if none listed, there are none):\n";
            Categories.printItemsInCategory(abbrev, coutStream);

            // Get confirmation from the user they want to proceed.
            cout << "Are you sure you wish to proceed?\nY/N: ";
            string confirm = "";
            cin >> confirm;
            capitalizeString(confirm);

            // If yes, remove the category and items from the AVL tree. Print the updated category list.
            if (confirm == "Y") {
                Categories.removeCategory(abbrev);
                cout << "New categories list:" << endl;
                Categories.printCategoriesInOrder(cout);
            }

            // Wait for user input.
            waitForUserInput();
        }


        // Create an item-
        else if (input == "7") {
            cout << "Creating Item-\nEnter the category of the item you want to create, followed by a '-' and the description. Example: MS- Description here" << endl;
            string descrip;

            // Get category and its description from the user. Need the full line so we're using cin.ignore().
            cin.ignore();
            getline(cin, descrip);

            // Convert the category abbreviation to uppercase until the '-' is found.
            string abbrev = extractCategory(descrip);

            // Capitalize the abbreviation.
            capitalizeString(abbrev);

            // Find the position of the first letter after the dash and update the description string to match.
            int findPartingChar = descrip.find('-');
            if (findPartingChar != -1 && (findPartingChar + 2) <= (descrip.length())) {
                descrip = descrip.substr(findPartingChar + 2);
            }

            // search for the category and if it's found it will add it to the category.
            // addItem returns the new item slot the item has.
            int slot = Categories.addItem(abbrev, descrip);

            // If valid SKU number: Format the SKU (adding leading 0's to the SKU number if necessary)
            if (slot != 0) {
                string leadingZeroes = addLeadingZeroes(slot);
                cout << "Success.\n" << abbrev << leadingZeroes << slot << " " << descrip << endl;
            }
            else {
                cout << "Invalid SKU number" << endl;
            }

            // Wait for user input.
            waitForUserInput();
        }


        // Remove an Item-
        else if (input == "8") {
            cout << "Removing Item-\nType out the SKU of the item you want to remove. Example: A001" << endl;

            // Get the SKU to remove from the user.
            string input;
            cin.clear();
            cin >> input;


            string category = "";
            int slot = 0;

            if (parseSKU(input, category, slot)) {
                // If valid SKU number: Format the SKU (adding leading 0's to the SKU number if necessary)
                if (slot != 0) {
                    string desc = Categories.removeItem(category, slot);
                    string leadingZeroes = addLeadingZeroes(slot);
                    cout << "Item removed: " << category << leadingZeroes << slot << " " << desc << endl;
                }
                else {
                    cout << "Invalid SKU number" << endl;
                }

            }
            else {
                cout << "Invalid SKU format" << endl;
            }

            // Wait for user input
            waitForUserInput();
        }

        // Improper input, complain-
        else {
            cout << "Improper input. Check and make sure you input everything correctly." << endl;
        }
        cout << endl;
    }

    // close the inventory file.
    inventory.close();

    // write back to the inventory file. More inefficient than updating the file in real time but shouldn't matter much in time complexity if there won't be that many items, which there aren't expected to be.
    ofstream inventoryOut("inventory.txt");
    Categories.printCategoriesInOrder(inventoryOut);
    inventoryOut << "-" << endl;
    Categories.printEveryItem(inventoryOut);

    cout << "Done. Have a nice day." << endl;

    return 0;
}





// Helper Functions to reduce some code duplication-

// wait for user input before continuing
void waitForUserInput() {
    cout << "Enter any key to go back to hub." << endl;
    string nothing;
    cin.ignore();
    getline(cin, nothing);  // Get user input (pause until the user is ready)
}

// convert a string to uppercase
void capitalizeString(string& str) {
    for (int i = 0; i < str.length(); ++i) {
        str[i] = toupper(str[i]);
    }
}

// extract the category and SKU number
bool parseSKU(const string& input, string& categ, int& num) {
    categ = "";
    string numToString = "0";
    int i = 0;
    bool toReturn = false;

    // Extract the category part (letters)
    while (isalpha(input[i])) {
        categ += toupper(input[i]);
        ++i;
    }

    // Extract the numeric part (digits)
    while (isdigit(input[i]) && i < input.length()) {
        numToString += input[i];
        ++i;
    }

    // Convert numeric part to integer if valid
    if (!numToString.empty()) {
        num = stoi(numToString);
        toReturn = true;
    }

    return toReturn;  // Return false if no valid number is found
}

// extract the category part of a description (before the '-')
string extractCategory(const string& descrip) {
    int i = 0;
    while (descrip[i] != '-' && i < descrip.length()) {
        ++i;
    }
    return descrip.substr(0, i);
}

// add leading zeroes for when outputting the SKU number.
string addLeadingZeroes(int num) {
    string extra = "";
    if (num < 100) {
        extra += "0";
    }
    if (num < 10) {
        extra += "0";
    }
    return extra;
}

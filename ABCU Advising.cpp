// ABCU Advising.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

struct Course {
    string courseId; // unique identifier
    string title;
    vector<string> prereq;
};

struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a bid
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};

class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
//    Node* removeNode(Node* node, string courseId);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    void Remove(string courseId);
    Course Search(string courseId);
   // void preOrder(Node* node);
   // void postOrder(Node* node);
   // void PreOrder();
   // void PostOrder();
};
/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    //root is equal to nullptr
    root = nullptr;
}
/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
    while (root != nullptr) {
        Remove(root->course.courseId);
    }
}
/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // call inOrder fuction and pass root 
    inOrder(root);
}
/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Course course) {
    // if root equarl to null ptr
    if (root == nullptr) {
        // root is equal to new node 
        root = new Node(course);
    }
    // else
    else {
        // add Node root and course
        addNode(root, course);
    }
}
/**
 * Search for a bid
 */
Course BinarySearchTree::Search(string courseId) {
    // set current node equal to root
    Node* current = root;
    Course course;
    // keep looping downwards until bottom reached or matching courseId found
    while (current != nullptr) {
        // if match found, return current bid
        if (courseId == current->course.courseId) {
            return current->course;
        }
        // if bid is smaller than current node then traverse left
        else if (courseId < current->course.courseId) {
            current = current->left;
        }
        // else larger so traverse right
        else {
            current = current->right;
        }
    }
    return course;
}

/**
 * Remove a bid
 */
void BinarySearchTree::Remove(string courseId) {
    // remove node root courseID

    Node* parent = nullptr;
    Node* successor = nullptr;
    Node* current = root;
    Node* temp;
    // Look for node
    while (current != nullptr) {
        // if node is found
        if (current->course.courseId == courseId) {
            // if node is leaf remove it.
            if (current->left == nullptr && current->right == nullptr) {
                if (parent == nullptr) {
                    root = nullptr;
                }
                else if (parent->left == current) {
                    parent->left = nullptr;
                }
                else {
                    parent->right = nullptr;
                }
            }
            // if node with only left child
            else if (current->right == nullptr) {
                if (parent == nullptr) {
                    root = current->left;
                }
                else if (parent->left == current) {
                    parent->left = current->left;
                }
                else {
                    parent->right = current->left;
                }
            }
            // if node with only right child
            else if (current->left == nullptr) {
                if (parent == nullptr) {
                    root = current->right;
                }
                else if (parent->left == current) {
                    parent->left = current->right;
                }
                else {
                    parent->right = current->right;
                }
            }
            // node has both right and left childen
            else {
                successor = current->right;
                while (successor->left != nullptr) {
                    successor = successor->left;
                }
                temp = successor;
                Remove(successor->course.courseId);
                current = temp;
            }
            return; // node removed
        }
        // Search right
        else if (current->course.courseId < courseId) {
            parent = current;
            current = current->right;
        }
        // Search left
        else {
            parent = current;
            current = current->left;
        }
    }
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param Course course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    // if node is larger then add to left
    if (course.courseId < node->course.courseId) {
        // if no left node
        if (node->left == nullptr) {
            // this node becomes left
            node->left = new Node(course);
        }
        // else recurse down the left node
        else {
            addNode(node->left, course);
        }
    }
    // else
    else {
        // if no right node
        if (node->right == nullptr) {
            // this node becomes right
            node->right = new Node(course);
        }
        //else
        else {
            // recurse down the left node
            addNode(node->right, course);
        }
    }
}

void BinarySearchTree::inOrder(Node* node) {
    //if node is not equal to null ptr
    if (node != nullptr) {
        //InOrder not left
        inOrder(node->left);
        //output courseID, title,
        cout << node->course.courseId << ": " << node->course.title;
        if (node->course.prereq.size() != 0) {
            for (int i = 0; i < node->course.prereq.size(); i++) {
                cout << ", " << node->course.prereq.at(i);
            }
        }
        cout << endl;
        //InOder right
        inOrder(node->right);
    }
}

void loadCourses(string csvPath, BinarySearchTree* bst) {

    // File pointer
    ifstream fin;

    // Open an existing file
    fin.open(csvPath, ios::in);

    // Read the Data from the file
    // as String Vector
    vector<string> row;
    string line, word;

    fin.seekg(3);
    getline(fin, line);

    cout << "Loading file";

    while (fin.peek()!=EOF) {
        

        //cout << line << endl;
        // used for breaking words
        stringstream s(line);

        // read every column data of a row and
        // store it in a string variable, 'word'
        while (getline(s, word, ',')) {

            // add all the column data
            // of a row to a vector
            row.push_back(word);
        }

        Course course;
        course.courseId = row.at(0);
        course.title = row.at(1);

        if (row.size() > 2) {
            for (int i = 2; i < row.size(); ++i) {
                course.prereq.push_back(row.at(i));
            }
        }

        bst->Insert(course);
        row.clear();
        line.clear();
        // read an entire row and
        // store it in a string variable 'line'
        getline(fin, line);
        cout << ".";
    }
    cout << "Courses loaded" << endl;
}
void displayMenu() {
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl << endl;
    cout << "What would you like to do? ";
}

int main(int argc, char* argv[]) {

        BinarySearchTree* bst;
        bst = new BinarySearchTree();
        string path, name;
        int option = 0;
        Course course;

        cout << "Welcome to the course planner." << endl << endl;

        while(option != 9){
        
            displayMenu();
            cin >> option;

            switch (option) {
            //Gets path to file and loads courses from that file 
            case 1:
                cout << "Enter file path to load courses from: ";
                cin >> path;
                loadCourses(path, bst);
                break;

            //Outputs loaded courses in alpabetical order
            case 2:
                bst->InOrder();
                break;

            //Searches for course and outputs information if found
            case 3:
                cout << "FIXME: print course" << endl;
                cout << "What course do you want to know about? ";
                cin >> name;
                
                //Convert lowercase to uppercase
                transform(name.begin(), name.end(), name.begin(), ::toupper);

                //Search for course
                course = bst->Search(name);
                if (!course.courseId.empty()) {
                    //Output course
                    cout << course.courseId << ", " << course.title << endl;
                    if (course.prereq.size() > 0) {

                        cout << "Prerequisites: ";

                        for (int i = 0; i < course.prereq.size(); ++i) {
                            cout << " " << course.prereq.at(i);
                        }
                        cout << endl;
                    }
                }
                break;
            
            //Exits program
            case 9:
                cout << "Good bye." << endl;
                break;
        
            //Invaild inputs
            default:
                cout << option << " is not a vaild option." << endl;
            }         
        }
        return 0;
}


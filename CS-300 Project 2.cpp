#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/*
-------------------------------------------------------------
  CS 300 - Project Two: Course Planner
  Oliver Rodriguez
  Description:
  This program loads course data from a CSV file into a
  Binary Search Tree (BST) and provides a menu for users
  to display all courses or view details for a specific course.
-------------------------------------------------------------
*/

// Structure to hold course information
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Node structure for Binary Search Tree
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course c) : course(c), left(nullptr), right(nullptr) {}
};

// Binary Search Tree class to store and manage courses
class CourseBST {
private:
    Node* root;

    // Helper function to insert a course into the tree
    Node* insert(Node* node, Course course) {
        if (node == nullptr) {
            return new Node(course);
        }
        if (course.courseNumber < node->course.courseNumber) {
            node->left = insert(node->left, course);
        }
        else {
            node->right = insert(node->right, course);
        }
        return node;
    }

    // Helper function to print courses in alphanumeric order
    void printInOrder(Node* node) {
        if (node != nullptr) {
            printInOrder(node->left);
            cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
            printInOrder(node->right);
        }
    }

    // Helper function to search for a course by course number
    Course* search(Node* node, string courseNumber) {
        if (node == nullptr) return nullptr;
        if (node->course.courseNumber == courseNumber) return &node->course;
        if (courseNumber < node->course.courseNumber) {
            return search(node->left, courseNumber);
        }
        return search(node->right, courseNumber);
    }

public:
    CourseBST() : root(nullptr) {}

    // Insert a course into the BST
    void insert(Course course) {
        root = insert(root, course);
    }

    // Print all courses in alphanumeric order
    void printAllCourses() {
        printInOrder(root);
    }

    // Search for a specific course
    Course* searchCourse(string courseNumber) {
        return search(root, courseNumber);
    }
};

// Helper function to trim whitespace from a string
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    size_t last = str.find_last_not_of(" \t\r\n");
    return (first == string::npos || last == string::npos) ? "" : str.substr(first, last - first + 1);
}

// Function to load courses from a file into the BST
void loadCourses(const string& filename, CourseBST& bst) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file '" << filename << "'." << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string courseNum, courseName, prereqField;
        vector<string> prereqs;

        getline(ss, courseNum, ',');
        getline(ss, courseName, ',');

        // Extract all prerequisites after the first two fields
        while (getline(ss, prereqField, ',')) {
            prereqField = trim(prereqField);
            if (!prereqField.empty()) prereqs.push_back(prereqField);
        }

        // Create a Course object and insert it into the BST
        Course c;
        c.courseNumber = trim(courseNum);
        c.courseTitle = trim(courseName);
        c.prerequisites = prereqs;

        bst.insert(c);
    }

    file.close();
    cout << "Courses successfully loaded from '" << filename << "'!" << endl;
}

// Function to print course information
void printCourseInfo(Course* course) {
    if (course == nullptr) {
        cout << "Course not found." << endl;
        return;
    }

    cout << course->courseNumber << ", " << course->courseTitle << endl;

    if (course->prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
    }
    else {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course->prerequisites.size(); ++i) {
            cout << course->prerequisites[i];
            if (i < course->prerequisites.size() - 1) cout << ", ";
        }
        cout << endl;
    }
}

// ------------------------------
// Main program
// ------------------------------
int main() {
    CourseBST bst;
    int choice = 0;

    // Hardcoded file name
    const string filename = "CS 300 ABCU_Advising_Program_Input.csv";

    cout << "Welcome to the course planner." << endl;

    // Main menu loop
    while (choice != 9) {
        cout << "\n1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit." << endl;
        cout << "What would you like to do? ";

        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Loading data from " << filename << "..." << endl;
            loadCourses(filename, bst);
            break;

        case 2:
            cout << "Here is a sample schedule:" << endl;
            bst.printAllCourses();
            break;

        case 3: {
            cout << "What course do you want to know about? ";
            string courseNum;
            cin >> courseNum;

            // Convert user input to uppercase
            transform(courseNum.begin(), courseNum.end(), courseNum.begin(), ::toupper);

            Course* c = bst.searchCourse(courseNum);
            printCourseInfo(c);
            break;
        }

        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;

        default:
            cout << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}

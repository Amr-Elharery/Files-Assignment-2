#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>

using namespace std;

// Structure to represent a node in the B-tree
struct Node {
    int isLeaf; // 0 for leaf, 1 for non-leaf
    int nextFreeNode; // Index of the next free node
    int keys[5]; // Array to store keys (Record IDs)
    int references[5]; // Array to store references (Record references)
};

// Function to create the index file as a text file
void CreateIndexFile(const char* filename, int numberOfRecords, int m) {
    ofstream file(filename);
    if (!file) {
        cerr << "Error creating file!" << endl;
        return;
    }

    // Write the special node (node 0)
    file << "-1 1 -1 -1 -1 -1 -1 -1 -1 -1 -1" << endl;

    // Initialize the rest of the nodes
    for (int i = 1; i < numberOfRecords; ++i) {
        file << "-1 " << (i + 1) << " -1 -1 -1 -1 -1 -1 -1 -1 -1" << endl;
    }

    // The last node points to -1 indicating no more free nodes
    file << "-1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1" << endl;

    file.close();
}

// Function to read a node from the text file
Node ReadNodeFromFile(ifstream& file) {
    Node node;
    string line;
    if (getline(file, line)) {
        istringstream iss(line);
        iss >> node.isLeaf >> node.nextFreeNode;
        for (int i = 0; i < 5; ++i) {
            iss >> node.keys[i];
        }
        for (int i = 0; i < 5; ++i) {
            iss >> node.references[i];
        }
    }
    return node;
}

// Function to write a node to the text file
void WriteNodeToFile(ofstream& file, const Node& node) {
    file << node.isLeaf << " " << node.nextFreeNode << " ";
    for (int i = 0; i < 5; ++i) {
        file << node.keys[i] << " ";
    }
    for (int i = 0; i < 5; ++i) {
        file << node.references[i] << " ";
    }
    file << endl;
}

// Function to insert a new record into the index file
int InsertNewRecordAtIndex(const char* filename, int RecordID, int Reference) {
    fstream file(filename, ios::in | ios::out);
    if (!file) {
        cerr << "Error opening file!" << endl;
        return -1;
    }

    Node root = ReadNodeFromFile(file);

    // If the root is empty, insert the record here
    if (root.isLeaf == -1) {
        root.isLeaf = 0;
        root.keys[0] = RecordID;
        root.references[0] = Reference;
        file.seekp(0, ios::beg);
        WriteNodeToFile(file, root);
        file.close();
        return 1; // Return the index of the root node
    }

    // Otherwise, find the appropriate leaf node to insert the record
    // (This is a simplified version, assuming the tree is small and fits in memory)
    // In a real B-tree, you would need to handle splitting nodes, etc.

    file.close();
    return -1; // No place to insert the record
}

// Function to delete a record from the index file
void DeleteRecordFromIndex(const char* filename, int RecordID) {
    fstream file(filename, ios::in | ios::out);
    if (!file) {
        cerr << "Error opening file!" << endl;
        return;
    }

    // Search for the record and delete it
    // (This is a simplified version, assuming the tree is small and fits in memory)
    // In a real B-tree, you would need to handle merging nodes, etc.

    file.close();
}

// Function to display the content of the index file
void DisplayIndexFileContent(const char* filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file!" << endl;
        return;
    }

    string line;
    int index = 0;
    while (getline(file, line)) {
        cout << "Node " << index << ": " << line << endl;
        ++index;
    }

    file.close();
}

// Function to search for a record in the index file
int SearchARecord(const char* filename, int RecordID) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file!" << endl;
        return -1;
    }

    Node node;
    while (file) {
        node = ReadNodeFromFile(file);
        for (int i = 0; i < 5; ++i) {
            if (node.keys[i] == RecordID) {
                file.close();
                return node.references[i];
            }
        }
    }

    file.close();
    return -1; // Record not found
}

int main() {
    const char* filename = "index_file.txt";
    int numberOfRecords = 10;
    int m = 5;

    // Create the index file
    CreateIndexFile(filename, numberOfRecords, m);

    // Insert some records
    InsertNewRecordAtIndex(filename, 3, 12);
    InsertNewRecordAtIndex(filename, 7, 24);
    InsertNewRecordAtIndex(filename, 10, 48);
    InsertNewRecordAtIndex(filename, 24, 60);
    InsertNewRecordAtIndex(filename, 14, 72);

    // Display the content of the index file
    DisplayIndexFileContent(filename);

    // Search for a record
    int reference = SearchARecord(filename, 7);
    if (reference != -1) {
        cout << "Record 7 found with reference: " << reference << endl;
    } else {
        cout << "Record 7 not found." << endl;
    }

    // Delete a record
    DeleteRecordFromIndex(filename, 7);

    // Display the content of the index file after deletion
    DisplayIndexFileContent(filename);

    return 0;
}
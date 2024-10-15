#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <ctime>

using namespace std;

// Node structure for BST
struct BSTNode {
    char character;
    int frequency;
    BSTNode* left;
    BSTNode* right;

    BSTNode(char ch) : character(ch), frequency(1), left(nullptr), right(nullptr) {}
};

// Binary Search Tree class
class BinarySearchTree {
private:
    BSTNode* root;

    void insert(BSTNode*& node, char character) {
        if (node == nullptr) {
            node = new BSTNode(character);
        } else if (character < node->character) {
            insert(node->left, character);
        } else if (character > node->character) {
            insert(node->right, character);
        } else {
            node->frequency++; // Increment frequency if character already exists
        }
    }

    void inOrderTraversal(BSTNode* node) {
        if (node != nullptr) {
            inOrderTraversal(node->left);
            cout << node->character << ": " << node->frequency << endl;
            inOrderTraversal(node->right);
        }
    }

public:
    BinarySearchTree() : root(nullptr) {}

    void insert(char character) {
        insert(root, character);
    }

    void displayFrequencies() {
        inOrderTraversal(root);
    }
};

// Node structure for Hash Table
struct HashNode {
    char character;
    int frequency;
    HashNode* next;

    HashNode(char ch) : character(ch), frequency(1), next(nullptr) {}
};

// Hash Table class
class HashTable {
private:
    HashNode** table;
    int size;

    int hashFunction(char character) {
        return tolower(character) % size; // Simple hash function
    }

public:
    HashTable(int s) : size(s) {
        table = new HashNode*[size];
        for (int i = 0; i < size; i++) {
            table[i] = nullptr;
        }
    }

    void insert(char character) {
        int index = hashFunction(character);
        HashNode* current = table[index];

        while (current != nullptr) {
            if (current->character == character) {
                current->frequency++;
                return;
            }
            current = current->next;
        }

        // If character not found, insert it
        HashNode* newNode = new HashNode(character);
        newNode->next = table[index];
        table[index] = newNode;
    }

    void displayFrequencies() {
        for (int i = 0; i < size; i++) {
            HashNode* current = table[i];
            while (current != nullptr) {
                cout << current->character << ": " << current->frequency << endl;
                current = current->next;
            }
        }
    }

    ~HashTable() {
        for (int i = 0; i < size; i++) {
            HashNode* current = table[i];
            while (current != nullptr) {
                HashNode* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] table;
    }
};

// Function to read character frequencies from file
void readCharacterFrequencies(const string& filename, BinarySearchTree& bst, HashTable& hashTable) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    char ch;
    while (file.get(ch)) {
        if (isalpha(ch)) {
            bst.insert(ch);
            hashTable.insert(ch);
        }
    }

    file.close();
}

// Function to generate text based on character frequencies
void generateText(const BinarySearchTree& bst, int length) {
    cout << "Generated Text: ";
    for (int i = 0; i < length; i++) {
        // Randomly select characters based on frequencies
        // Implement a simple character selection based on frequencies
        // Placeholder: for demonstration, it will print random characters
        cout << static_cast<char>('a' + rand() % 26); // Placeholder logic
    }
    cout << endl;
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed random number generator

    BinarySearchTree bst;
    HashTable hashTable(26); // Simple hash table of size 26 for lowercase letters

    // Read character frequencies from the file
    readCharacterFrequencies("merchant_of_venice.txt", bst, hashTable);

    // Display frequencies using both data structures
    cout << "Character Frequencies (BST):" << endl;
    bst.displayFrequencies();
    cout << endl;

    cout << "Character Frequencies (Hash Table):" << endl;
    hashTable.displayFrequencies();
    cout << endl;

    // Generate text based on frequencies
    int length;
    cout << "Enter the desired length of generated text: ";
    cin >> length;
    generateText(bst, length);

    return 0;
}

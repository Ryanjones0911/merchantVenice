#include <stdio.h>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <type_traits>

// formatted using formatter.org/cpp-formatter
// Google coding style
class CharDist {
   private:
    int counts[27];  // 27 counters for 26 letters + space
    int total;

   public:
    CharDist() {
        total = 0;
        for (int i = 0; i < 27; ++i) {
            counts[i] = 0;
        }
    }

    bool occurs(char c) {
        int index = c - 'a';  // get ASCII value of current character
        if (index < 0 || index >= 26) return false;
        return counts[index] > 0;
    }

    char getRandomChar() {
        if (total == 0) return ' ';  // In case of no counts, return space

        int randomIndex = rand() % total + 1;
        int cumulative = 0;

        for (int i = 0; i < 27; i++) {
            cumulative += counts[i];
            if (cumulative >= randomIndex) {
                return (i == 26) ? ' ' : ('a' + i);
            }
        }
        return ' ';
    }

    void increment(char c) {
        int index;
        if (c == ' ') {
            index = 26;
        } else if (c >= 'a' && c <= 'z') {
            index = c - 'a';  // Map 'a' to 0, 'b' to 1, ..., 'z' to 25
        } else {
            return;  // Skip invalid characters
        }
        counts[index]++;
        total++;
    }
};

template <typename K, typename V>
class BinarySearchTree {
   private:
    struct Node {
        K key;
        V value;
        Node *left, *right;
        Node(K k, V v) {
            key = k;
            value = v;
            left = nullptr;
            right = nullptr;
        }
    };

    Node* root;

    // Helper to insert a new (key, value)
    Node* insert(Node* node, K key, V value) {
        if (node == nullptr) {
            return new Node(key, value);
        }
        if (key < node->key) {
            node->left = insert(node->left, key, value);
        } else if (key > node->key) {
            node->right = insert(node->right, key, value);
        } else {
            node->value = value;  // Replace value if key exists
        }
        return node;
    }

    // Helper to find a key
    Node* find(Node* node, K key) {
        if (node == nullptr || node->key == key) {
            return node;
        }
        if (key < node->key) {
            return find(node->left, key);
        }
        return find(node->right, key);
    }
    // again just a constructor
   public:
    BinarySearchTree() { root = nullptr; }

    void insert(K key, V value) { root = insert(root, key, value); }

    V* find(K key) {
        Node* result = find(root, key);
        return result ? &result->value : nullptr;
    }

    bool isEmpty() const { return root == nullptr; }
};

typedef BinarySearchTree<std::string, CharDist> MapType;
void processInput(const std::string& text, int windowSize, MapType& map) {
    for (int i = 0; i + windowSize <= text.length(); i++) {
        std::string window = text.substr(i, windowSize);

        // Check for the next character after the window
        if (i + windowSize < text.length()) {
            char nextChar = text[i + windowSize];

            // Ignore invalid characters. This might be a leftover appendage
            // from before I put everything into one string, but I don't wanna
            // risk removing it
            if (!isprint(nextChar) || nextChar == '\n') {
                continue;
            }
            CharDist* dist = map.find(window);

            if (dist == nullptr) {
                CharDist newDist;             // Create a new distribution
                newDist.increment(nextChar);  // Increment for nextChar

                map.insert(window, newDist);
            } else {
                dist->increment(
                    nextChar);  // Increment for existing distribution
            }
        }
    }
}

std::string generateOutput(const std::string& inputText, int windowSize,
                           int length, MapType& map) {
    std::string output = inputText.substr(0, windowSize);

    for (int i = windowSize; i < length; ++i) {
        std::string window = output.substr(i - windowSize, windowSize);
        CharDist* dist = map.find(window);
        if (dist == nullptr) {
            output += ' ';  // Default to space if no distribution
        } else {
            output += dist->getRandomChar();
        }
    }

    return output;
}

int main() {
    srand(time(0));

    std::ifstream inputFile("merchant.txt");
    if (!inputFile) {
        std::cerr << "Error opening input file." << std::endl;
        return 1;
    }

    std::string text((std::istreambuf_iterator<char>(inputFile)),
                     std::istreambuf_iterator<char>());
    text.erase(
        remove(text.begin(), text.end(), '\n'),
        text.end());  // removes newlines from the file, makes everything easier

    int windowSize, outputLength;
    std::cout << "Enter window size: ";
    std::cin >> windowSize;
    std::cout << "Enter output text length: ";
    std::cin >> outputLength;

    if (windowSize > text.length() || windowSize < 0 || outputLength < 0) {
        std::cerr << "Exception occured during input: invalid input (maybe you "
                     "input something larger than file length or negative?) \n";
        return 1;
    }
    std::cout << "Processing input text..." << std::endl;

    MapType map;
    try {
        processInput(text, windowSize, map);
    } catch (const std::exception& e) {
        std::cerr << "Exception occurred during input processing: " << e.what()
                  << std::endl;
        return 1;
    }

    std::cout << "Input processing completed successfully." << std::endl;

    std::string output;
    try {
        output = generateOutput(text, windowSize, outputLength, map);
    } catch (const std::exception& e) {
        std::cerr << "Exception occurred during output generation: " << e.what()
                  << std::endl;
        return 1;
    }

    std::ofstream outputFile("output.txt");
    if (!outputFile) {
        std::cerr << "Error opening output file." << std::endl;
        return 1;
    }

    outputFile << output;
    std::cout << "Output text generated and written to output.txt."
              << std::endl;

    return 0;
}
#include<stdio.h>
#include<string>
#include<fstream>
#include<cmath>
#include<type_traits>
#include<iostream>
#include<algorithm>

class CharDistribution {
    private: int counts[27]; // 27 counters for 26 letters + space
    int total; // total occurrences of all characters
    public: CharDistribution(): total(0) {
        for (int i = 0; i < 27; ++i) {
            counts[i] = 0;
        }
    }

    bool occurs(char c) {
        int index = c - 'a';
        if (index < 0 || index >= 26) return false; // Bounds check
        return counts[index] > 0;
    }

    char getRandomChar() {
        if (total == 0) return ' '; // In case of no counts, return space

        int randomIndex = rand() % total + 1;
        int cumulative = 0;

        for (int i = 0; i < 27; i++) {
            cumulative += counts[i];
            if (cumulative >= randomIndex) {
                return (i == 26) ? ' ' : ('a' + i);
            }
        }
        return ' '; // fallback
    }

    void increment(char c) {
        int index;
        if (c == ' ') {
            index = 26; // Space character maps to the 27th index
        } else if (c >= 'a' && c <= 'z') {
            index = c - 'a'; // Map 'a' to 0, 'b' to 1, ..., 'z' to 25
        } else {
            return; // Skip invalid characters
        }
        counts[index]++;
        total++;
    }
};

template < typename K, typename V >
    class myHashMap {
        private:
            //simple linked list object to handle seperate chaining
            struct Node {
                K key;
                V value;
                Node * next;

                //constructor
                Node(K paramK, V paramV) {
                    key = paramK;
                    value = paramV;
                    next = nullptr;

                }
            };

        //This is not necessary for this project, but I wanted to make a more generically useful hashmap for fun
        int hash(K key) {
            //if int, just do the modulo and return
            if constexpr(std::is_integral < K > ::value) {
                return key % capacity;
            }
            //if string, get the ASCII int value of that string and hash that
            else if constexpr(std::is_same < K, std::string > ::value) {
                int sum = 0;

                //cast every individual char in the string to it's int value and add it to 'sum'
                for (char c: key) {
                    sum += static_cast < int > (c);
                }

                return sum % capacity;
            }
            else //I just don't wanna deal with doubles right now. Only integers and strings for the moment
            {
                throw std::invalid_argument("Invalid key type " + std::to_string(key) + " at line " + std::to_string(__LINE__));
            }
        }

        void resize() {
            int newSize = capacity * 2;
            Node ** newBucketsArray = new Node * [newSize];

            for (int i = 0; i < newSize; i++) {
                newBucketsArray[i] = nullptr;
            }

            //rehash everything
            {
                for (int i = 0; i < capacity; i++) {
                    Node * current = bucketsArray[i];
                    while (current != nullptr) {
                        Node * nextNode = current -> next;
                        int newIndex = hash(current -> key);
                        current -> next = newBucketsArray[newIndex];
                        newBucketsArray[newIndex] = current;
                        current = nextNode;
                    }
                }
                delete[] bucketsArray;
                bucketsArray = newBucketsArray;
                capacity = newSize;
            }
        }

        int sizeCurrent;
        int capacity;
        float loadFactor;
        Node ** bucketsArray;

        public:
            //constructor
            myHashMap(int startingCapacity = 20) {
                sizeCurrent = 0;
                capacity = startingCapacity;
                loadFactor = .7f;

                bucketsArray = new Node * [capacity];
                for (int i = 0; i < capacity; i++) {
                    bucketsArray[i] = nullptr;
                }
            }

            //simple destructor 
            ~myHashMap() {
                for (int i = 0; i < capacity; i++) {
                    Node * current = bucketsArray[i];
                    while (current != nullptr) {
                        Node * temp = current;
                        current = current -> next;
                        delete temp;
                    }
                }
                delete[] bucketsArray;
            }

        void removeElement(K key) {
            int index = hash(key);
            Node * current = bucketsArray[index];
            Node * prev = nullptr;

            while (current != nullptr) {
                if (current -> key == key) {
                    // If we're removing the head node
                    if (prev == nullptr) {
                        bucketsArray[index] = current -> next; // Update head of the bucket
                    } else {
                        prev -> next = current -> next; // Bypass the current node
                    }
                    delete current; // Free memory
                    sizeCurrent--; // Decrease the size
                    return; // Exit the function
                }
                prev = current; // Move prev to current
                current = current -> next; // Move to the next node
            }
            // If we reach here, the key was not found
            throw std::runtime_error("Key not found");
        }

        void insert(K key, V value) {
            int index = hash(key);

            Node * newNode = new Node(key, value);

            newNode -> next = bucketsArray[index];
            bucketsArray[index] = newNode;
            sizeCurrent++;

            //check load factor
            if ((static_cast < float > ((sizeCurrent)) / capacity) > loadFactor) {
                resize();
            }
        }

        int size() {
            return sizeCurrent;
        }

        bool empty() {
            if (sizeCurrent == 0) {
                return true;
            }
            return false;
        }

        V * find(K key) {
            int index = hash(key);

            Node * current = bucketsArray[index];

            while (current != nullptr) {
                if (current -> key == key) {
                    return & current -> value;
                }
                current = current -> next;
            }

            //throw std::runtime_error("The key '" + key + "' cannot be located in the hash map");
            return nullptr;

        }
        void display() {
            std::cout << "Hash Map Structure:" << std::endl;
            for (int i = 0; i < capacity; i++) {
                std::cout << "Bucket " << i << ": ";
                Node * current = bucketsArray[i];
                if (current == nullptr) {
                    std::cout << "empty"; // Indicate empty bucket
                } else {
                    while (current != nullptr) {
                        std::cout << "[" << current -> key << ": " << current -> value << "] ";
                        current = current -> next; // Move to the next node in the chain
                    }
                }
                std::cout << std::endl; // New line after each bucket
            }
            std::cout << "Total elements: " << sizeCurrent << std::endl; // Show total number of elements
        }

    };

template < typename K, typename V >
    class BinarySearchTree {
        private: struct Node {
            K key;
            V value;
            Node * left, * right;
            Node(K k, V v): key(k), value(v), left(nullptr), right(nullptr) {}
        };

        Node * root;

        // Helper to insert a new (key, value)
        Node * insert(Node * node, K key, V value) {
            if (node == nullptr) {
                return new Node(key, value);
            }
            if (key < node -> key) {
                node -> left = insert(node -> left, key, value);
            } else if (key > node -> key) {
                node -> right = insert(node -> right, key, value);
            } else {
                node -> value = value; // Replace value if key exists
            }
            return node;
        }

        // Helper to find a key
        Node * find(Node * node, K key) {
            if (node == nullptr || node -> key == key) {
                return node;
            }
            if (key < node -> key) {
                return find(node -> left, key);
            }
            return find(node -> right, key);
        }

        public: BinarySearchTree(): root(nullptr) {}

        void insert(K key, V value) {
            root = insert(root, key, value);
        }

        V * find(K key) {
            Node * result = find(root, key);
            return result ? & result -> value : nullptr;
        }

        bool isEmpty() const {
            return root == nullptr;
        }
    };

// Choose either BinarySearchTree or HashTable for the map implementation
typedef BinarySearchTree<std::string, CharDistribution> MapType;
//typedef myHashMap < std::string, CharDistribution > MapType;
void processInput(const std::string & text, int windowSize, MapType & map) {
    for (int i = 0; i + windowSize <= text.length(); ++i) {
        std::string window = text.substr(i, windowSize);

        // Check for the next character after the window
        if (i + windowSize < text.length()) {

            char nextChar = text[i + windowSize];

            // Ignore invalid characters
            if (!isprint(nextChar) || nextChar == '\n') {
                continue;
            }
            CharDistribution * dist = map.find(window);

            if (dist == nullptr) {
                CharDistribution newDist; // Create a new distribution
                newDist.increment(nextChar); // Increment for nextChar

                map.insert(window, newDist);
            } else {
                dist -> increment(nextChar); // Increment for existing distribution
            }
        } 
    }
}

std::string generateOutput(const std::string & inputText, int windowSize, int length, MapType & map) {
    std::string output = inputText.substr(0, windowSize);

    for (int i = windowSize; i < length; ++i) {
        std::string window = output.substr(i - windowSize, windowSize);
        CharDistribution * dist = map.find(window);
        if (dist == nullptr) {
            output += ' '; // Default to space if no distribution
        } else {
            output += dist -> getRandomChar();
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

    std::string text((std::istreambuf_iterator < char > (inputFile)), std::istreambuf_iterator < char > ());
    text.erase(remove(text.begin(), text.end(), '\n'), text.end());

    int windowSize, outputLength;
    std::cout << "Enter window size: ";
    std::cin >> windowSize;
    std::cout << "Enter output text length: ";
    std::cin >> outputLength;

    if(windowSize > text.length() || windowSize < 0 || outputLength < 0)
    {
        std::cerr << "Exception occured during input: invalid input (maybe you input something larger than file length or negative?) \n";
        return 1;
    }
    std::cout << "Processing input text..." << std::endl;

    MapType map;
    try {
        processInput(text, windowSize, map);
    } catch (const std::exception & e) {
        std::cerr << "Exception occurred during input processing: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Input processing completed successfully." << std::endl;

    std::string output;
    try {
        output = generateOutput(text, windowSize, outputLength, map);
    } catch (const std::exception & e) {
        std::cerr << "Exception occurred during output generation: " << e.what() << std::endl;
        return 1;
    }

    std::ofstream outputFile("output.txt");
    if (!outputFile) {
        std::cerr << "Error opening output file." << std::endl;
        return 1;
    }

    outputFile << output;
    std::cout << "Output text generated and written to output.txt." << std::endl;

    return 0;
}
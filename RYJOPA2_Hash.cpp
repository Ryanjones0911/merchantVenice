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
class myHashMap {
   private:
    // simple linked list object to handle seperate chaining
    struct Node {
        K key;
        V value;
        Node* next;

        // constructor
        Node(K paramK, V paramV) {
            key = paramK;
            value = paramV;
            next = nullptr;
        }
    };

    // This is not necessary for this project, but I wanted to make a more
    // generically useful hashmap for fun
    int hash(K key) {
        // if int, just do the modulo and return
        if constexpr (std::is_integral<K>::value) {
            return key % capacity;
        }
        // if string, get the ASCII int value of that string and hash that
        else if constexpr (std::is_same<K, std::string>::value) {
            int sum = 0;

            // cast every individual char in the string to it's int value and
            // add it to 'sum'
            for (char c : key) {
                sum += static_cast<int>(c);
            }

            return sum % capacity;
        } else  // I just don't wanna deal with doubles right now. Only integers
                // and strings for the moment
        {
            throw std::invalid_argument("Invalid key type " +
                                        std::to_string(key) + " at line " +
                                        std::to_string(__LINE__));
        }
    }

    void resize() {
        int newSize = capacity * 2;
        Node** newBucketsArray = new Node*[newSize];

        for (int i = 0; i < newSize; i++) {
            newBucketsArray[i] = nullptr;
        }

        // rehash everything
        {
            for (int i = 0; i < capacity; i++) {
                Node* current = bucketsArray[i];
                while (current != nullptr) {
                    Node* nextNode = current->next;
                    int newIndex = hash(current->key);
                    current->next = newBucketsArray[newIndex];
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
    Node** bucketsArray;

   public:
    // constructor
    myHashMap(int startingCapacity = 20) {
        sizeCurrent = 0;
        capacity = startingCapacity;
        loadFactor = .7f;

        bucketsArray = new Node*[capacity];
        for (int i = 0; i < capacity; i++) {
            bucketsArray[i] = nullptr;
        }
    }

    // simple destructor
    ~myHashMap() {
        for (int i = 0; i < capacity; i++) {
            Node* current = bucketsArray[i];
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] bucketsArray;
    }

    void removeElement(K key) {
        int index = hash(key);
        Node* current = bucketsArray[index];
        Node* prev = nullptr;

        while (current != nullptr) {
            if (current->key == key) {
                // If we're removing the head node
                if (prev == nullptr) {
                    bucketsArray[index] =
                        current->next;  // Update head of the bucket
                } else {
                    prev->next = current->next;  // Bypass the current node
                }
                delete current;  // Free memory
                sizeCurrent--;   // Decrease the size
                return;          // Exit the function
            }
            prev = current;           // Move prev to current
            current = current->next;  // Move to the next node
        }
        // If we reach here, the key was not found
        throw std::runtime_error("Key not found");
    }

    void insert(K key, V value) {
        int index = hash(key);

        Node* newNode = new Node(key, value);

        newNode->next = bucketsArray[index];
        bucketsArray[index] = newNode;
        sizeCurrent++;

        // check load factor
        if ((static_cast<float>((sizeCurrent)) / capacity) > loadFactor) {
            resize();
        }
    }

    int size() { return sizeCurrent; }

    bool empty() {
        if (sizeCurrent == 0) {
            return true;
        }
        return false;
    }

    V* find(K key) {
        int index = hash(key);

        Node* current = bucketsArray[index];

        while (current != nullptr) {
            if (current->key == key) {
                return &current->value;
            }
            current = current->next;
        }

        // throw std::runtime_error("The key '" + key + "' cannot be located in
        // the hash map");
        return nullptr;
    }
    // not useful for this project, just nice to have
    void display() {
        std::cout << "Hash Map Structure:" << std::endl;
        for (int i = 0; i < capacity; i++) {
            std::cout << "Bucket " << i << ": ";
            Node* current = bucketsArray[i];
            if (current == nullptr) {
                std::cout << "empty";  // Indicate empty bucket
            } else {
                while (current != nullptr) {
                    std::cout << "[" << current->key << ": " << current->value
                              << "] ";
                    current =
                        current->next;  // Move to the next node in the chain
                }
            }
            std::cout << std::endl;  // New line after each bucket
        }
        std::cout << "Total elements: " << sizeCurrent
                  << std::endl;  // Show total number of elements
    }
};

typedef myHashMap<std::string, CharDist> MapType;
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
        text.end());  // removes newlines from the file, makes everything easier.
                      // I don't know if this is strictly allowed, but it's not a 
                      // premade data structure and it's just saving me from
                      // writing string manipulation functions that were
                      // not the point of this project, so I felt it was
                      // okay. 

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
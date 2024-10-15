#include<stdio.h>
#include<string>
#include<fstream>
#include<cmath>
#include<type_traits>
#include<iostream>

/*
class charDistribution
{
    private:
        // data 
    public:
        charDistribution(// args );
        ~charDistribution();
    
    //constructor
    charDistribution(// args )
    {

    }

    //constructor
    ~charDistribution()
    {

    }   
};*/

template <typename K, typename V>
class myHashMap
{
    private:
        //simple linked list object to handle seperate chaining
        struct Node
        {
            K key;
            V value;
            Node* next;
         

            //constructor
            Node(K paramK, V paramV)
            {
                key = paramK;
                value = paramV;
                next = nullptr;
           
            }
        };

        //This is not necessary for this project, but I wanted to make a more generically useful hashmap for fun
        int hash(K key)
        {
            //if int, just do the modulo and return
            if constexpr (std::is_integral<K>::value)
            {
                return key % capacity;
            }
            //if string, get the ASCII int value of that string and hash that
            else if constexpr (std::is_same<K, std::string>::value)
            {
                int sum = 0;
                
                //cast every individual char in the string to it's int value and add it to 'sum'
                for(char c : key)
                {
                    sum += static_cast<int>(c);
                }

                return sum % capacity;
            }
            else //I just don't wanna deal with doubles right now. Only integers and strings for the moment
            {
                throw std::invalid_argument("Invalid key type " + std::to_string(key) + " at line " + std::to_string(__LINE__));
            }
        }

        void resize()
        {
            int newSize = capacity * 2;
            std::cout << "\nResizing from " << capacity << " to " << newSize << std::endl;
            Node** newBucketsArray = new Node*[newSize];
        

            for (int i = 0; i < newSize; i++)
            {
                newBucketsArray[i] = nullptr;
            }

            //rehash everything
            {
                for (int i = 0; i < capacity; i++)
                {
                    Node* current = bucketsArray[i];
                    while (current != nullptr)
                    {
                        Node* nextNode = current->next;
                        int newIndex = hash(current->key);
                        std::cout << "Rehashing key: " << current->key << " to new index: " << newIndex << std::endl;
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
        //constructor
        myHashMap(int startingCapacity = 20)
        {
            sizeCurrent = 0;
            capacity = startingCapacity;
            loadFactor = .7f;

            


            bucketsArray = new Node* [capacity];
            for(int i = 0; i < capacity; i++)
            {
                bucketsArray[i] = nullptr;
            }
        }
        
        //simple destructor 
        ~myHashMap()
        {
            for(int i = 0; i < capacity; i++)
            {
                Node* current = bucketsArray[i];
                while(current != nullptr)
                {
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
                bucketsArray[index] = current->next; // Update head of the bucket
            } else {
                prev->next = current->next; // Bypass the current node
            }
            delete current; // Free memory
            sizeCurrent--; // Decrease the size
            return; // Exit the function
        }
        prev = current; // Move prev to current
        current = current->next; // Move to the next node
    }
    // If we reach here, the key was not found
    throw std::runtime_error("Key not found");
}


    void insertElement(K key, V value)
    {
        int index = hash(key);
        
        Node* newNode = new Node(key, value);

        newNode->next = bucketsArray[index];
        bucketsArray[index] = newNode;
        sizeCurrent++;

        //check load factor
        if((static_cast<float>((sizeCurrent)) / capacity) > loadFactor)
        {
            resize();
        }
    }

    int size()
    {
        return sizeCurrent;
    }

    bool empty()
    {
        if(sizeCurrent == 0)
        {
            return true;
        }
        return false;
    }

    V find(K key)
    {
        int index = hash(key);

        Node* current = bucketsArray[index];

        while(current != nullptr)
        {
            if(current->key == key)
            {
                return current->value;
            }
            current = current->next;
        }

        throw std::runtime_error("The key '" + std::to_string(key) + "' cannot be located in the hash map");

    }
    void display() {
        std::cout << "Hash Map Structure:" << std::endl;
        for (int i = 0; i < capacity; i++) {
            std::cout << "Bucket " << i << ": ";
            Node* current = bucketsArray[i];
            if (current == nullptr) {
                std::cout << "empty"; // Indicate empty bucket
            } else {
                while (current != nullptr) {
                    std::cout << "[" << current->key << ": " << current->value << "] ";
                    current = current->next; // Move to the next node in the chain
                }
            }
            std::cout << std::endl; // New line after each bucket
        }
        std::cout << "Total elements: " << sizeCurrent << std::endl; // Show total number of elements
    }

};


/*
class binarySearchTree
{
    private:
        // data 
    public:
        binarySearchTree(// args );
        ~binarySearchTree();

    //constructor
    binarySearchTree(// args )
    {

    }

    //destructor
    ~binarySearchTree()
    {

    }
};*/






/*this will be the main logic, ideally just getting input and calling functions
int main()
{

//select either Binary Search Tree or Hashmap implementation



//scan input text and generate a character distribution


//return generated output text based on constructed char distribution
return 0;
}*/
int main()
{
    // Create an instance of myHashMap with a capacity of 10
    myHashMap<int, std::string> map(10);

    // Insert elements into the map that will cause collisions
    std::cout << "Inserting elements into the hash map (testing collisions):" << std::endl;
    map.insertElement(10, "ten");  // This will hash to index 0 (10 % 10 = 0)
    map.insertElement(20, "twenty"); // This will also hash to index 0 (20 % 10 = 0)
    map.insertElement(30, "thirty"); // This will hash to index 0 (30 % 10 = 0)
    map.insertElement(25, "twenty five");

    // Visualize the internal structure after insertions
    std::cout << "Hash map structure after insertions (with collisions):" << std::endl;
    map.display(); // Display the contents of the hash map

    // Retrieve elements to check if they were stored correctly
    try
    {
        std::cout << "Retrieving values:" << std::endl;
        std::cout << "Value for key 10: " << map.find(10) << std::endl;
        std::cout << "Value for key 20: " << map.find(20) << std::endl;
        std::cout << "Value for key 30: " << map.find(30) << std::endl;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }

    // Remove an element and visualize the structure again
    std::cout << "\nRemoving key 30" << std::endl;
    map.removeElement(30);
    
    // Visualize the internal structure after removal
    std::cout << "Hash map structure after removing key 30:" << std::endl;
    map.display();

    // Try to find the removed key
    try
    {
        std::cout << "Trying to find removed key 30: ";
        std::cout << map.find(20) << std::endl; // This should throw an error
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl; // Should print an error message
    }

    // Display size and emptiness
    std::cout << "\nMap size: " << map.size() << std::endl;
    std::cout << "Is map empty? " << (map.empty() ? "Yes" : "No") << std::endl;

    return 0;
}

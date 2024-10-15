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
            Node* prev;

            //constructor
            Node(K paramK, V paramV)
            {
                key = paramK;
                value = paramV;
                next = nullptr;
                prev = nullptr;
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
                std::cerr << "Invalid key value type" << std::endl;
                return -1;
            }
        }

        void resize()
        {
            int newSize = capacity * 2;
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
                        int newIndex = hash(current->key) % newSize;
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

    void removeElement(K key)
    {
        int index = hash(key);
        Node* current = bucketsArray[index];

        while (current != nullptr)
        {
            if (current->key == key)
            {
                // Update pointers to remove current node
                if (current->prev == nullptr) // head of the list
                {
                    bucketsArray[index] = current->next;
                    if (current->next != nullptr)
                    {
                        current->next->prev = nullptr; // update the next node's prev
                    }
                }
                else
                {
                    current->prev->next = current->next;
                    if (current->next != nullptr)
                    {
                        current->next->prev = current->prev; // link the next node back
                    }
                }
                delete current;
                sizeCurrent--;
                return;
            }
            current = current->next;
        }
    }

    void insertElement(K key, V value)
    {
        int index = hash(key);
        
        Node* newNode = new Node(key, value);

        newNode->next = bucketsArray[index];
        bucketsArray[index] = newNode;
        sizeCurrent++;

        //check load factor
        if(static_cast<float>(sizeCurrent / capacity) > loadFactor)
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

        throw std::runtime_error("This key cannot be located");
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






//this will be the main logic, ideally just getting input and calling functions
int main()
{

//select either Binary Search Tree or Hashmap implementation

myHashMap<std::string, std::string> testing;

testing.insertElement("this is the key","this is the value");
std::cout << testing.find("this is the key") << '\n';
std::cout << "current size is " << testing.size() << '\n';
std::cout << testing.empty() << '\n';

testing.removeElement("this is the key");
std::cout << "current size is " << testing.size() << '\n';
std::cout << testing.empty();


//get user input for window


//scan input text and generate a character distribution


//return generated output text based on constructed char distribution
return 0;
}
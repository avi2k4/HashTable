//
//  main.cpp
//  HashTable
//
//  Created by Avighnash Kumar on 4/4/23.
//

#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>

#define NAME_SIZE 25

// Student class declaration
class Student {
public:
    int id;
    char * first_name;
    char * last_name;
    double gpa;
    
    Student(int id, char * first_name, char * last_name, float gpa) {
        this->id = id;
        this->first_name = new char[NAME_SIZE];
        strcpy(this->first_name, first_name);
        this->last_name = new char[NAME_SIZE];
        strcpy(this->last_name, last_name);
        this->gpa = gpa;
    }
    
    ~Student() {
        delete this->first_name;
        delete this->last_name;
    }
};

// Node struct declaration
struct Node {
    Student * student;
    struct Node * next;
};

// HashTable class declaration
class HashTable {
private:
    static const int maxCollisionCount = 3;
    int size;
    struct Node ** table;
    
    // Hash function, dependent on declared size of table
    int hash(int key, int size) {
        return key % size;
    }
    
    // Recursively deletes linked list
    void deleteLL(struct Node * current) {
        if (current == NULL) {
            return;
        }
        
        this->deleteLL(current->next);
        delete(current);
    }
    
    // Initializes every node to NULL in the table
    void init() {
        for (int i = 0; i < this->size; i++) {
            this->table[i] = NULL;
        }
    }
    
    void rehash() {
        Node ** old_table = this->table;
        int old_size = this->size;

        // Double size
        this->size *= 2;
        this->table = new Node * [this->size];

        this->init();

        // Reinsert all students into new table
        for (int i = 0; i < old_size; i++) {
            Node * current = old_table[i];
            
            while (current != NULL) {
                this->insert(new Student(current->student->id, current->student->first_name, current->student->last_name, current->student->gpa));
            }
        }

        // Delete the old table
        delete[] old_table;
    }
    
public:
    HashTable(int size) {
        this->size = size;
        this->table = new Node * [size];
        
        // Prevent memory leak
        this->init();
    }
    
    ~HashTable() {
        // Deallocate every node
        for (int i = 0; i < this->size; i++) {
            this->deleteLL(this->table[i]);
        }
    }
    
    // Insert student into linked list, using chaining
    void insert(Student * student) {
        int location = this->hash(student->id, this->size);
        bool doesExist = this->table[location] != NULL;
        
        struct Node * node = new Node();
        node->student = student;
        node->next = NULL;
        
        int collisionCount = 0;
        
        // Doesn't exist so is first in chain
        if (!doesExist) {
            this->table[location] = node;
        }
        else {
            // Set collission count to 1 because student with that hash code exists
            collisionCount += 1;
            struct Node * current = this->table[location];
            
            while (current != NULL) {
                if (current->next == NULL) {
                    current->next = node;
                    break;
                }
                
                current = current->next;
                collisionCount += 1;
            }
        }
        
        // If # of collisions than the max allowed # of collision, rehash
        if (collisionCount >= this->maxCollisionCount) {
            this->rehash();
        }
    }
    
    // Fetch the student with the ID
    Node * get(int id) {
        int location = this->hash(id, this->size);
        bool doesExist = this->table[location] != NULL;
        
        // Exit with NULL if student with ID not found
        if (!doesExist) {
            return NULL;
        }
        
        struct Node * current = this->table[location];
        
        while (current != NULL) {
            // Can exit early because first student with ID was found
            if (current->student->id == id) {
                return current;
            }
            
            current = current->next;
        }
        
        return current;
    }
    
    // Delete and return bool for success. If student with ID doesn't exist, then returns false
    bool del(int id) {
        int location = this->hash(id, this->size);
        bool doesExist = this->table[location] != NULL;
        
        // Exit with false because student doesn't exist
        if (!doesExist) {
            return false;
        }
        
        struct Node * current = this->table[location];
        
        // If is the only node in the chain, then just delete
        if (current->next == NULL) {
            delete current;
            this->table[location] = NULL;
        }
        else {
            struct Node * previous = NULL;
            
            // Fetch the right node
            while (current->next != NULL) {
                if (current->student->id == id) {
                    break;
                }
               
                previous = current;
                current = current->next;
            }
            
            // If is the first node in the chain
            if (previous == NULL) {
                this->table[location] = current->next;
                delete current;
            }
            else {
                previous->next = current->next;
                delete current;
            }
        }

        return true;
    }
    
    // Iteratively print every student
    void print() {
        for (int i = 0; i < this->size; i++) {
            if (this->table[i] != NULL) {
                struct Node * current = this->table[i];
                
                while (current != NULL) {
                    Student * student = current->student;
                    
                    std::cout << "---------------------------" << std::endl;
                    std::cout << "ID: " << student->id << std::endl;
                    std::cout << "Name: " << student->first_name << " " << student->last_name << std::endl;
                    std::cout << "GPA: " << std::setprecision(2) << std::fixed << student->gpa << std::endl;
                    std::cout << "---------------------------" << std::endl;

                    current = current->next;
                }
            }
        }
    }
    
    // Deletes every node in the table
    void deleteAll() {
        for (int i = 0; i < this->size; i++) {
            this->deleteLL(this->table[i]);
            this->table[i] = NULL;
        }
    }
};

// Generate random GPA
float randomGPA() {
    return static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / 4));
}

// Generate random students and automatically insert into the hash table
void generateRandomStudentsAndInsert(HashTable * table, int count) {
    std::vector<char *> first_names;
    std::vector<char *> last_names;
    
    std::ifstream first_names_file("first_names.txt");
    std::ifstream last_names_file("last_names.txt");
    
    // Check if files exist
    if (!first_names_file) {
        std::cout << "First names file doesn't exist. Exiting." << std::endl;
        return;
    }
    
    if (!last_names_file) {
        std::cout << "Last names file doesn't exist. Exiting." << std::endl;
        return;
    }
    
    char first_name[NAME_SIZE];
    while (first_names_file >> first_name) {
        char * copy = new char[NAME_SIZE];
        strcpy(copy, first_name);
        first_names.push_back(copy);
    }
    
    char last_name[NAME_SIZE];
    while (last_names_file >> last_name) {
        char * copy = new char[NAME_SIZE];
        strcpy(copy, last_name);
        last_names.push_back(copy);
    }

    // Iterate over count of students to be generated, create new student, and insert
    for (int id = 0; id < count; id++) {
        char * chosen_first_name = first_names[std::rand() % first_names.size()];
        char * chosen_last_name = last_names[std::rand() % last_names.size()];

        table->insert(new Student(id, chosen_first_name, chosen_last_name, randomGPA()));
    }
}

int main() {
    HashTable * table = new HashTable(1000);
    
    std::cout << "\nWelcome to your student hashtable! Here are the commands:" << std::endl;
    std::cout << "[1] to add a student" << std::endl;
    std::cout << "[2] to delete a student, by id" << std::endl;
    std::cout << "[3] to print all students" << std::endl;
    std::cout << "[4] to generate random students" << std::endl;
    std::cout << "[5] to quit" << std::endl;
    
    while (true) {
        std::cout << "\nPerform a command" << std::endl;

        int input;
        std::cin >> input;

        // Add student
        if (input == 1) {
            std::cout << "Enter the id:" << std::endl;
            int id;
            std::cin >> id;
            
            std::cout << "Enter the first name:" << std::endl;
            char first_name[NAME_SIZE];
            std::cin >> first_name;
            
            std::cout << "Enter the last name:" << std::endl;
            char last_name[NAME_SIZE];
            std::cin >> last_name;
            
            std::cout << "Enter the GPA:" << std::endl;
            float gpa;
            std::cin >> gpa;
            
            table->insert(new Student(id, first_name, last_name, gpa));
            std::cout << "Added student with id: " << id << std::endl;
        }
        // Delete student by ID
        else if (input == 2) {
            std::cout << "Enter the id of the student you would like to delete:" << std::endl;
            int id_input;
            std::cin >> id_input;
            
            if (table->del(id_input)) {
                std::cout << "Student with id " << id_input << " successfully deleted!" << std::endl;
            }
            else {
                std::cout << "Student doesn't exist" << std::endl;
            }
        }
        // Print a;; students
        else if (input == 3) {
            table->print();
        }
        // Generate # of students
        else if (input == 4) {
            std::cout << "Enter the number of students you would like to generate:" << std::endl;
            int generate_input;
            std::cin >> generate_input;
            
            generateRandomStudentsAndInsert(table, generate_input);
        }
        else if (input == 5) {
            break;
        }
    }
    
    return 0;
}
